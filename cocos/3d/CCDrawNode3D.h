/****************************************************************************
 Copyright (c) 2014-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __DRAW_NODE_3D_H__
#define __DRAW_NODE_3D_H__

#include "2d/CCNode.h"
#include "3d/CCAABB.h"
#include "3d/CCOBB.h"

NS_CC_BEGIN

/**
 * Copy DrawNode for 3D geometry drawing.
 */
class DrawNode3D: public Node
{
public:
    /** creates and initialize a DrawNode3D node */
    static DrawNode3D* create();
    
    /**
     * Draw 3D Line
     */
    void drawLine(const Vec3 &from, const Vec3 &to, const Color4B &color);
    
    /**
    * Draw 3D cube
    * @param point to a vertex array who has 8 element.
    *        vertices[0]:Left-top-front,
    *        vertices[1]:Left-bottom-front,
    *        vertices[2]:Right-bottom-front,
    *        vertices[3]:Right-top-front,
    *        vertices[4]:Right-top-back,
    *        vertices[5]:Right-bottom-back,
    *        vertices[6]:Left-bottom-back,
    *        vertices[7]:Left-top-back.
    * @param color
    */
    void drawCube(const Vec3 &from, const Vec3 &to, const Color4B &color);
    void drawCube(const AABB &aabb, const Color4B &color);
    void drawCube(const OBB &obb, const Color4B &color);
    void drawCube(Vec3* vertices, const Color4B &color);
    
    /** Clear the geometry in the node's buffer. */
    void clear();
    
    /**
    * @js NA
    * @lua NA
    */
    const BlendFunc& getBlendFunc() const;
    
    /**
    * @code
    * When this function bound into js or lua,the parameter will be changed
    * In js: var setBlendFunc(var src, var dst)
    * @endcode
    * @lua NA
    */
    void setBlendFunc(const BlendFunc &blendFunc);

    
    // Overrides
    virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
    
CC_CONSTRUCTOR_ACCESS:
    DrawNode3D();
    virtual ~DrawNode3D();
    virtual bool init() override;

protected:
    void updateCommand(const Mat4& transform);

    void onBeforeDraw();
    void onAfterDraw();

    struct V3F_C4B
    {
        Vec3     vertices;
        Color4B  colors;
    };
    void ensureCapacity(int count);


    BlendFunc   _blendFunc;
    CustomCommand _customCommand;
    backend::ProgramState* _programStateLine                    = nullptr;
    backend::DepthStencilDescriptor *_depthstencilDescriptor    = nullptr;
    backend::UniformLocation _locMVPMatrix;
    std::vector<V3F_C4B> _bufferLines;
    size_t _oldLineCount = 0;
    size_t _currentLineCount = 0;
    

private:
    CC_DISALLOW_COPY_AND_ASSIGN(DrawNode3D);

    bool _rendererDepthTestEnabled  = true;
};

NS_CC_END

#endif // __DRAW_NODE_3D_H__
