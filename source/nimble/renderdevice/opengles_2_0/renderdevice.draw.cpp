//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/renderdevice.h>
#include <nimble/renderdevice-opengles_2_0/framebuffer.h>
#include <nimble/renderdevice-opengles_2_0/mappings.h>

//////////////////////////////////////////////////////////////////////////

#define BUFFER_OFFSET(i) ((void*)(i))

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! draws with current state
void RenderDevice::draw(){
    renderdevice::IIndexBuffer *pIndexBuffer = m_context.m_pIndexBuffer;
    core::assert_error(pIndexBuffer != NULL);
    
    // draw data
    unsigned int mode = gIndexFormatMap[pIndexBuffer->getPrimitiveType()];
    unsigned int count = pIndexBuffer->getNumIndices();
    
    // OpenGL ES 2.0 doesn't support QUAD LISTS
    core::assert_error(mode != renderdevice::IIndexBuffer::kPrimitiveQuadList);
    // OpenGL ES 2.0 doesn't support anything but short indices
    core::assert_error(renderdevice::IIndexBuffer::kIndexTypeShort == pIndexBuffer->getIndexType());

    // bind various internal parameters
    this->setGLMatrix(renderdevice::IRenderDevice::kMatrixModeProjection);
    this->setGLMatrix(renderdevice::IRenderDevice::kMatrixModeView);
    this->setGLMatrix(renderdevice::IRenderDevice::kMatrixModeModel);
    
    // draw
    glDrawElements(mode, count, GL_UNSIGNED_SHORT, 0);
}
//! draws with current state
void RenderDevice::draw(core::UInt32 startIndex, core::UInt32 numIndices){
    renderdevice::IIndexBuffer *pIndexBuffer = m_context.m_pIndexBuffer;
    core::assert_error(pIndexBuffer != NULL);
    
    // draw data
    core::UInt32 mode = gIndexFormatMap[pIndexBuffer->getPrimitiveType()];
    core::UInt32 count = numIndices;
    
    // OpenGL ES 2.0 doesn't support QUAD LISTS
    core::assert_error(mode != renderdevice::IIndexBuffer::kPrimitiveQuadList);
    // OpenGL ES 2.0 doesn't support anything but short indices
    core::assert_error(renderdevice::IIndexBuffer::kIndexTypeShort == pIndexBuffer->getIndexType());
    
    // bind various internal parameters
    this->setGLMatrix(renderdevice::IRenderDevice::kMatrixModeProjection);
    this->setGLMatrix(renderdevice::IRenderDevice::kMatrixModeView);
    this->setGLMatrix(renderdevice::IRenderDevice::kMatrixModeModel);
    
    // draw
    core::Size byteOffset = startIndex * sizeof(short);
    glDrawElements(mode, count, GL_UNSIGNED_SHORT, BUFFER_OFFSET(byteOffset));
}

//////////////////////////////////////////////////////////////////////////

//! starts rendering a frame
void RenderDevice::beginFrame(){
    assert(m_context.m_pRenderDeviceDelegate != NULL);
    m_context.m_pRenderDeviceDelegate->frameHasBegun();
}
//! ends rendering a frame
void RenderDevice::endFrame(){
    assert(m_context.m_pRenderDeviceDelegate != NULL);
    m_context.m_pRenderDeviceDelegate->frameHasEnded();
    renderdevice::opengles_2_0::FrameBuffer *pNativeFrameBuffer = dynamic_cast<renderdevice::opengles_2_0::FrameBuffer*>(m_context.m_pFrameBuffer);
    core::assert_error(pNativeFrameBuffer != NULL);
    pNativeFrameBuffer->discardDepthBuffer();
}

//////////////////////////////////////////////////////////////////////////