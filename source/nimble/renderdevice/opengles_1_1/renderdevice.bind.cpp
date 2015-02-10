//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_1_1/renderdevice.h>
#include <nimble/renderdevice-opengles_1_1/framebuffer.h>
#include <nimble/renderdevice-opengles_1_1/vertexbuffer.h>
#include <nimble/renderdevice-opengles_1_1/indexbuffer.h>
#include <nimble/renderdevice-opengles_1_1/texture.h>
#include <nimble/renderdevice-opengles_1_1/mappings.h>
#include <nimble/core/debug.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_1_1;

//////////////////////////////////////////////////////////////////////////

//! sets frame buffer
//! \param pFrameBuffer the frame buffer
//! \return true if successful
bool RenderDevice::bindFrameBuffer(renderdevice::IFrameBuffer* pFrameBuffer, bool force){
    if(!pFrameBuffer){
        core::logWarning("graphics", "NULL frame buffer parameter being bound");
        return false;
    }
    if(m_context.m_pFrameBuffer == pFrameBuffer && !force){
        return false;
    }
    if(renderdevice::opengles_1_1::FrameBuffer* pNativeFrameBuffer = dynamic_cast<renderdevice::opengles_1_1::FrameBuffer*>(pFrameBuffer)){
        core::assert_error(pNativeFrameBuffer, "Invalid native frame buffer");
        m_context.m_pFrameBuffer = pFrameBuffer;
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, pNativeFrameBuffer->getFrameBufferHandle());
        return true;
    }else{
        core::logError("graphics", "Failed to query and bind a native frame buffer");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets index array
//! \param pIndexBuffer the index array to bind
bool RenderDevice::bindIndexBuffer(renderdevice::IIndexBuffer* pIndexBuffer, bool force){
    if(!pIndexBuffer){
        core::logWarning("graphics", "NULL index buffer parameter being bound");
        return false;
    }
    if(m_context.m_pIndexBuffer == pIndexBuffer && !force){
        return false;
    }
    if(renderdevice::opengles_1_1::IndexBuffer* pNativeIndexBuffer = dynamic_cast<renderdevice::opengles_1_1::IndexBuffer*>(pIndexBuffer)){
        core::assert_error(pNativeIndexBuffer, "Invalid native index buffer");
        m_context.m_pIndexBuffer = pIndexBuffer;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pNativeIndexBuffer->getArrayBufferHandle());
        return true;
    }else{
        core::logError("graphics", "Failed to query and bind a native index buffer");
        return false;
    }
}

//! sets vertex array
//! \param pVertexBuffer the vertex array to bind
bool RenderDevice::bindVertexBuffer(renderdevice::IVertexBuffer* pVertexBuffer, bool force){
    if(!pVertexBuffer){
        core::logWarning("graphics", "NULL vertex buffer parameter being bound");
        return false;
    }
    if(m_context.m_pVertexBuffer == pVertexBuffer && !force){
        return false;
    }
    if(renderdevice::opengles_1_1::VertexBuffer* pNativeVertexBuffer = dynamic_cast<renderdevice::opengles_1_1::VertexBuffer*>(pVertexBuffer)){
        core::assert_error(pNativeVertexBuffer, "Invalid native vertex buffer");
        m_context.m_pVertexBuffer = pVertexBuffer;
        
        // check if we actually need to rebind
        GLint currentlyBoundVboHandle;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentlyBoundVboHandle);
        if(currentlyBoundVboHandle != pNativeVertexBuffer->getArrayBufferHandle()){
            const renderdevice::VertexFormat &vertexFormat = pNativeVertexBuffer->getVertexFormat();
            unsigned int numAttributes = vertexFormat.getNumAttributes();
            unsigned int vertexStride = vertexFormat.getVertexStride();
            
            glBindBuffer(GL_ARRAY_BUFFER, pNativeVertexBuffer->getArrayBufferHandle());
            
            // enable client states / bind buffers
            core::Size offset = 0;
            int originalVertexAttributeEnabledMask = m_context.m_vertexAttributeEnabledMask;
            m_context.m_vertexAttributeEnabledMask = 0x0;
            for(unsigned int i = 0; i < numAttributes; ++i){
                renderdevice::VertexFormat::eAttributeSemantic vertexSemantic = vertexFormat.getAttributeSemantic(i);
                renderdevice::VertexFormat::eAttributeType vertexType = vertexFormat.getAttributeType(i);
                
                assert(renderdevice::VertexFormat::kFirstAttributeSemantic <= vertexSemantic && vertexSemantic < renderdevice::VertexFormat::kMaxAttributeSemantics);
                assert(renderdevice::VertexFormat::kFirstAttributeType <= vertexType && vertexType < renderdevice::VertexFormat::kMaxAttributeTypes);
                
                GLenum type = gVertexAttributeTypeMap[vertexType];
                unsigned int dimension = gVertexAttributeTypeDimensionMap[vertexType];
                int vertexSemanticBit = (1 << vertexSemantic);
                
                switch(vertexSemantic){
                    case VertexFormat::kSemanticPosition:
                        if(!(originalVertexAttributeEnabledMask & vertexSemanticBit)){
                            glEnableClientState(GL_VERTEX_ARRAY);
                        }
                        m_context.m_vertexAttributeEnabledMask |= vertexSemanticBit;
                        glVertexPointer(dimension, type, vertexStride, (char*)offset);
                        break;
                    case VertexFormat::kSemanticNormal:
                        if(!(originalVertexAttributeEnabledMask & vertexSemanticBit)){
                            glEnableClientState(GL_NORMAL_ARRAY);
                        }
                        m_context.m_vertexAttributeEnabledMask |= vertexSemanticBit;
                        glNormalPointer(type, vertexStride, (char*)offset);
                        break;
                    case VertexFormat::kSemanticColor:
                        if(!(originalVertexAttributeEnabledMask & vertexSemanticBit)){
                            glEnableClientState(GL_COLOR_ARRAY);
                        }
                        m_context.m_vertexAttributeEnabledMask |= vertexSemanticBit;
                        glColorPointer(dimension, type, vertexStride, (char*)offset);
                        break;
                    case VertexFormat::kSemanticTexCoord:
                        if(!(originalVertexAttributeEnabledMask & vertexSemanticBit)){
                            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                        }
                        m_context.m_vertexAttributeEnabledMask |= vertexSemanticBit;
                        glTexCoordPointer(dimension, type, vertexStride, (char*)offset);
                        break;
                    default:
                        break;
                }
                offset += vertexFormat.getAttributeTypeByteSize(vertexType);
            }
            
            // disable client states
            for(int i = 0; i < renderdevice::VertexFormat::kMaxAttributeSemantics; i++){
                int vertexSemanticBit = (1 << i);
                if((vertexSemanticBit & originalVertexAttributeEnabledMask) && !(vertexSemanticBit & m_context.m_vertexAttributeEnabledMask)){
                    switch(i){
                        case VertexFormat::kSemanticPosition:
                            glDisableClientState(GL_VERTEX_ARRAY);
                            break;
                        case VertexFormat::kSemanticNormal:
                            glDisableClientState(GL_NORMAL_ARRAY);
                            break;
                        case VertexFormat::kSemanticColor:
                            glDisableClientState(GL_COLOR_ARRAY);
                            break;
                        case VertexFormat::kSemanticTexCoord:
                            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        return true;
    }else{
        core::logError("graphics", "Failed to query and bind a native vertex buffer");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets texture
//! \param textureUnit the index of the texture unit to replace
//! \param pTexture the texture data
bool RenderDevice::bindTexture(core::UInt32 textureUnit, renderdevice::ITexture* pTexture, bool force){
	if(!pTexture){
        core::logWarning("graphics", "NULL texture being bound to unit %d", textureUnit);
        return false;
    }
    if(m_context.m_textureArray[textureUnit] == pTexture && !force){
        return false;
    }
    
    if(renderdevice::opengles_1_1::Texture *pNativeTexture = dynamic_cast<renderdevice::opengles_1_1::Texture*>(pTexture)){
        core::assert_error(pNativeTexture, "Invalid native texture");
        unsigned int dimension  = gTextureDimensionMap[pNativeTexture->getDimension()];
        m_context.m_textureArray[textureUnit] = pTexture;
        if(m_context.m_currentTextureUnitIndex != textureUnit){
            m_context.m_currentTextureUnitIndex = textureUnit;
            unsigned int internalTextureUnit = gTextureUnitMap[textureUnit];
            glActiveTexture(internalTextureUnit);
        }
        glBindTexture(dimension, pNativeTexture->getTextureHandle());
        return true;
    }else{
        core::logError("graphics", "Failed to query and bind a native texture");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets a program
//! \param pShaderProgram the program
//! \return true if successful
bool RenderDevice::bindShaderProgram(renderdevice::IShaderProgram* pShaderProgram){
    core::logError("renderdevice", "Failed to bind ShaderProgram - OpenGLES 1.1 does not support shaders");
    return false;
}

//////////////////////////////////////////////////////////////////////////