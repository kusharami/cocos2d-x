/****************************************************************************
 Copyright (c) 2018-2019 Xiamen Yaji Software Co., Ltd.

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
 
#include "ProgramCache.h"
#include "Device.h"
#include "base/CCConfiguration.h"

CC_BACKEND_BEGIN

namespace
{
    std::string getShaderMacrosForLight()
    {
        char def[256];
        auto conf = Configuration::getInstance();

        snprintf(def, sizeof(def) - 1, "\n#define MAX_DIRECTIONAL_LIGHT_NUM %d \n"
            "\n#define MAX_POINT_LIGHT_NUM %d \n"
            "\n#define MAX_SPOT_LIGHT_NUM %d \n",
            conf->getMaxSupportDirLightInShader(),
            conf->getMaxSupportPointLightInShader(),
            conf->getMaxSupportSpotLightInShader());

        return std::string(def);
    }
}

ProgramCache* ProgramCache::_sharedProgramCache = nullptr;

ProgramCache* ProgramCache::getInstance()
{
    if(!_sharedProgramCache)
    {
        _sharedProgramCache = new (std::nothrow) ProgramCache();
        if(!_sharedProgramCache->init())
        {
            CC_SAFE_RELEASE(_sharedProgramCache);
        }
    }
    return _sharedProgramCache;
}

void ProgramCache::destroyInstance()
{
    CC_SAFE_RELEASE_NULL(_sharedProgramCache);
}

ProgramCache::~ProgramCache()
{
    _cachedPrograms.clear();
    removeCustomPrograms();
    CCLOGINFO("deallocing ProgramCache: %p", this);
    ShaderCache::destroyInstance();
}

bool ProgramCache::init()
{
    addProgram(ProgramType::POSITION_TEXTURE_COLOR);
    addProgram(ProgramType::ETC1);
    addProgram(ProgramType::LABEL_DISTANCE_NORMAL);
    addProgram(ProgramType::LABEL_NORMAL);
    addProgram(ProgramType::LABLE_OUTLINE);
    addProgram(ProgramType::LABLE_DISTANCEFIELD_GLOW);
    addProgram(ProgramType::POSITION_COLOR_LENGTH_TEXTURE);
    addProgram(ProgramType::POSITION_COLOR_TEXTURE_AS_POINTSIZE);
    addProgram(ProgramType::POSITION_COLOR);
    addProgram(ProgramType::POSITION);
    addProgram(ProgramType::LAYER_RADIA_GRADIENT);
    addProgram(ProgramType::POSITION_TEXTURE);
    addProgram(ProgramType::POSITION_TEXTURE_COLOR_ALPHA_TEST);
    addProgram(ProgramType::POSITION_UCOLOR);
    addProgram(ProgramType::ETC1_GRAY);
    addProgram(ProgramType::GRAY_SCALE);
    addProgram(ProgramType::LINE_COLOR_3D);
    addProgram(ProgramType::CAMERA_CLEAR);
    addProgram(ProgramType::SKYBOX_3D);
    addProgram(ProgramType::SKINPOSITION_TEXTURE_3D);
    addProgram(ProgramType::SKINPOSITION_TEXTURE_3D_ALPHA_TEST);
    addProgram(ProgramType::SKINPOSITION_NORMAL_TEXTURE_3D);
    addProgram(ProgramType::POSITION_NORMAL_TEXTURE_3D);
    addProgram(ProgramType::POSITION_TEXTURE_3D);
    addProgram(ProgramType::POSITION_TEXTURE_3D_ALPHA_TEST);
    addProgram(ProgramType::POSITION_3D);
    addProgram(ProgramType::POSITION_NORMAL_3D);
    addProgram(ProgramType::POSITION_BUMPEDNORMAL_TEXTURE_3D);
    addProgram(ProgramType::SKINPOSITION_BUMPEDNORMAL_TEXTURE_3D);
    addProgram(ProgramType::TERRAIN_3D);
    addProgram(ProgramType::PARTICLE_TEXTURE_3D);
    addProgram(ProgramType::PARTICLE_COLOR_3D);
    return true;
}

void ProgramCache::addProgram(ProgramType type)
{
    Program* program = nullptr;
    switch (type) {
        case ProgramType::POSITION_TEXTURE_COLOR:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, positionTextureColor_frag);
            break;
        case ProgramType::ETC1:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, etc1_frag);
            break;
        case ProgramType::LABEL_DISTANCE_NORMAL:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, label_distanceNormal_frag);
            break;
        case ProgramType::LABEL_NORMAL:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, label_normal_frag);
            break;
        case ProgramType::LABLE_OUTLINE:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, labelOutline_frag);
            break;
        case ProgramType::LABLE_DISTANCEFIELD_GLOW:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, labelDistanceFieldGlow_frag);
            break;
        case ProgramType::POSITION_COLOR_LENGTH_TEXTURE:
            program = backend::Device::getInstance()->newProgram(positionColorLengthTexture_vert, positionColorLengthTexture_frag);
            break;
        case ProgramType::POSITION_COLOR_TEXTURE_AS_POINTSIZE:
            program = backend::Device::getInstance()->newProgram(positionColorTextureAsPointsize_vert, positionColor_frag);
            break;
        case ProgramType::POSITION_COLOR:
            program = backend::Device::getInstance()->newProgram(positionColor_vert, positionColor_frag);
            break;
        case ProgramType::POSITION:
            program = backend::Device::getInstance()->newProgram(position_vert, positionColor_frag);
            break;
        case ProgramType::LAYER_RADIA_GRADIENT:
            program = backend::Device::getInstance()->newProgram(position_vert, layer_radialGradient_frag);
            break;
        case ProgramType::POSITION_TEXTURE:
            program = backend::Device::getInstance()->newProgram(positionTexture_vert, positionTexture_frag);
            break;
        case ProgramType::POSITION_TEXTURE_COLOR_ALPHA_TEST:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, positionTextureColorAlphaTest_frag);
            break;
        case ProgramType::POSITION_UCOLOR:
            program = backend::Device::getInstance()->newProgram(positionUColor_vert, positionUColor_frag);
            break;
        case ProgramType::ETC1_GRAY:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, etc1Gray_frag);
            break;
        case ProgramType::GRAY_SCALE:
            program = backend::Device::getInstance()->newProgram(positionTextureColor_vert, grayScale_frag);
            break;
        case ProgramType::LINE_COLOR_3D:
            program = backend::Device::getInstance()->newProgram(lineColor3D_vert, lineColor3D_frag);
            break;
        case ProgramType::CAMERA_CLEAR:
            program = backend::Device::getInstance()->newProgram(cameraClear_vert, cameraClear_frag);
            break;
        case ProgramType::SKYBOX_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_skybox_vert, CC3D_skybox_frag);
            break;
        case ProgramType::SKINPOSITION_TEXTURE_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_skinPositionTexture_vert, CC3D_colorTexture_frag);
            break;
        case ProgramType::SKINPOSITION_TEXTURE_3D_ALPHA_TEST:
            program = backend::Device::getInstance()->newProgram(CC3D_skinPositionTexture_vert, CC3D_colorTextureAlphaTest_frag);
            break;
        case ProgramType::SKINPOSITION_NORMAL_TEXTURE_3D:
            {
                std::string def = getShaderMacrosForLight();
                program = backend::Device::getInstance()->newProgram(def + CC3D_skinPositionNormalTexture_vert, def + CC3D_colorNormalTexture_frag);
            }
            break;
        case ProgramType::POSITION_NORMAL_TEXTURE_3D:
            {
                std::string def = getShaderMacrosForLight();
                program = backend::Device::getInstance()->newProgram(def + CC3D_positionNormalTexture_vert, def + CC3D_colorNormalTexture_frag);
            }
            break;
        case ProgramType::POSITION_TEXTURE_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_positionTexture_vert, CC3D_colorTexture_frag);
            break;
        case ProgramType::POSITION_TEXTURE_3D_ALPHA_TEST:
            program = backend::Device::getInstance()->newProgram(CC3D_positionTexture_vert, CC3D_colorTextureAlphaTest_frag);
            break;
        case ProgramType::POSITION_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_positionTexture_vert, CC3D_color_frag);
            break;
        case ProgramType::POSITION_NORMAL_3D:
            {
                std::string def = getShaderMacrosForLight();
                program = backend::Device::getInstance()->newProgram(def + CC3D_positionNormalTexture_vert, def + CC3D_colorNormal_frag);
            }
            break;
        case ProgramType::POSITION_BUMPEDNORMAL_TEXTURE_3D:
            {
                std::string def = getShaderMacrosForLight();
                std::string normalMapDef = "\n#define USE_NORMAL_MAPPING 1 \n";
                program = backend::Device::getInstance()->newProgram(def + normalMapDef + CC3D_positionNormalTexture_vert, def + normalMapDef + CC3D_colorNormalTexture_frag);
            }
            break;
        case ProgramType::SKINPOSITION_BUMPEDNORMAL_TEXTURE_3D:
            {
                std::string def = getShaderMacrosForLight();
                std::string normalMapDef = "\n#define USE_NORMAL_MAPPING 1 \n";
                program = backend::Device::getInstance()->newProgram(def + normalMapDef + CC3D_skinPositionNormalTexture_vert, def + normalMapDef + CC3D_colorNormalTexture_frag);
            }
            break;
        case ProgramType::TERRAIN_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_terrain_vert, CC3D_terrain_frag);
            break;
        case ProgramType::PARTICLE_TEXTURE_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_particle_vert, CC3D_particleTexture_frag);
            break;
        case ProgramType::PARTICLE_COLOR_3D:
            program = backend::Device::getInstance()->newProgram(CC3D_particle_vert, CC3D_particleColor_frag);
            break;
        default:
            CCASSERT(false, "Not built-in program type.");
            break;
    }
    program->setProgramType(type);
    _cachedPrograms.insert(type, program);
    program->release();
}

backend::Program* ProgramCache::getBuiltinProgram(ProgramType type) const
{
    const auto& iter = _cachedPrograms.find(type);
    if (_cachedPrograms.end() != iter)
    {
        return iter->second;
    }
    return nullptr;
}

void ProgramCache::removeCustomProgram(const std::string& key)
{
    _cachedCustomPrograms.erase(key);
}

void ProgramCache::removeUnusedCustomPrograms()
{
    for (auto iter = _cachedCustomPrograms.cbegin(); iter != _cachedCustomPrograms.cend();)
    {
        auto program = iter->second;
        if (program->getReferenceCount() == 1)
        {
            iter = _cachedCustomPrograms.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void ProgramCache::removeCustomPrograms()
{
    _cachedCustomPrograms.clear();
}

void ProgramCache::addCustomProgram(const std::string &key, backend::Program *program)
{
    _cachedCustomPrograms.insert(key, program);
}

backend::Program* ProgramCache::getCustomProgram(const std::string &key) const
{
    const auto& iter = ProgramCache::_cachedCustomPrograms.find(key);
    if (_cachedCustomPrograms.end() != iter)
    {
        return iter->second;
    }
    
    return nullptr;
}

ProgramCache::ProgramCache()
{
    //
}

CC_BACKEND_END
