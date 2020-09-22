include(libcocos2dx.pri)

QT += core gui widgets network

CONFIG += c++11 warn_off
DESTDIR = $$COCOS2DX_LIB
TARGET = cocos2dx
TEMPLATE = lib
CONFIG += staticlib

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
} else {
    DEFINES += NDEBUG
}

!isEmpty(DEBUG_COCOS) {
    emscripten:DEFINES += QT_FORCE_ASSERTS
}

msvc {
    QMAKE_CXXFLAGS_WARN_OFF -= -W0
    QMAKE_CXXFLAGS += -W3 /wd4251 /wd4573
    DEFINES += _CRT_SECURE_NO_WARNINGS
    DEFINES += _SCL_SECURE_NO_WARNINGS
    CONFIG += no_batch
    QMAKE_CXXFLAGS += /wd4005 /wd4244 /bigobj
} else {
    clang|gcc {
        DEFINES += USE_FILE32API
    }
}


SOURCES +=\
    src/QtCocosContext.cpp \
    src/QtCocosHelper.cpp \
    src/QtCocosExtension.cpp \
    src/QtCocosWindow.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCAction.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionCamera.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionCatmullRom.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionEase.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionGrid.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionGrid3D.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionInstant.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionInterval.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionManager.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionPageTurn3D.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionProgressTimer.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionTiledGrid.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCActionTween.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCAnimation.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCAnimationCache.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCAtlasNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCAutoPolygon.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCCamera.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCCameraBackgroundBrush.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCClippingNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCClippingRectangleNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCComponent.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCComponentContainer.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCDrawNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFastTMXLayer.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFastTMXTiledMap.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFontAtlas.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFontAtlasCache.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFontCharMap.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFontFNT.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCFontFreeType.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCGrid.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCLabel.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCLabelAtlas.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCLabelTextFormatter.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCLayer.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCLight.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCMenu.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCMenuItem.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCMotionStreak.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCNodeGrid.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCParallaxNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCParticleBatchNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCParticleExamples.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCParticleSystem.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCParticleSystemQuad.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCProgressTimer.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCProtectedNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCRenderTexture.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCScene.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCSprite.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCSpriteBatchNode.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCSpriteFrame.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCSpriteFrameCache.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTextFieldTTF.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTileMapAtlas.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTMXLayer.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTMXObjectGroup.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTMXTiledMap.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTMXXMLParser.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTransition.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTransitionPageTurn.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTransitionProgress.cpp \
    $$COCOS2DX_PATH/cocos/2d/CCTweenFunction.cpp \
    $$COCOS2DX_PATH/cocos/3d/CC3DProgramInfo.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCAABB.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCAnimate3D.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCAnimation3D.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCAttachNode.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCBillBoard.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCBundle3D.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCBundle3DData.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCBundleReader.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCFrustum.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCMesh.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCMeshSkin.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCMeshVertexIndexData.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCMotionStreak3D.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCOBB.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCObjLoader.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCPlane.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCRay.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCSkeleton3D.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCSkybox.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCSprite3D.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCSprite3DMaterial.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCTerrain.cpp \
    $$COCOS2DX_PATH/cocos/3d/CCVertexAttribBinding.cpp \
    $$COCOS2DX_PATH/cocos/base/atitc.cpp \
    $$COCOS2DX_PATH/cocos/base/base64.cpp \
    $$COCOS2DX_PATH/cocos/base/CCAsyncTaskPool.cpp \
    $$COCOS2DX_PATH/cocos/base/CCAutoreleasePool.cpp \
    $$COCOS2DX_PATH/cocos/base/ccCArray.cpp \
    $$COCOS2DX_PATH/cocos/base/CCConfiguration.cpp \
    $$COCOS2DX_PATH/cocos/base/CCConsole.cpp \
    $$COCOS2DX_PATH/cocos/base/CCData.cpp \
    $$COCOS2DX_PATH/cocos/base/CCDirector.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEvent.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventAcceleration.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventController.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventCustom.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventDispatcher.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventFocus.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventKeyboard.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListener.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerAcceleration.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerCustom.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerFocus.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerKeyboard.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerMouse.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerTouch.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventMouse.cpp \
    $$COCOS2DX_PATH/cocos/base/CCEventTouch.cpp \
    $$COCOS2DX_PATH/cocos/base/ccFPSImages.c \
    $$COCOS2DX_PATH/cocos/base/CCIMEDispatcher.cpp \
    $$COCOS2DX_PATH/cocos/base/CCNinePatchImageParser.cpp \
    $$COCOS2DX_PATH/cocos/base/CCNS.cpp \
    $$COCOS2DX_PATH/cocos/base/CCProfiling.cpp \
    $$COCOS2DX_PATH/cocos/base/CCProperties.cpp \
    $$COCOS2DX_PATH/cocos/base/ccRandom.cpp \
    $$COCOS2DX_PATH/cocos/base/CCRef.cpp \
    $$COCOS2DX_PATH/cocos/base/CCScheduler.cpp \
    $$COCOS2DX_PATH/cocos/base/CCScriptSupport.cpp \
    $$COCOS2DX_PATH/cocos/base/CCStencilStateManager.cpp \
    $$COCOS2DX_PATH/cocos/base/CCTouch.cpp \
    $$COCOS2DX_PATH/cocos/base/ccTypes.cpp \
    $$COCOS2DX_PATH/cocos/base/ccUTF8.cpp \
    $$COCOS2DX_PATH/cocos/base/ccUtils.cpp \
    $$COCOS2DX_PATH/cocos/base/CCValue.cpp \
    $$COCOS2DX_PATH/cocos/base/etc1.cpp \
    $$COCOS2DX_PATH/cocos/base/ObjectFactory.cpp \
    $$COCOS2DX_PATH/cocos/base/pvr.cpp \
    $$COCOS2DX_PATH/cocos/base/s3tc.cpp \
    $$COCOS2DX_PATH/cocos/base/TGAlib.cpp \
    $$COCOS2DX_PATH/cocos/base/ZipUtils.cpp \
    $$COCOS2DX_PATH/cocos/cocos2d.cpp \
    $$COCOS2DX_PATH/cocos/editor-support/cocostudio/CocosStudioExtension.cpp \
    $$COCOS2DX_PATH/cocos/math/CCAffineTransform.cpp \
    $$COCOS2DX_PATH/cocos/math/CCGeometry.cpp \
    $$COCOS2DX_PATH/cocos/math/CCVertex.cpp \
    $$COCOS2DX_PATH/cocos/math/Mat4.cpp \
    $$COCOS2DX_PATH/cocos/math/MathUtil.cpp \
    $$COCOS2DX_PATH/cocos/math/Quaternion.cpp \
    $$COCOS2DX_PATH/cocos/math/TransformUtils.cpp \
    $$COCOS2DX_PATH/cocos/math/Vec2.cpp \
    $$COCOS2DX_PATH/cocos/math/Vec3.cpp \
    $$COCOS2DX_PATH/cocos/math/Vec4.cpp \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMesh.cpp \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshAgent.cpp \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshDebugDraw.cpp \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshObstacle.cpp \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshUtils.cpp \
    $$COCOS2DX_PATH/cocos/network/CCDownloader.cpp \
    $$COCOS2DX_PATH/cocos/network/CCDownloader-qt.cpp \
    $$COCOS2DX_PATH/cocos/platform/CCFileUtils.cpp \
    $$COCOS2DX_PATH/cocos/platform/CCGLView.cpp \
    $$COCOS2DX_PATH/cocos/platform/CCSAXParser.cpp \
    $$COCOS2DX_PATH/cocos/platform/qt/CCImage-qt.cpp \
    $$COCOS2DX_PATH/cocos/platform/qt/CCGLViewImpl-qt.cpp \
    $$COCOS2DX_PATH/cocos/platform/qt/CCApplication-qt.cpp \
    $$COCOS2DX_PATH/cocos/platform/qt/CCCommon-qt.cpp \
    $$COCOS2DX_PATH/cocos/platform/qt/CCDevice-qt.cpp \
    $$COCOS2DX_PATH/cocos/platform/qt/CCFileUtils-qt.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/CommandBuffer.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/DepthStencilState.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/Device.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/BufferGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/CommandBufferGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/DepthStencilStateGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/DeviceGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/DeviceInfoGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/ProgramGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/RenderPipelineGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/ShaderModuleGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/TextureGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/UtilsGL.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/Program.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/ProgramCache.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/ProgramState.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/RenderPassDescriptor.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/ShaderCache.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/ShaderModule.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/Texture.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/Types.cpp \
    $$COCOS2DX_PATH/cocos/renderer/backend/VertexLayout.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCCallbackCommand.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCCustomCommand.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCGroupCommand.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCMaterial.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCMeshCommand.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCPass.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCQuadCommand.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderCommand.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderer.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderState.cpp \
    $$COCOS2DX_PATH/cocos/renderer/ccShaders.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTechnique.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTexture2D.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureAtlas.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureCache.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureCube.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureUtils.cpp \
    $$COCOS2DX_PATH/cocos/renderer/CCTrianglesCommand.cpp \
    $$COCOS2DX_PATH/cocos/ui/CocosGUI.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIAbstractCheckButton.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIButton.cpp \
    $$COCOS2DX_PATH/cocos/ui/UICheckBox.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIEditBox/UIEditBox.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIHBox.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIHelper.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIImageView.cpp \
    $$COCOS2DX_PATH/cocos/ui/UILayout.cpp \
    $$COCOS2DX_PATH/cocos/ui/UILayoutComponent.cpp \
    $$COCOS2DX_PATH/cocos/ui/UILayoutManager.cpp \
    $$COCOS2DX_PATH/cocos/ui/UILayoutParameter.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIListView.cpp \
    $$COCOS2DX_PATH/cocos/ui/UILoadingBar.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIPageView.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIPageViewIndicator.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIRadioButton.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIRelativeBox.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIRichText.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIScale9Sprite.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIScrollView.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIScrollViewBar.cpp \
    $$COCOS2DX_PATH/cocos/ui/UISlider.cpp \
    $$COCOS2DX_PATH/cocos/ui/UITabControl.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIText.cpp \
    $$COCOS2DX_PATH/cocos/ui/UITextAtlas.cpp \
    $$COCOS2DX_PATH/cocos/ui/UITextBMFont.cpp \
    $$COCOS2DX_PATH/cocos/ui/UITextField.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIVBox.cpp \
    $$COCOS2DX_PATH/cocos/ui/UIWidget.cpp \
    $$COCOS2DX_PATH/external/ConvertUTF/ConvertUTF.c \
    $$COCOS2DX_PATH/external/ConvertUTF/ConvertUTFWrapper.cpp \
    $$COCOS2DX_PATH/external/edtaa3func/edtaa3func.cpp \
    $$COCOS2DX_PATH/external/xxhash/xxhash.c \
    $$COCOS2DX_PATH/external/recast/fastlz/fastlz.c \
    $$COCOS2DX_PATH/external/recast/DebugUtils/DebugDraw.cpp \
    $$COCOS2DX_PATH/external/recast/Detour/DetourAlloc.cpp \
    $$COCOS2DX_PATH/external/recast/Detour/DetourCommon.cpp \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourCrowd.cpp \
    $$COCOS2DX_PATH/external/recast/DebugUtils/DetourDebugDraw.cpp \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourLocalBoundary.cpp \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNavMesh.cpp \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNavMeshBuilder.cpp \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNavMeshQuery.cpp \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNode.cpp \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourObstacleAvoidance.cpp \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourPathCorridor.cpp \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourPathQueue.cpp \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourProximityGrid.cpp \
    $$COCOS2DX_PATH/external/recast/DetourTileCache/DetourTileCache.cpp \
    $$COCOS2DX_PATH/external/recast/DetourTileCache/DetourTileCacheBuilder.cpp \
    $$COCOS2DX_PATH/external/clipper/clipper.cpp \
    $$COCOS2DX_PATH/external/poly2tri/common/shapes.cc \
    $$COCOS2DX_PATH/external/poly2tri/sweep/cdt.cc \
    $$COCOS2DX_PATH/external/poly2tri/sweep/sweep.cc \
    $$COCOS2DX_PATH/external/poly2tri/sweep/sweep_context.cc \
    $$COCOS2DX_PATH/external/poly2tri/sweep/advancing_front.cc \
    $$COCOS2DX_PATH/external/unzip/ioapi.cpp \
    $$COCOS2DX_PATH/external/unzip/ioapi_mem.cpp \
    $$COCOS2DX_PATH/external/unzip/unzip.cpp \
    $$COCOS2DX_PATH/external/md5/md5.c \
    $$COCOS2DX_PATH/external/tinyxml2/tinyxml2.cpp
    
HEADERS += \
    src/QtCocosContext.h \
    src/QtCocosHelper.h \
    src/QtCocosExtension.h \
    src/QtCocosWindow.h \
    src/CCObjectHolder.h \
    $$COCOS2DX_PATH/cocos/2d/CCAction.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionCamera.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionCatmullRom.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionEase.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionGrid.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionGrid3D.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionInstant.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionInterval.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionManager.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionPageTurn3D.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionProgressTimer.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionTiledGrid.h \
    $$COCOS2DX_PATH/cocos/2d/CCActionTween.h \
    $$COCOS2DX_PATH/cocos/2d/CCAnimation.h \
    $$COCOS2DX_PATH/cocos/2d/CCAnimationCache.h \
    $$COCOS2DX_PATH/cocos/2d/CCAtlasNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCAutoPolygon.h \
    $$COCOS2DX_PATH/cocos/2d/CCCamera.h \
    $$COCOS2DX_PATH/cocos/2d/CCCameraBackgroundBrush.h \
    $$COCOS2DX_PATH/cocos/2d/CCClippingNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCClippingRectangleNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCComponent.h \
    $$COCOS2DX_PATH/cocos/2d/CCComponentContainer.h \
    $$COCOS2DX_PATH/cocos/2d/CCDrawNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCFastTMXLayer.h \
    $$COCOS2DX_PATH/cocos/2d/CCFastTMXTiledMap.h \
    $$COCOS2DX_PATH/cocos/2d/CCFont.h \
    $$COCOS2DX_PATH/cocos/2d/CCFontAtlas.h \
    $$COCOS2DX_PATH/cocos/2d/CCFontAtlasCache.h \
    $$COCOS2DX_PATH/cocos/2d/CCFontCharMap.h \
    $$COCOS2DX_PATH/cocos/2d/CCFontFNT.h \
    $$COCOS2DX_PATH/cocos/2d/CCFontFreeType.h \
    $$COCOS2DX_PATH/cocos/2d/CCGrid.h \
    $$COCOS2DX_PATH/cocos/2d/CCLabel.h \
    $$COCOS2DX_PATH/cocos/2d/CCLabelAtlas.h \
    $$COCOS2DX_PATH/cocos/2d/CCLabelTextFormatter.h \
    $$COCOS2DX_PATH/cocos/2d/CCLayer.h \
    $$COCOS2DX_PATH/cocos/2d/CCLight.h \
    $$COCOS2DX_PATH/cocos/2d/CCMenu.h \
    $$COCOS2DX_PATH/cocos/2d/CCMenuItem.h \
    $$COCOS2DX_PATH/cocos/2d/CCMotionStreak.h \
    $$COCOS2DX_PATH/cocos/2d/CCNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCNodeGrid.h \
    $$COCOS2DX_PATH/cocos/2d/CCParallaxNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCParticleBatchNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCParticleExamples.h \
    $$COCOS2DX_PATH/cocos/2d/CCParticleSystem.h \
    $$COCOS2DX_PATH/cocos/2d/CCParticleSystemQuad.h \
    $$COCOS2DX_PATH/cocos/2d/CCProgressTimer.h \
    $$COCOS2DX_PATH/cocos/2d/CCProtectedNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCRenderTexture.h \
    $$COCOS2DX_PATH/cocos/2d/CCScene.h \
    $$COCOS2DX_PATH/cocos/2d/CCSprite.h \
    $$COCOS2DX_PATH/cocos/2d/CCSpriteBatchNode.h \
    $$COCOS2DX_PATH/cocos/2d/CCSpriteFrame.h \
    $$COCOS2DX_PATH/cocos/2d/CCSpriteFrameCache.h \
    $$COCOS2DX_PATH/cocos/2d/CCTextFieldTTF.h \
    $$COCOS2DX_PATH/cocos/2d/CCTileMapAtlas.h \
    $$COCOS2DX_PATH/cocos/2d/CCTMXLayer.h \
    $$COCOS2DX_PATH/cocos/2d/CCTMXObjectGroup.h \
    $$COCOS2DX_PATH/cocos/2d/CCTMXTiledMap.h \
    $$COCOS2DX_PATH/cocos/2d/CCTMXXMLParser.h \
    $$COCOS2DX_PATH/cocos/2d/CCTransition.h \
    $$COCOS2DX_PATH/cocos/2d/CCTransitionPageTurn.h \
    $$COCOS2DX_PATH/cocos/2d/CCTransitionProgress.h \
    $$COCOS2DX_PATH/cocos/2d/CCTweenFunction.h \
    $$COCOS2DX_PATH/cocos/3d/CC3DProgramInfo.h \
    $$COCOS2DX_PATH/cocos/3d/CCAABB.h \
    $$COCOS2DX_PATH/cocos/3d/CCAnimate3D.h \
    $$COCOS2DX_PATH/cocos/3d/CCAnimation3D.h \
    $$COCOS2DX_PATH/cocos/3d/CCAnimationCurve.h \
    $$COCOS2DX_PATH/cocos/3d/CCAttachNode.h \
    $$COCOS2DX_PATH/cocos/3d/CCBillBoard.h \
    $$COCOS2DX_PATH/cocos/3d/CCBundle3D.h \
    $$COCOS2DX_PATH/cocos/3d/CCBundle3DData.h \
    $$COCOS2DX_PATH/cocos/3d/CCBundleReader.h \
    $$COCOS2DX_PATH/cocos/3d/CCFrustum.h \
    $$COCOS2DX_PATH/cocos/3d/CCMesh.h \
    $$COCOS2DX_PATH/cocos/3d/CCMeshSkin.h \
    $$COCOS2DX_PATH/cocos/3d/CCMeshVertexIndexData.h \
    $$COCOS2DX_PATH/cocos/3d/CCMotionStreak3D.h \
    $$COCOS2DX_PATH/cocos/3d/CCOBB.h \
    $$COCOS2DX_PATH/cocos/3d/CCObjLoader.h \
    $$COCOS2DX_PATH/cocos/3d/CCPlane.h \
    $$COCOS2DX_PATH/cocos/3d/CCRay.h \
    $$COCOS2DX_PATH/cocos/3d/CCSkeleton3D.h \
    $$COCOS2DX_PATH/cocos/3d/CCSkybox.h \
    $$COCOS2DX_PATH/cocos/3d/CCSprite3D.h \
    $$COCOS2DX_PATH/cocos/3d/CCSprite3DMaterial.h \
    $$COCOS2DX_PATH/cocos/3d/CCTerrain.h \
    $$COCOS2DX_PATH/cocos/3d/CCVertexAttribBinding.h \
    $$COCOS2DX_PATH/cocos/3d/cocos3d.h \
    $$COCOS2DX_PATH/cocos/base/atitc.h \
    $$COCOS2DX_PATH/cocos/base/base64.h \
    $$COCOS2DX_PATH/cocos/base/CCAsyncTaskPool.h \
    $$COCOS2DX_PATH/cocos/base/CCAutoreleasePool.h \
    $$COCOS2DX_PATH/cocos/base/ccCArray.h \
    $$COCOS2DX_PATH/cocos/base/ccConfig.h \
    $$COCOS2DX_PATH/cocos/base/CCConfiguration.h \
    $$COCOS2DX_PATH/cocos/base/CCConsole.h \
    $$COCOS2DX_PATH/cocos/base/ccConstants.h \
    $$COCOS2DX_PATH/cocos/base/CCData.h \
    $$COCOS2DX_PATH/cocos/base/CCDirector.h \
    $$COCOS2DX_PATH/cocos/base/CCEnumClass.h \
    $$COCOS2DX_PATH/cocos/base/CCEvent.h \
    $$COCOS2DX_PATH/cocos/base/CCEventAcceleration.h \
    $$COCOS2DX_PATH/cocos/base/CCEventController.h \
    $$COCOS2DX_PATH/cocos/base/CCEventCustom.h \
    $$COCOS2DX_PATH/cocos/base/CCEventDispatcher.h \
    $$COCOS2DX_PATH/cocos/base/CCEventFocus.h \
    $$COCOS2DX_PATH/cocos/base/CCEventKeyboard.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListener.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerAcceleration.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerController.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerCustom.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerFocus.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerKeyboard.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerMouse.h \
    $$COCOS2DX_PATH/cocos/base/CCEventListenerTouch.h \
    $$COCOS2DX_PATH/cocos/base/CCEventMouse.h \
    $$COCOS2DX_PATH/cocos/base/CCEventTouch.h \
    $$COCOS2DX_PATH/cocos/base/CCEventType.h \
    $$COCOS2DX_PATH/cocos/base/ccFPSImages.h \
    $$COCOS2DX_PATH/cocos/base/CCIMEDelegate.h \
    $$COCOS2DX_PATH/cocos/base/CCIMEDispatcher.h \
    $$COCOS2DX_PATH/cocos/base/ccMacros.h \
    $$COCOS2DX_PATH/cocos/base/CCMap.h \
    $$COCOS2DX_PATH/cocos/base/CCNinePatchImageParser.h \
    $$COCOS2DX_PATH/cocos/base/CCNS.h \
    $$COCOS2DX_PATH/cocos/base/CCProfiling.h \
    $$COCOS2DX_PATH/cocos/base/CCProperties.h \
    $$COCOS2DX_PATH/cocos/base/CCProtocols.h \
    $$COCOS2DX_PATH/cocos/base/ccRandom.h \
    $$COCOS2DX_PATH/cocos/base/CCRef.h \
    $$COCOS2DX_PATH/cocos/base/CCRefPtr.h \
    $$COCOS2DX_PATH/cocos/base/CCScheduler.h \
    $$COCOS2DX_PATH/cocos/base/CCScriptSupport.h \
    $$COCOS2DX_PATH/cocos/base/CCStencilStateManager.h \
    $$COCOS2DX_PATH/cocos/base/CCTouch.h \
    $$COCOS2DX_PATH/cocos/base/ccTypes.h \
    $$COCOS2DX_PATH/cocos/base/CCUserDefault.h \
    $$COCOS2DX_PATH/cocos/base/ccUTF8.h \
    $$COCOS2DX_PATH/cocos/base/ccUtils.h \
    $$COCOS2DX_PATH/cocos/base/CCValue.h \
    $$COCOS2DX_PATH/cocos/base/CCVector.h \
    $$COCOS2DX_PATH/cocos/base/etc1.h \
    $$COCOS2DX_PATH/cocos/base/firePngData.h \
    $$COCOS2DX_PATH/cocos/base/ObjectFactory.h \
    $$COCOS2DX_PATH/cocos/base/pvr.h \
    $$COCOS2DX_PATH/cocos/base/s3tc.h \
    $$COCOS2DX_PATH/cocos/base/TGAlib.h \
    $$COCOS2DX_PATH/cocos/base/uthash.h \
    $$COCOS2DX_PATH/cocos/base/utlist.h \
    $$COCOS2DX_PATH/cocos/base/ZipUtils.h \
    $$COCOS2DX_PATH/cocos/cocos2d.h \
    $$COCOS2DX_PATH/cocos/editor-support/cocostudio/CocosStudioExtension.h \
    $$COCOS2DX_PATH/cocos/math/CCAffineTransform.h \
    $$COCOS2DX_PATH/cocos/math/CCGeometry.h \
    $$COCOS2DX_PATH/cocos/math/CCMath.h \
    $$COCOS2DX_PATH/cocos/math/CCMathBase.h \
    $$COCOS2DX_PATH/cocos/math/CCVertex.h \
    $$COCOS2DX_PATH/cocos/math/Mat4.h \
    $$COCOS2DX_PATH/cocos/math/MathUtil.h \
    $$COCOS2DX_PATH/cocos/math/Quaternion.h \
    $$COCOS2DX_PATH/cocos/math/TransformUtils.h \
    $$COCOS2DX_PATH/cocos/math/Vec2.h \
    $$COCOS2DX_PATH/cocos/math/Vec3.h \
    $$COCOS2DX_PATH/cocos/math/Vec4.h \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMesh.h \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshAgent.h \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshDebugDraw.h \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshObstacle.h \
    $$COCOS2DX_PATH/cocos/navmesh/CCNavMeshUtils.h \
    $$COCOS2DX_PATH/cocos/network/CCDownloader.h \
    $$COCOS2DX_PATH/cocos/network/CCDownloader-qt.h \
    $$COCOS2DX_PATH/cocos/network/CCIDownloaderImpl.h \
    $$COCOS2DX_PATH/cocos/platform/CCApplication.h \
    $$COCOS2DX_PATH/cocos/platform/CCApplicationProtocol.h \
    $$COCOS2DX_PATH/cocos/platform/CCCommon.h \
    $$COCOS2DX_PATH/cocos/platform/CCDevice.h \
    $$COCOS2DX_PATH/cocos/platform/CCFileUtils.h \
    $$COCOS2DX_PATH/cocos/platform/CCGL.h \
    $$COCOS2DX_PATH/cocos/platform/CCGLView.h \
    $$COCOS2DX_PATH/cocos/platform/CCImage.h \
    $$COCOS2DX_PATH/cocos/platform/CCPlatformConfig.h \
    $$COCOS2DX_PATH/cocos/platform/CCPlatformDefine.h \
    $$COCOS2DX_PATH/cocos/platform/CCPlatformMacros.h \
    $$COCOS2DX_PATH/cocos/platform/CCSAXParser.h \
    $$COCOS2DX_PATH/cocos/platform/CCStdC.h \
    $$COCOS2DX_PATH/cocos/platform/qt/CCGL-qt.h \
    $$COCOS2DX_PATH/cocos/platform/qt/CCGLViewImpl-qt.h \
    $$COCOS2DX_PATH/cocos/platform/qt/CCApplication-qt.h \
    $$COCOS2DX_PATH/cocos/platform/qt/CCFileUtils-qt.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Backend.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Buffer.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/CommandBuffer.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/DepthStencilState.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Device.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/DeviceInfo.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Macros.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/BufferGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/CommandBufferGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/DepthStencilStateGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/DeviceGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/DeviceInfoGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/ProgramGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/RenderPipelineGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/ShaderModuleGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/TextureGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/opengl/UtilsGL.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Program.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/ProgramCache.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/ProgramState.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/RenderPassDescriptor.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/RenderPipeline.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/RenderPipelineDescriptor.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/ShaderCache.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/ShaderModule.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Texture.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/Types.h \
    $$COCOS2DX_PATH/cocos/renderer/backend/VertexLayout.h \
    $$COCOS2DX_PATH/cocos/renderer/CCCallbackCommand.h \
    $$COCOS2DX_PATH/cocos/renderer/CCCustomCommand.h \
    $$COCOS2DX_PATH/cocos/renderer/CCGroupCommand.h \
    $$COCOS2DX_PATH/cocos/renderer/CCMaterial.h \
    $$COCOS2DX_PATH/cocos/renderer/CCMeshCommand.h \
    $$COCOS2DX_PATH/cocos/renderer/CCPass.h \
    $$COCOS2DX_PATH/cocos/renderer/CCPipelineDescriptor.h \
    $$COCOS2DX_PATH/cocos/renderer/CCQuadCommand.h \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderCommand.h \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderCommandPool.h \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderer.h \
    $$COCOS2DX_PATH/cocos/renderer/CCRenderState.h \
    $$COCOS2DX_PATH/cocos/renderer/ccShaders.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTechnique.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTexture2D.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureAtlas.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureCache.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureCube.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTextureUtils.h \
    $$COCOS2DX_PATH/cocos/renderer/CCTrianglesCommand.h \
    $$COCOS2DX_PATH/cocos/ui/CocosGUI.h \
    $$COCOS2DX_PATH/cocos/ui/GUIDefine.h \
    $$COCOS2DX_PATH/cocos/ui/GUIExport.h \
    $$COCOS2DX_PATH/cocos/ui/UIAbstractCheckButton.h \
    $$COCOS2DX_PATH/cocos/ui/UIButton.h \
    $$COCOS2DX_PATH/cocos/ui/UICheckBox.h \
    $$COCOS2DX_PATH/cocos/ui/UIEditBox/UIEditBox.h \
    $$COCOS2DX_PATH/cocos/ui/UIEditBox/UIEditBoxImpl.h \
    $$COCOS2DX_PATH/cocos/ui/UIHBox.h \
    $$COCOS2DX_PATH/cocos/ui/UIHelper.h \
    $$COCOS2DX_PATH/cocos/ui/UIImageView.h \
    $$COCOS2DX_PATH/cocos/ui/UILayout.h \
    $$COCOS2DX_PATH/cocos/ui/UILayoutComponent.h \
    $$COCOS2DX_PATH/cocos/ui/UILayoutManager.h \
    $$COCOS2DX_PATH/cocos/ui/UILayoutParameter.h \
    $$COCOS2DX_PATH/cocos/ui/UIListView.h \
    $$COCOS2DX_PATH/cocos/ui/UILoadingBar.h \
    $$COCOS2DX_PATH/cocos/ui/UIPageView.h \
    $$COCOS2DX_PATH/cocos/ui/UIPageViewIndicator.h \
    $$COCOS2DX_PATH/cocos/ui/UIRadioButton.h \
    $$COCOS2DX_PATH/cocos/ui/UIRelativeBox.h \
    $$COCOS2DX_PATH/cocos/ui/UIRichText.h \
    $$COCOS2DX_PATH/cocos/ui/UIScale9Sprite.h \
    $$COCOS2DX_PATH/cocos/ui/UIScrollView.h \
    $$COCOS2DX_PATH/cocos/ui/UIScrollViewBar.h \
    $$COCOS2DX_PATH/cocos/ui/UISlider.h \
    $$COCOS2DX_PATH/cocos/ui/UITabControl.h \
    $$COCOS2DX_PATH/cocos/ui/UIText.h \
    $$COCOS2DX_PATH/cocos/ui/UITextAtlas.h \
    $$COCOS2DX_PATH/cocos/ui/UITextBMFont.h \
    $$COCOS2DX_PATH/cocos/ui/UITextField.h \
    $$COCOS2DX_PATH/cocos/ui/UIVBox.h \
    $$COCOS2DX_PATH/cocos/ui/UIVideoPlayer.h \
    $$COCOS2DX_PATH/cocos/ui/UIWidget.h \
    $$COCOS2DX_PATH/external/ConvertUTF/ConvertUTF.h \
    $$COCOS2DX_PATH/external/edtaa3func/edtaa3func.h \
    $$COCOS2DX_PATH/external/xxhash/xxhash.h \
    $$COCOS2DX_PATH/external/recast/DebugUtils/DebugDraw.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourAlloc.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourAssert.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourCommon.h \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourCrowd.h \
    $$COCOS2DX_PATH/external/recast/DebugUtils/DetourDebugDraw.h \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourLocalBoundary.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourMath.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNavMesh.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNavMeshBuilder.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNavMeshQuery.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourNode.h \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourObstacleAvoidance.h \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourPathCorridor.h \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourPathQueue.h \
    $$COCOS2DX_PATH/external/recast/DetourCrowd/DetourProximityGrid.h \
    $$COCOS2DX_PATH/external/recast/Detour/DetourStatus.h \
    $$COCOS2DX_PATH/external/recast/DetourTileCache/DetourTileCache.h \
    $$COCOS2DX_PATH/external/recast/DetourTileCache/DetourTileCacheBuilder.h \
    $$COCOS2DX_PATH/external/recast/fastlz/fastlz.h \
    $$COCOS2DX_PATH/external/clipper/clipper.hpp \
    $$COCOS2DX_PATH/external/poly2tri/poly2tri.h \
    $$COCOS2DX_PATH/external/poly2tri/common/shapes.h \
    $$COCOS2DX_PATH/external/poly2tri/common/utils.h \
    $$COCOS2DX_PATH/external/poly2tri/sweep/cdt.h \
    $$COCOS2DX_PATH/external/poly2tri/sweep/sweep.h \
    $$COCOS2DX_PATH/external/poly2tri/sweep/sweep_context.h \
    $$COCOS2DX_PATH/external/poly2tri/sweep/advancing_front.h \
    $$COCOS2DX_PATH/external/unzip/crypt.h \
    $$COCOS2DX_PATH/external/unzip/ioapi.h \
    $$COCOS2DX_PATH/external/unzip/ioapi_mem.h \
    $$COCOS2DX_PATH/external/unzip/unzip.h \
    $$COCOS2DX_PATH/external/md5/md5.h \
    $$COCOS2DX_PATH/external/tinyxml2/tinyxml2.h \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_color.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_colorNormal.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_colorNormalTexture.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_colorTexture.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_particle.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_skybox.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_terrain.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/cameraClear.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/etc1.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/etc1_Gray.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/label_distanceFieldGlow.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/label_distanceNormal.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/label_normal.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/label_outline.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/layer_radialGradient.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/lineColor.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionColor.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionColorLengthTexture.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTexture.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTextureColor.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTextureColorAlphaTest.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTextureUColor.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionUColor.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/ui_Gray.frag \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_particle.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_positionNormalTexture.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_positionTexture.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_skybox.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/3D_terrain.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/cameraClear.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/lineColor.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/position.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionColor.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionColorLengthTexture.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionColorTextureAsPointsize.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionNoMVP.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTexture.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTextureColor.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionTextureUColor.vert \
    $$COCOS2DX_PATH/cocos/renderer/shaders/positionUColor.vert


macx {

HEADERS += \
    $$COCOS2DX_PATH/cocos/platform/mac/CCPlatformDefine-mac.h \
    $$COCOS2DX_PATH/cocos/platform/mac/CCStdC-mac.h \
    $$COCOS2DX_PATH/cocos/platform/mac/CCGL-mac.h

}

win32 {

SOURCES += $$COCOS2DX_PATH/cocos/platform/win32/CCStdC-win32.cpp

HEADERS += \
    $$COCOS2DX_PATH/cocos/platform/win32/CCStdC-win32.h \
    $$COCOS2DX_PATH/cocos/platform/win32/CCPlatformDefine-win32.h

}


emscripten {

HEADERS += \
    $$COCOS2DX_PATH/cocos/platform/emscripten/CCGL-emscripten.h \
    $$COCOS2DX_PATH/cocos/platform/emscripten/CCPlatformDefine-emscripten.h \
    $$COCOS2DX_PATH/cocos/platform/emscripten/CCStdC-emscripten.h

}


linux {

HEADERS += \
    $$COCOS2DX_PATH/cocos/platform/linux/CCPlatformDefine-linux.h \
    $$COCOS2DX_PATH/cocos/platform/linux/CCStdC-linux.h

}

include($$COCOS2DX_PATH/external/freetype2/qt/freetype_build.pri)
