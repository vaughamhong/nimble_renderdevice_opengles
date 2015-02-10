//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/renderdevice.h>
#include <nimble/renderdevice/opengles_3_0/texture.h>
#include <nimble/renderdevice/opengles_3_0/vertexbuffer.h>
#include <nimble/renderdevice/opengles_3_0/indexbuffer.h>
#include <nimble/renderdevice/opengles_3_0/framebuffer.h>
#include <memory>

//////////////////////////////////////////////////////////////////////////

#define BUFFER_OFFSET(i) ((void*)(i))

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
RenderDevice::RenderDevice(){
    // reset our internal context
    this->resetRenderContext();
}
//! Destructor
RenderDevice::~RenderDevice(){
    assert(m_context.m_pRenderDeviceDelegate != 0);
    m_context.m_pRenderDeviceDelegate = 0;
}

//////////////////////////////////////////////////////////////////////////

//! set delegate
void RenderDevice::setDelegate(renderdevice::IRenderDevice::Delegate* pRenderDeviceDelegate){
    m_context.m_pRenderDeviceDelegate = pRenderDeviceDelegate;
}

//////////////////////////////////////////////////////////////////////////

//! returns the width of the frame
//! \return the width of the frame
unsigned int RenderDevice::getFrameWidth(){
    return m_context.m_pFrameBuffer->getWidth();
}
//! returns the height of the frame
//! \return the height of the frame
unsigned int RenderDevice::getFrameHeight(){
    return m_context.m_pFrameBuffer->getHeight();
}

//////////////////////////////////////////////////////////////////////////

//! clears our buffers
//! \param buffer our buffer mask
void RenderDevice::clearBuffer(unsigned int bufferMask){
    unsigned int glBufferMask = 0;
    if(bufferMask & renderdevice::kFrameBufferColor0Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor1Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor2Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor3Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor4Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor5Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor6Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferColor7Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferDepthBit){glBufferMask |= GL_DEPTH_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferStencilBit){glBufferMask |= GL_STENCIL_BUFFER_BIT;}
    if(bufferMask & renderdevice::kFrameBufferAccumulationBit){assert(false);}
    GLDEBUG(glClear(glBufferMask));
}
//! sets our clear color
//! \param r red component
//! \param g green component
//! \param b blue component
void RenderDevice::setClearColor(float r, float g, float b, float a){
    GLDEBUG(glClearColor(r, g, b, a));
}

//////////////////////////////////////////////////////////////////////////

//! reset our context
void RenderDevice::resetRenderContext(){
    // frame buffer state
    m_context.m_pFrameBuffer = 0;
    
    // vertex / index buffer states
    m_context.m_pVertexBuffer = 0;
    m_context.m_pIndexBuffer = 0;
    
    // texture states
    memset(m_context.m_textures, 0, sizeof(renderdevice::ITexture*) * renderdevice::kMaxTextureUnits);
    
    // shader states
    m_context.m_pShaderProgram = 0;
    m_context.m_pVertexShader = 0;
    m_context.m_pPixelShader = 0;
    
    // flags for shader program matrix params
#define SHADERPARAM_TUPLE(NAME, SHADERNAME) m_context.m_shaderHas##NAME##ShaderParam = false;
    SHADERPARAM_TUPLESET
#undef SHADERPARAM_TUPLE
    
    m_context.m_viewport = math::Rect2i();
    m_context.m_currentTextureUnitIndex = -1;
    
    // render states
    for(int i = 0; i < renderdevice::kMaxRenderStates; i++){
        m_context.m_renderStates[i] = false;
    }
    this->enableState(renderdevice::kRenderStateColorWrite);
    this->enableState(renderdevice::kRenderStateDepthWrite);
    this->disableState(renderdevice::kRenderStateStencilWrite);
    this->setDepthFunc(renderdevice::kRenderDepthFuncLessEqual);
    this->setStencilFunc(renderdevice::kRenderStencilFuncAlways, 0, 0xff);
    this->setStencilOp(renderdevice::kRenderStencilOpKeep, renderdevice::kRenderStencilOpKeep, renderdevice::kRenderStencilOpKeep);
    this->setStencilMask(0xff);
    this->setCullFace(renderdevice::kRenderCullFaceBack);
    this->setFrontFaceOrientation(renderdevice::kRenderOrientationCounterClockwise);
    this->setMatrixMode(renderdevice::kRenderMatrixModeModel);
    
    // flags for matrix state has changed
#define SHADERPARAM_TUPLE(NAME, SHADERNAME) m_context.m_final##NAME##MatrixChanged = false;
    SHADERPARAM_TUPLESET
#undef SHADERPARAM_TUPLE
    
    // model matrix state
    m_context.m_modelMatrixStack.clear();
    m_context.m_finalModelMatrixStack.clear();
    m_context.m_modelMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_topModelMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalModelMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalNormalMatrix = nimble::math::Matrix4x4f::identity();
    
    // view matrix state
    m_context.m_viewMatrixStack.clear();
    m_context.m_finalViewMatrixStack.clear();
    m_context.m_viewMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_topViewMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalViewMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalInvViewMatrix = nimble::math::Matrix4x4f::identity();
    
    // projection matrix state
    m_context.m_projectionMatrixStack.clear();
    m_context.m_finalProjectionMatrixStack.clear();
    m_context.m_projectionMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_topProjectionMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalProjectionMatrix = nimble::math::Matrix4x4f::identity();
    
    // model view matrix state
    m_context.m_finalModelViewMatrix = nimble::math::Matrix4x4f::identity();
    
    // model view projection matrix state
    m_context.m_finalModelViewProjectionMatrix = nimble::math::Matrix4x4f::identity();
    
    // matrix mode state
    m_context.m_matrixModeStack.clear();
    m_context.m_matrixModeStack.reserve(128);
    m_context.m_currentMatrixMode = renderdevice::kRenderMatrixModeModel;
    
    // setup our matrix stack
    this->setMatrixMode(renderdevice::kRenderMatrixModeModel);
    clearMatrix();
    this->setMatrixMode(renderdevice::kRenderMatrixModeProjection);
    clearMatrix();
    this->setMatrixMode(renderdevice::kRenderMatrixModeView);
    clearMatrix();
    this->setMatrixMode(renderdevice::kRenderMatrixModeModel);
    
    // delegate state
    m_context.m_pRenderDeviceDelegate = 0;
}

//////////////////////////////////////////////////////////////////////////