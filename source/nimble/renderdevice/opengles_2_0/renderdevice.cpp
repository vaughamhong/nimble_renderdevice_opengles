//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_2_0/renderdevice.h>
#include <nimble/renderdevice/opengles_2_0/texture.h>
#include <nimble/renderdevice/opengles_2_0/vertexbuffer.h>
#include <nimble/renderdevice/opengles_2_0/indexbuffer.h>
#include <nimble/renderdevice/opengles_2_0/framebuffer.h>
#include <memory>

//////////////////////////////////////////////////////////////////////////

#define BUFFER_OFFSET(i) ((void*)(i))

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
RenderDevice::RenderDevice(){
    // reset our internal context
    this->resetRenderContext();
}
//! Destructor
RenderDevice::~RenderDevice(){
	assert(m_context.m_pRenderDeviceDelegate != NULL);
	m_context.m_pRenderDeviceDelegate = NULL;
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
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor0Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor1Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor2Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor3Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor4Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor5Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor6Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferColor7Bit){glBufferMask |= GL_COLOR_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferDepthBit){glBufferMask |= GL_DEPTH_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferStencilBit){glBufferMask |= GL_STENCIL_BUFFER_BIT;}
	if(bufferMask & renderdevice::IFrameBuffer::kBufferAccumulationBit){assert(false);}
	glClear(glBufferMask);
}
//! sets our clear color
//! \param r red component
//! \param g green component
//! \param b blue component
void RenderDevice::setClearColor(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
}

//////////////////////////////////////////////////////////////////////////

//! reset our context
void RenderDevice::resetRenderContext(){
    // common states
	m_context.m_pVertexBuffer = 0;
	m_context.m_pIndexBuffer = 0;
	m_context.m_textureArray.resize(1);
    m_context.m_pShaderProgram = 0;
	m_context.m_pVertexShader = 0;
    m_context.m_pPixelShader = 0;
    m_context.m_viewport = math::Rect2i();
    m_context.m_currentTextureUnitIndex = -1;
    m_context.m_vertexAttributeEnabledMask = 0;
	
    // render states
    this->enableState(renderdevice::IRenderDevice::kStateDepthWrite);
    this->disableState(renderdevice::IRenderDevice::kStateDepthTest);
    this->disableState(renderdevice::IRenderDevice::kStateStencilTest);
    this->disableState(renderdevice::IRenderDevice::kStateCullTest);
    this->disableState(renderdevice::IRenderDevice::kStateLighting);
    this->disableState(renderdevice::IRenderDevice::kStateTexturing);
    this->setDepthFunc(renderdevice::IRenderDevice::kDepthFuncLessEqual);
    //this->setStencilFunc(renderdevice::IRenderDevice::STENCILFUNC_???);
    this->setCullFace(renderdevice::IRenderDevice::kCullFaceBack);
    this->setFrontFaceOrientation(renderdevice::IRenderDevice::kOrientationCounterClockwise);
    this->setMatrixMode(renderdevice::IRenderDevice::kMatrixModeModel);
    
	m_context.m_pFrameBuffer = 0;
    
    // matrix states
	m_context.m_modelMatrixStack.clear();
	m_context.m_finalModelMatrixStack.clear();
	m_context.m_modelMatrix = nimble::math::Matrix4x4f::identity();
	m_context.m_topModelMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalModelMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalModelMatrixChanged = false;
    
	m_context.m_viewMatrixStack.clear();
	m_context.m_finalViewMatrixStack.clear();
	m_context.m_viewMatrix = nimble::math::Matrix4x4f::identity();
	m_context.m_topViewMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalViewMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalViewMatrixChanged = false;
    
	m_context.m_projectionMatrixStack.clear();
	m_context.m_finalProjectionMatrixStack.clear();
	m_context.m_projectionMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_topProjectionMatrix = nimble::math::Matrix4x4f::identity();
	m_context.m_finalProjectionMatrix = nimble::math::Matrix4x4f::identity();
    m_context.m_finalProjectionMatrixChanged = false;
    
	m_context.m_finalMatrix = nimble::math::Matrix4x4f::identity();
	m_context.m_matrixModeStack.clear();
	m_context.m_matrixModeStack.reserve(128);
    m_context.m_currentMatrixMode = renderdevice::IRenderDevice::kMatrixModeModel;
    
	// setup our matrix stack
	this->setMatrixMode(renderdevice::IRenderDevice::kMatrixModeModel);
	clearMatrix();
	this->setMatrixMode(renderdevice::IRenderDevice::kMatrixModeProjection);
	clearMatrix();
	this->setMatrixMode(renderdevice::IRenderDevice::kMatrixModeView);
	clearMatrix();
	this->setMatrixMode(renderdevice::IRenderDevice::kMatrixModeModel);
    
    m_context.m_pRenderDeviceDelegate = NULL;
}

//////////////////////////////////////////////////////////////////////////