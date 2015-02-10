//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/renderdevice.h>
#include <nimble/renderdevice/opengles_3_0/framebuffer.h>
#include <nimble/renderdevice/opengles_3_0/mappings.h>

//////////////////////////////////////////////////////////////////////////

#define BUFFER_OFFSET(i) ((void*)(i))

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! draws with current state
void RenderDevice::draw(){
    renderdevice::IIndexBuffer *pIndexBuffer = m_context.m_pIndexBuffer;
    core::assert_error(pIndexBuffer != 0);
    
    // draw data
    unsigned int mode = gIndexFormatMap[pIndexBuffer->getPrimitiveType()];
    unsigned int count = pIndexBuffer->getNumIndices();
    unsigned int indexType = pIndexBuffer->getIndexType();
    
    // bind various internal parameters
    this->patchShaderProgramMatrixParams();
    
    // draw
    if(indexType == renderdevice::kIndexTypeUInt8){
        GLDEBUG(glDrawElements(mode, count, GL_UNSIGNED_BYTE, 0));
    }else if(indexType == renderdevice::kIndexTypeUInt16){
        GLDEBUG(glDrawElements(mode, count, GL_UNSIGNED_SHORT, 0));
    }else if(indexType == renderdevice::kIndexTypeUInt32){
        GLDEBUG(glDrawElements(mode, count, GL_UNSIGNED_INT, 0));
    }
}
//! draws with current state
void RenderDevice::drawElements(uint32_t startIndex, uint32_t numIndices){
    renderdevice::IIndexBuffer *pIndexBuffer = m_context.m_pIndexBuffer;
    core::assert_error(pIndexBuffer != 0);
    
    // draw data
    uint32_t mode = gIndexFormatMap[pIndexBuffer->getPrimitiveType()];
    uint32_t count = numIndices;
    unsigned int indexType = pIndexBuffer->getIndexType();
    
    // bind various internal parameters
    this->patchShaderProgramMatrixParams();
    
    // draw
    size_t byteOffset = startIndex * sizeof(unsigned int);
    if(indexType == renderdevice::kIndexTypeUInt8){
        GLDEBUG(glDrawElements(mode, count, GL_UNSIGNED_BYTE, BUFFER_OFFSET(byteOffset)));
    }else if(indexType == renderdevice::kIndexTypeUInt16){
        GLDEBUG(glDrawElements(mode, count, GL_UNSIGNED_SHORT, BUFFER_OFFSET(byteOffset)));
    }else if(indexType == renderdevice::kIndexTypeUInt32){
        GLDEBUG(glDrawElements(mode, count, GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset)));
    }
}

//////////////////////////////////////////////////////////////////////////

//! starts rendering a frame
void RenderDevice::beginFrame(){
    if(m_context.m_pRenderDeviceDelegate){
        m_context.m_pRenderDeviceDelegate->frameHasBegun();
    }
    GLDEBUG(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
//! ends rendering a frame
void RenderDevice::endFrame(){
    if(m_context.m_pRenderDeviceDelegate){
        m_context.m_pRenderDeviceDelegate->frameHasEnded();
    }
    renderdevice::opengles_3_0::FrameBuffer *pNativeFrameBuffer = dynamic_cast<renderdevice::opengles_3_0::FrameBuffer*>(m_context.m_pFrameBuffer);
    if(pNativeFrameBuffer){
        pNativeFrameBuffer->discardDepthBuffer();
    }
}

//////////////////////////////////////////////////////////////////////////