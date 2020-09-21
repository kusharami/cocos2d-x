/* Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 * Copyright (c) 2012 cocos2d-x.org
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 * Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "2d/CCDrawNode.h"
#include <stddef.h> // offsetof
#include "base/ccTypes.h"
#include "base/CCEventType.h"
#include "base/CCConfiguration.h"
#include "renderer/CCRenderer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventDispatcher.h"
#include "2d/CCActionCatmullRom.h"
#include "base/ccUtils.h"
#include "renderer/ccShaders.h"
#include "renderer/backend/ProgramState.h"

NS_CC_BEGIN

static inline Tex2F v2ToTex2F(const Vec2 &v)
{
    return {v.x, v.y};
}

// implementation of DrawNode

DrawNode::DrawNode(float lineWidth)
: _lineWidth(lineWidth)
, _defaultLineWidth(lineWidth)
{
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
#if CC_ENABLE_CACHE_TEXTURE_DATA
    //TODO new-renderer: interface setupBuffer removal

    // Need to listen the event only when not use batchnode, because it will use VBO
//    auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
//        /** listen the event that renderer was recreated on Android/WP8 */
//        this->setupBuffer();
//    });

//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
}

DrawNode::~DrawNode()
{
    free(_buffer);
    _buffer = nullptr;
    free(_bufferGLPoint);
    _bufferGLPoint = nullptr;
    free(_bufferGLLine);
    _bufferGLLine = nullptr;
    
    CC_SAFE_RELEASE(_programState);
    CC_SAFE_RELEASE(_programStatePoint);
    CC_SAFE_RELEASE(_programStateLine);
}

DrawNode* DrawNode::create(float defaultLineWidth)
{
    DrawNode* ret = new (std::nothrow) DrawNode(defaultLineWidth);
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

void DrawNode::ensureCapacity(int count)
{
    reserve(_bufferCount + count, TRIANGLES);
}

void DrawNode::ensureCapacityGLPoint(int count)
{
    reserve(_bufferCountGLPoint + count, POINTS);
}

void DrawNode::ensureCapacityGLLine(int count)
{
    reserve(_bufferCountGLLine + count, LINES);
}

bool DrawNode::init()
{
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    updateShader();
    reserve(18, TRIANGLES);
    reserve(4, POINTS);
    reserve(8, LINES);
    
    _dirty = true;
    _dirtyGLLine = true;
    _dirtyGLPoint = true;
    return true;
}

void DrawNode::updateShader()
{
    CC_SAFE_RELEASE(_programState);
    auto* program = backend::Program::getBuiltinProgram(backend::ProgramType::POSITION_COLOR_LENGTH_TEXTURE);
    _programState = new (std::nothrow) backend::ProgramState(program);
    _customCommand.getPipelineDescriptor().programState = _programState;
    setVertexLayout(_customCommand);
    _customCommand.setDrawType(CustomCommand::DrawType::ARRAY);
    _customCommand.setPrimitiveType(CustomCommand::PrimitiveType::TRIANGLE);

    CC_SAFE_RELEASE(_programStatePoint);
    program = backend::Program::getBuiltinProgram(backend::ProgramType::POSITION_COLOR_TEXTURE_AS_POINTSIZE);
    _programStatePoint = new (std::nothrow) backend::ProgramState(program);
    _customCommandGLPoint.getPipelineDescriptor().programState = _programStatePoint;
    setVertexLayout(_customCommandGLPoint);
    _customCommandGLPoint.setDrawType(CustomCommand::DrawType::ARRAY);
    _customCommandGLPoint.setPrimitiveType(CustomCommand::PrimitiveType::POINT);

    CC_SAFE_RELEASE(_programStateLine);
    program = backend::Program::getBuiltinProgram(backend::ProgramType::POSITION_COLOR_LENGTH_TEXTURE);
    _programStateLine = new (std::nothrow) backend::ProgramState(program);
    _customCommandGLLine.getPipelineDescriptor().programState = _programStateLine;
    setVertexLayout(_customCommandGLLine);
    _customCommandGLLine.setDrawType(CustomCommand::DrawType::ARRAY);
    _customCommandGLLine.setPrimitiveType(CustomCommand::PrimitiveType::LINE);
}

void DrawNode::setVertexLayout(CustomCommand& cmd)
{
    auto* programState = cmd.getPipelineDescriptor().programState;
    auto layout = programState->getVertexLayout();
    const auto& attributeInfo = programState->getProgram()->getActiveAttributes();
    auto iter = attributeInfo.find("a_position");
    if(iter != attributeInfo.end())
    {
        layout->setAttribute("a_position", iter->second.location, backend::VertexFormat::FLOAT2, 0, false);
    }
    
    iter = attributeInfo.find("a_texCoord");
    if(iter != attributeInfo.end())
    {
        layout->setAttribute("a_texCoord", iter->second.location, backend::VertexFormat::FLOAT2, offsetof(V2F_C4B_T2F, texCoords), false);
    }
    
    iter = attributeInfo.find("a_color");
    if(iter != attributeInfo.end())
    {
        layout->setAttribute("a_color", iter->second.location, backend::VertexFormat::UBYTE4, offsetof(V2F_C4B_T2F, colors), true);
    }
    layout->setLayout(sizeof(V2F_C4B_T2F));
}

void DrawNode::updateBlendState(CustomCommand& cmd)
{
    backend::BlendDescriptor& blendDescriptor = cmd.getPipelineDescriptor().blendDescriptor;
    blendDescriptor.blendEnabled = true;
    if (_blendFunc == BlendFunc::ALPHA_NON_PREMULTIPLIED)
    {
        blendDescriptor.sourceRGBBlendFactor = backend::BlendFactor::SRC_ALPHA;
        blendDescriptor.destinationRGBBlendFactor = backend::BlendFactor::ONE_MINUS_SRC_ALPHA;
        blendDescriptor.sourceAlphaBlendFactor = backend::BlendFactor::SRC_ALPHA;
        blendDescriptor.destinationAlphaBlendFactor = backend::BlendFactor::ONE_MINUS_SRC_ALPHA;
        setOpacityModifyRGB(false);
    }
    else
    {
        blendDescriptor.sourceRGBBlendFactor = backend::BlendFactor::ONE;
        blendDescriptor.destinationRGBBlendFactor = backend::BlendFactor::ONE_MINUS_SRC_ALPHA;
        blendDescriptor.sourceAlphaBlendFactor = backend::BlendFactor::ONE;
        blendDescriptor.destinationAlphaBlendFactor = backend::BlendFactor::ONE_MINUS_SRC_ALPHA;
        setOpacityModifyRGB(true);
    }
}

void DrawNode::updateUniforms(const Mat4 &transform, CustomCommand& cmd)
{
    auto& pipelineDescriptor = cmd.getPipelineDescriptor();
    const auto& matrixP = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Mat4 matrixMVP = matrixP * transform;
    auto mvpLocation = pipelineDescriptor.programState->getUniformLocation("u_MVPMatrix");
    pipelineDescriptor.programState->setUniform(mvpLocation, matrixMVP.m, sizeof(matrixMVP.m));

    float alpha = _displayedOpacity / 255.0f;
    auto alphaUniformLocation = pipelineDescriptor.programState->getUniformLocation("u_alpha");
    pipelineDescriptor.programState->setUniform(alphaUniformLocation, &alpha, sizeof(alpha));
}

void DrawNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t)
{
    if(_bufferCount)
    {
        updateBlendState(_customCommand);
        updateUniforms(transform, _customCommand);
        _customCommand.init(_globalZOrder);
        renderer->addCommand(&_customCommand);
    }
    
    if(_bufferCountGLPoint)
    {
        updateBlendState(_customCommandGLPoint);
        updateUniforms(transform, _customCommandGLPoint);
        _customCommandGLPoint.init(_globalZOrder);
        renderer->addCommand(&_customCommandGLPoint);
    }
    
    if(_bufferCountGLLine)
    {
        updateBlendState(_customCommandGLLine);
        updateUniforms(transform, _customCommandGLLine);
        _customCommandGLLine.setLineWidth(_lineWidth);
        _customCommandGLLine.init(_globalZOrder);
        renderer->addCommand(&_customCommandGLLine);
    }
}


void DrawNode::drawPoint(const Vec2 &position, const float pointSize, const Color4B &color)
{
    ensureCapacityGLPoint(1);
    
    V2F_C4B_T2F *point = _bufferGLPoint + _bufferCountGLPoint;
    *point = {position, color, Tex2F(pointSize,0)};
    
    _customCommandGLPoint.updateVertexBuffer(point, _bufferCountGLPoint*sizeof(V2F_C4B_T2F), sizeof(V2F_C4B_T2F));
    _bufferCountGLPoint += 1;
    _dirtyGLPoint = true;
    _customCommandGLPoint.setVertexDrawInfo(0, _bufferCountGLPoint);   
}

void DrawNode::drawPoint(const Vec2& position, const float pointSize, const Color4F &color)
{
    drawPoint(position, pointSize, Color4B(color));
}

void DrawNode::drawPoints(const Vec2 *position, unsigned int numberOfPoints, const Color4B &color)
{
    drawPoints(position, numberOfPoints, 1.0, color);
}

void DrawNode::drawPoints(const Vec2 *position, unsigned int numberOfPoints, const Color4F &color)
{
    drawPoints(position, numberOfPoints, 1.0, color);
}

void DrawNode::drawPoints(const Vec2 *position, unsigned int numberOfPoints, const float pointSize, const Color4B &color)
{
    ensureCapacityGLPoint(numberOfPoints);
    
    V2F_C4B_T2F *point = _bufferGLPoint + _bufferCountGLPoint;
    for(unsigned int i=0; i < numberOfPoints; i++)
    {
        *(point + i) = {position[i], color, Tex2F(pointSize,0)};
    }
    
    _customCommandGLPoint.updateVertexBuffer(point, _bufferCountGLPoint*sizeof(V2F_C4B_T2F), numberOfPoints*sizeof(V2F_C4B_T2F));
    _bufferCountGLPoint += numberOfPoints;
    _dirtyGLPoint = true;
    _customCommandGLPoint.setVertexDrawInfo(0, _bufferCountGLPoint);
}

void DrawNode::drawPoints(const Vec2 *position, unsigned int numberOfPoints, const float pointSize, const Color4F &color)
{
    drawPoints(position, numberOfPoints, pointSize, Color4B(color));
}

void DrawNode::drawLine(const Vec2 &origin, const Vec2 &destination, const Color4B &color)
{
    ensureCapacityGLLine(2);
    
    V2F_C4B_T2F *point = _bufferGLLine + _bufferCountGLLine;
    
    *point = {origin, color, Tex2F(0.0, 0.0)};
    *(point+1) = {destination, color, Tex2F(0.0, 0.0)};
    
    _customCommandGLLine.updateVertexBuffer(point, _bufferCountGLLine*sizeof(V2F_C4B_T2F), 2*sizeof(V2F_C4B_T2F));
    _bufferCountGLLine += 2;
    _dirtyGLLine = true;
    _customCommandGLLine.setVertexDrawInfo(0, _bufferCountGLLine);
}

void DrawNode::drawLine(const Vec2 &origin, const Vec2 &destination, const Color4F &color)
{
    drawLine(origin, destination, Color4B(color));
}

void DrawNode::drawRect(const Vec2 &origin, const Vec2 &destination, const Color4B &color)
{
    drawLine(origin, Vec2(destination.x, origin.y), color);
    drawLine(Vec2(destination.x, origin.y), destination, color);
    drawLine(destination, Vec2(origin.x, destination.y), color);
    drawLine(Vec2(origin.x, destination.y), origin, color);
}

void DrawNode::drawRect(const Vec2 &origin, const Vec2 &destination, const Color4F &color)
{
    drawRect(origin, destination, Color4B(color));
}

void DrawNode::drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4B &color)
{
    unsigned int vertex_count;
    if(closePolygon)
    {
        vertex_count = 2 * numberOfPoints;
        ensureCapacityGLLine(vertex_count);
    }
    else
    {
        vertex_count = 2 * (numberOfPoints - 1);
        ensureCapacityGLLine(vertex_count);
    }
    
    V2F_C4B_T2F *point = _bufferGLLine + _bufferCountGLLine;
    V2F_C4B_T2F *cursor = point;
    
    unsigned int i = 0;
    for(; i < numberOfPoints - 1; i++)
    {
        *point = {poli[i], color, Tex2F(0.0, 0.0)};
        *(point + 1) = {poli[i+1], color, Tex2F(0.0, 0.0)};
        point += 2;
    }
    if(closePolygon)
    {
        *point = {poli[i], color, Tex2F(0.0, 0.0)};
        *(point + 1) = {poli[0], color, Tex2F(0.0, 0.0)};
    }
    
    _customCommandGLLine.updateVertexBuffer(cursor, _bufferCountGLLine*sizeof(V2F_C4B_T2F), vertex_count*sizeof(V2F_C4B_T2F));
    _bufferCountGLLine += vertex_count;
    _customCommandGLLine.setVertexDrawInfo(0, _bufferCountGLLine);
}

void DrawNode::drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const Color4F &color)
{
    drawPoly(poli, numberOfPoints, closePolygon, Color4B(color));
}

void DrawNode::drawCircle(const Vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const Color4B &color)
{
    const float coef = 2.0f * (float)M_PI/segments;
    
    Vec2 *vertices = new (std::nothrow) Vec2[segments+2];
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        float j = radius * cosf(rads + angle) * scaleX + center.x;
        float k = radius * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i].x = j;
        vertices[i].y = k;
    }
    if(drawLineToCenter)
    {
        vertices[segments+1].x = center.x;
        vertices[segments+1].y = center.y;
        drawPoly(vertices, segments+2, true, color);
    }
    else
        drawPoly(vertices, segments+1, true, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawNode::drawCircle(const Vec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const Color4F &color)
{
    drawCircle(center, radius, angle, segments, drawLineToCenter, scaleX, scaleY, Color4B(color));
}

void DrawNode::drawCircle(const Vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const Color4B &color)
{
    drawCircle(center, radius, angle, segments, drawLineToCenter, 1.0f, 1.0f, color);
}

void DrawNode::drawCircle(const Vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const Color4F &color)
{
    drawCircle(center, radius, angle, segments, drawLineToCenter, 1.0f, 1.0f, color);
}

void DrawNode::drawQuadBezier(const Vec2 &origin, const Vec2 &control, const Vec2 &destination, unsigned int segments, const Color4B &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0.0f;
    for(unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 2) * origin.x + 2.0f * (1 - t) * t * control.x + t * t * destination.x;
        vertices[i].y = powf(1 - t, 2) * origin.y + 2.0f * (1 - t) * t * control.y + t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;
    
    drawPoly(vertices, segments+1, false, color);

    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawNode::drawQuadBezier(const Vec2 &origin, const Vec2 &control, const Vec2 &destination, unsigned int segments, const Color4F &color)
{
    drawQuadBezier(origin, control, destination, segments, Color4B(color));
}

void DrawNode::drawCubicBezier(const Vec2 &origin, const Vec2 &control1, const Vec2 &control2, const Vec2 &destination, unsigned int segments, const Color4B &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    float t = 0;
    for (unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 3) * origin.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * destination.x;
        vertices[i].y = powf(1 - t, 3) * origin.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;
    
    drawPoly(vertices, segments+1, false, color);

    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawNode::drawCubicBezier(const Vec2 &origin, const Vec2 &control1, const Vec2 &control2, const Vec2 &destination, unsigned int segments, const Color4F &color)
{
    drawCubicBezier(origin, control1, control2, destination, segments, Color4B(color));
}

void DrawNode::drawCardinalSpline(PointArray *config, float tension, unsigned int segments, const Color4B &color)
{
    Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
    if( ! vertices )
        return;
    
    ssize_t p;
    float lt;
    float deltaT = 1.0f / config->count();
    
    for( unsigned int i=0; i < segments+1;i++) {
        
        float dt = (float)i / segments;
        
        // border
        if( dt == 1 ) {
            p = config->count() - 1;
            lt = 1;
        } else {
            p = static_cast<ssize_t>(dt / deltaT);
            lt = (dt - deltaT * (float)p) / deltaT;
        }
        
        // Interpolate
        Vec2 pp0 = config->getControlPointAtIndex(p-1);
        Vec2 pp1 = config->getControlPointAtIndex(p+0);
        Vec2 pp2 = config->getControlPointAtIndex(p+1);
        Vec2 pp3 = config->getControlPointAtIndex(p+2);
        
        Vec2 newPos = ccCardinalSplineAt( pp0, pp1, pp2, pp3, tension, lt);
        vertices[i].x = newPos.x;
        vertices[i].y = newPos.y;
    }
    
    drawPoly(vertices, segments+1, false, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawNode::drawCardinalSpline(PointArray *config, float tension,  unsigned int segments, const Color4F &color)
{
    drawCardinalSpline(config, tension, segments, Color4B(color));
}

void DrawNode::drawCatmullRom(PointArray *points, unsigned int segments, const Color4B &color)
{
    drawCardinalSpline( points, 0.5f, segments, color);
}

void DrawNode::drawCatmullRom(PointArray *points, unsigned int segments, const Color4F &color)
{
    drawCardinalSpline( points, 0.5f, segments, color);
}

void DrawNode::drawDot(const Vec2 &pos, float radius, const Color4B &color)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F a = {Vec2(pos.x - radius, pos.y - radius), color, Tex2F(-1.0, -1.0) };
    V2F_C4B_T2F b = {Vec2(pos.x - radius, pos.y + radius), color, Tex2F(-1.0,  1.0) };
    V2F_C4B_T2F c = {Vec2(pos.x + radius, pos.y + radius), color, Tex2F( 1.0,  1.0) };
    V2F_C4B_T2F d = {Vec2(pos.x + radius, pos.y - radius), color, Tex2F( 1.0, -1.0) };
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    V2F_C4B_T2F_Triangle triangle1 = {a, c, d};
    triangles[0] = triangle0;
    triangles[1] = triangle1;
    
    _customCommand.updateVertexBuffer(triangles, _bufferCount*sizeof(V2F_C4B_T2F), vertex_count*sizeof(V2F_C4B_T2F));
    _bufferCount += vertex_count;
    _dirty = true;
    _customCommand.setVertexDrawInfo(0, _bufferCount);
}

void DrawNode::drawDot(const Vec2 &pos, float radius, const Color4F &color)
{
    drawDot(pos, radius, Color4B(color));
}

void DrawNode::drawRect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2 &p4, const Color4B &color)
{
    drawLine(p1, p2, color);
    drawLine(p2, p3, color);
    drawLine(p3, p4, color);
    drawLine(p4, p1, color);
}

void DrawNode::drawRect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2& p4, const Color4F &color)
{
    drawRect(p1, p2, p3, p4, Color4B(color));
}

void DrawNode::drawSegment(const Vec2 &from, const Vec2 &to, float width, float height, const Color4B &color)
{
	if (width != height)
	{		
		bool horizontal;
		if (from.y == to.y)
		{
			horizontal = true;
		} else
		{
			auto p3 = from - to;
			int angle = int(MATH_RAD_TO_DEG(p3.getAngle()));
			if (angle < 0)
				angle = 360 - (-angle % 360);
			else if (angle > 360)
				angle = angle % 360;
		
			horizontal = angle <= 45 || angle >= 360 - 45 ||
				(angle >= 180 - 45 && angle <= 180 + 45);
		}
		if(!horizontal)
		{
			std::swap(width, height);
		}
	}
	
    unsigned int vertex_count = 6*3;
    ensureCapacity(vertex_count);
    
    Vec2 a = from;
    Vec2 b = to;


    Vec2 n = ((b - a).getPerp()).getNormalized();
    Vec2 t = n.getPerp();

    Vec2 nw = n * height;
    Vec2 tw = t * width;
    Vec2 v0 = b - (nw + tw);
    Vec2 v1 = b + (nw - tw);
    Vec2 v2 = b - nw;
    Vec2 v3 = b + nw;
    Vec2 v4 = a - nw;
    Vec2 v5 = a + nw;
    Vec2 v6 = a - (nw - tw);
    Vec2 v7 = a + (nw + tw);
    
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    
    V2F_C4B_T2F_Triangle triangles0 = {
        {v0, color, v2ToTex2F(-(n + t))},
        {v1, color, v2ToTex2F(n - t)},
        {v2, color, v2ToTex2F(-n)},
    };
    triangles[0] = triangles0;

    V2F_C4B_T2F_Triangle triangles1 = {
        {v3, color, v2ToTex2F(n)},
        {v1, color, v2ToTex2F(n - t)},
        {v2, color, v2ToTex2F(-n)},
    };
    triangles[1] = triangles1;

    V2F_C4B_T2F_Triangle triangles2 = {
        {v3, color, v2ToTex2F(n)},
        {v4, color, v2ToTex2F(-n)},
        {v2, color, v2ToTex2F(-n)},
    };
    triangles[2] = triangles2;

    V2F_C4B_T2F_Triangle triangles3 = {
        {v3, color, v2ToTex2F(n)},
        {v4, color, v2ToTex2F(-n)},
        {v5, color, v2ToTex2F(n) },
    };
    triangles[3] = triangles3;

    V2F_C4B_T2F_Triangle triangles4 = {
        {v6, color, v2ToTex2F(t - n)},
        {v4, color, v2ToTex2F(-n) },
        {v5, color, v2ToTex2F(n)},
    };
    triangles[4] = triangles4;

    V2F_C4B_T2F_Triangle triangles5 = {
        {v6, color, v2ToTex2F(t - n)},
        {v7, color, v2ToTex2F(t + n)},
        {v5, color, v2ToTex2F(n)},
    };
    triangles[5] = triangles5;
    
    _customCommand.updateVertexBuffer(triangles, _bufferCount*sizeof(V2F_C4B_T2F), vertex_count*sizeof(V2F_C4B_T2F));
    _bufferCount += vertex_count;
    _dirty = true;
    _customCommand.setVertexDrawInfo(0, _bufferCount);
}

void DrawNode::drawSegment(const Vec2 &from, const Vec2 &to, float radius, const Color4B &color)
{
    drawSegment(from, to, radius, radius, color);    
}

void DrawNode::drawSegment(const Vec2 &from, const Vec2 &to, float radius, const Color4F &color)
{
    drawSegment(from, to, radius, Color4B(color));
}

void DrawNode::drawPolygon(const Vec2 *verts, int count, const Color4B &fillColor, float borderWidth, const Color4B &borderColor)
{
    CCASSERT(count >= 0, "invalid count value");
    
    bool outline = (borderColor.a > 0 && borderWidth > 0.0f);
    
    auto  triangle_count = outline ? (3*count - 2) : (count - 2);
    auto vertex_count = 3*triangle_count;
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle *cursor = triangles;
    
    for (int i = 0; i < count-2; i++)
    {
        V2F_C4B_T2F_Triangle tmp = {
            {verts[0],   fillColor, v2ToTex2F(Vec2::ZERO)},
            {verts[i+1], fillColor, v2ToTex2F(Vec2::ZERO)},
            {verts[i+2], fillColor, v2ToTex2F(Vec2::ZERO)},
        };
        
        *cursor++ = tmp;
    }
    
    if(outline)
    {
        struct ExtrudeVerts {Vec2 offset, n;};
        struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts) * count);
        
        for (int i = 0; i < count; i++)
        {
            Vec2 v0 = verts[(i-1+count)%count];
            Vec2 v1 = verts[i];
            Vec2 v2 = verts[(i+1)%count];
            
            Vec2 n1 = ((v1 - v0).getPerp()).getNormalized();
            Vec2 n2 = ((v2 - v1).getPerp()).getNormalized();
            
            Vec2 offset = (n1 + n2) * (1.0f / (Vec2::dot(n1, n2) + 1.0f));
            extrude[i] = {offset, n2};
        }
        
        for(int i = 0; i < count; i++)
        {
            int j = (i+1)%count;
            Vec2 v0 = verts[i];
            Vec2 v1 = verts[j];
            
            Vec2 n0 = extrude[i].n;
            
            Vec2 offset0 = extrude[i].offset;
            Vec2 offset1 = extrude[j].offset;
            
            Vec2 inner0 = v0 - offset0 * borderWidth;
            Vec2 inner1 = v1 - offset1 * borderWidth;
            Vec2 outer0 = v0 + offset0 * borderWidth;
            Vec2 outer1 = v1 + offset1 * borderWidth;
            
            V2F_C4B_T2F_Triangle tmp1 = {
                {inner0, borderColor, v2ToTex2F(-n0)},
                {inner1, borderColor, v2ToTex2F(-n0)},
                {outer1, borderColor, v2ToTex2F(n0)}
            };
            *cursor++ = tmp1;
            
            V2F_C4B_T2F_Triangle tmp2 = {
                {inner0, borderColor, v2ToTex2F(-n0)},
                {outer0, borderColor, v2ToTex2F(n0)},
                {outer1, borderColor, v2ToTex2F(n0)}
            };
            *cursor++ = tmp2;
        }
        
        free(extrude);
    }
    
    _customCommand.updateVertexBuffer(triangles, _bufferCount*sizeof(V2F_C4B_T2F), vertex_count*sizeof(V2F_C4B_T2F));
    _bufferCount += vertex_count;
    _customCommand.setVertexDrawInfo(0, _bufferCount);
    _dirty = true;
}

void DrawNode::drawPolygon(const Vec2 *vert, int count, float borderWidth, float borderHeight, const Color4B &borderColor, const Color4B &fillColor)
{
	if (count <= 0)
		return;

	if (count >= 2 && vert[count - 1].equals(vert[0]))
		count--;

	if (fabs(borderWidth - borderHeight) >= FLT_EPSILON)
	{
		if (fillColor.a > 0)
			drawPolygon(vert, count, fillColor, 0.f, borderColor);

		for (int i = 0; i < count; i++)
		{
			int next = i + 1;
			if (next == count)
				next = 0;

			drawSegment(vert[i], vert[next], borderWidth, borderHeight, borderColor);
		}
	} else
	{
		drawPolygon(vert, count, fillColor, borderWidth, borderColor);
	}
}

void DrawNode::drawPolygon(const Vec2 *verts, int count, const Color4F &fillColor, float borderWidth, const Color4F &borderColor)
{
	drawPolygon(verts, count, Color4B(fillColor), borderWidth, Color4B(borderColor));
}

void DrawNode::drawSolidRect(const Vec2 &origin, const Vec2 &destination, const Color4B &color)
{
    Vec2 vertices[] = {
        origin,
        Vec2(destination.x, origin.y),
        destination,
        Vec2(origin.x, destination.y)
    };
    
    drawSolidPoly(vertices, 4, color);
}

void DrawNode::drawSolidRect(const Vec2 &origin, const Vec2 &destination, const Color4F &color)
{
    drawSolidRect(origin, destination, Color4B(color));
}

void DrawNode::drawSolidPoly(const Vec2 *poli, unsigned int numberOfPoints, const Color4B &color)
{
    drawPolygon(poli, numberOfPoints, color, 0.0, Color4B());
}

void DrawNode::drawSolidPoly(const Vec2 *poli, unsigned int numberOfPoints, const Color4F &color)
{
    drawSolidPoly(poli, numberOfPoints, Color4B(color));
}

void DrawNode::drawSolidCircle(const Vec2 &center, float radius, float angle, unsigned int segments, float scaleX, float scaleY, const Color4B &color)
{
    const float coef = 2.0f * (float)M_PI/segments;
    
    Vec2 *vertices = new (std::nothrow) Vec2[segments];
    if( ! vertices )
        return;
    
    for(unsigned int i = 0;i < segments; i++)
    {
        float rads = i*coef;
        float j = radius * cosf(rads + angle) * scaleX + center.x;
        float k = radius * sinf(rads + angle) * scaleY + center.y;
        
        vertices[i].x = j;
        vertices[i].y = k;
    }
    
    drawSolidPoly(vertices, segments, color);
    
    CC_SAFE_DELETE_ARRAY(vertices);
}

void DrawNode::drawSolidCircle(const Vec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY, const Color4F &color)
{
    drawSolidCircle(center, radius, angle, segments, scaleX, scaleY, Color4B(color));
}

void DrawNode::drawSolidCircle(const Vec2 &center, float radius, float angle, unsigned int segments, const Color4B &color)
{
    drawSolidCircle(center, radius, angle, segments, 1.0f, 1.0f, color);
}

void DrawNode::drawSolidCircle( const Vec2& center, float radius, float angle, unsigned int segments, const Color4F& color)
{
    drawSolidCircle(center, radius, angle, segments, 1.0f, 1.0f, Color4B(color));
}

void DrawNode::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Color4B &color)
{
    unsigned int vertex_count = 3;
    ensureCapacity(vertex_count);

    V2F_C4B_T2F a = {p1, color, Tex2F(0.0, 0.0) };
    V2F_C4B_T2F b = {p2, color, Tex2F(0.0,  0.0) };
    V2F_C4B_T2F c = {p3, color, Tex2F(0.0,  0.0) };

    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount);
    V2F_C4B_T2F_Triangle triangle = {a, b, c};
    triangles[0] = triangle;

    _customCommand.updateVertexBuffer(triangles, _bufferCount*sizeof(V2F_C4B_T2F), vertex_count*sizeof(V2F_C4B_T2F));
    _bufferCount += vertex_count;
    _dirty = true;
    _customCommand.setVertexDrawInfo(0, _bufferCount);
}

void DrawNode::drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Color4F &color)
{
   drawTriangle(p1, p2, p3, Color4B(color));
}

void DrawNode::clear()
{
    _bufferCount = 0;
    _dirty = true;
    _bufferCountGLLine = 0;
    _dirtyGLLine = true;
    _bufferCountGLPoint = 0;
    _dirtyGLPoint = true;
    _lineWidth = _defaultLineWidth;
}

const BlendFunc& DrawNode::getBlendFunc() const
{
    return _blendFunc;
}

void DrawNode::setBlendFunc(const BlendFunc &blendFunc)
{
    _blendFunc = blendFunc;
}

void DrawNode::setLineWidth(float lineWidth)
{
    _lineWidth = lineWidth;
}

float DrawNode::getLineWidth()
{
    return this->_lineWidth;
}

int DrawNode::getCapacity(BufferType bufferType) const
{
    switch (bufferType) {
        case TRIANGLES:
            return _bufferCapacity;
            
        case POINTS:
            return _bufferCapacityGLPoint;
            
        case LINES:
            return _bufferCapacityGLLine;
    }
    
    return 0;
}


int *DrawNode::getCapacityPtr(BufferType bufferType)
{
    switch (bufferType) {
        case TRIANGLES:
            return &_bufferCapacity;
            
        case POINTS:
            return &_bufferCapacityGLPoint;
            
        case LINES:
            return &_bufferCapacityGLLine;
    }
    
    return nullptr;
}

V2F_C4B_T2F **DrawNode::getBufferPtr(DrawNode::BufferType bufferType)
{
    switch (bufferType) {
        case TRIANGLES:
            return &_buffer;
            
        case POINTS:
            return &_bufferGLPoint;
            
        case LINES:
            return &_bufferGLLine;
    }
    
    return nullptr;
}

CustomCommand *DrawNode::getCommandPtr(DrawNode::BufferType bufferType)
{
    switch (bufferType) {
        case TRIANGLES:
            return &_customCommand;
            
        case POINTS:
            return &_customCommandGLPoint;
            
        case LINES:
            return &_customCommandGLLine;
    }
    
    return nullptr;
}

int DrawNode::getSize(BufferType bufferType) const
{
    switch (bufferType) {
        case TRIANGLES:
            return _bufferCount;
            
        case POINTS:
            return _bufferCountGLPoint;
            
        case LINES:
            return _bufferCountGLLine;
    }
    
    return 0;
}

int *DrawNode::getSizePtr(DrawNode::BufferType bufferType)
{
    switch (bufferType) {
        case TRIANGLES:
            return &_bufferCount;
            
        case POINTS:
            return &_bufferCountGLPoint;
            
        case LINES:
            return &_bufferCountGLLine;
    }
    
    return nullptr;
}

void DrawNode::reserve(int capacity, BufferType bufferType)
{
    if (capacity < 0) {
        return;
    }
    auto ptr = getCapacityPtr(bufferType);
    if (!ptr) {
        return;
    }
    if (capacity <= *ptr) {
        return;
    }
   
    *ptr = capacity;
    auto &buffer = *getBufferPtr(bufferType);
    auto &command = *getCommandPtr(bufferType);
    size_t newSize = size_t(capacity) * sizeof(V2F_C4B_T2F);
    buffer = (V2F_C4B_T2F*)realloc(buffer, newSize);
    if (capacity > 0) {
        command.createVertexBuffer(sizeof(V2F_C4B_T2F), capacity, CustomCommand::BufferUsage::STATIC);
        command.updateVertexBuffer(buffer, newSize);
    } else {
        command.setVertexBuffer(nullptr);
    }
}

void DrawNode::shrink(BufferType bufferType)
{
    int size = getSize(bufferType);
    if (getCapacity(bufferType) == size)
        return;
    
    *getCapacityPtr(bufferType) = 0;
    reserve(size, bufferType);
    
    if (size > 0) {
        auto &command = *getCommandPtr(bufferType);
        auto &buffer = *getBufferPtr(bufferType);
        command.updateVertexBuffer(buffer, size, size_t(size) * sizeof(V2F_C4B_T2F));
    }
}

void DrawNode::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    if (_isolated)
    {
        //ignore `parentTransform` from parent
        Node::visit(renderer, Mat4::IDENTITY, parentFlags);
    }
    else
    {
        Node::visit(renderer, parentTransform, parentFlags);
    }
}

NS_CC_END
