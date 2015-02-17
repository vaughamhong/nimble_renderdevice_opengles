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
    uint32_t count = (uint32_t)m_context.m_pIndexBuffer->getNumIndices();
    uint32_t indexType = m_context.m_pIndexBuffer->getIndexType();
    uint32_t mode = m_context.m_pIndexBuffer->getPrimitiveType();
    drawElements(0, count, indexType, mode);
}
//! draws with current state
void RenderDevice::drawElements(uint32_t startIndex, uint32_t numIndices, uint32_t indexType, uint32_t mode){
    // patch our shader program with the appropriate
    // uniform matrix updates. We only update matrices that have changed.
    this->patchShaderProgramWithUpdatedMatrices();
    
    // draw
    uint32_t glmode = gIndexFormatMap[mode];
    size_t byteOffset = startIndex * renderdevice::getIndexTypeSize((renderdevice::eIndexType)indexType);
    if(indexType == renderdevice::kIndexTypeUInt8){
        GLDEBUG(glDrawElements(glmode, numIndices, GL_UNSIGNED_BYTE, BUFFER_OFFSET(byteOffset)));
    }else if(indexType == renderdevice::kIndexTypeUInt16){
        GLDEBUG(glDrawElements(glmode, numIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(byteOffset)));
    }else if(indexType == renderdevice::kIndexTypeUInt32){
        GLDEBUG(glDrawElements(glmode, numIndices, GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset)));
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