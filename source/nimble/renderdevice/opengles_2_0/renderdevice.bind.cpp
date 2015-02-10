//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/renderdevice.h>
#include <nimble/renderdevice-opengles_2_0/framebuffer.h>
#include <nimble/renderdevice-opengles_2_0/shaderprogram.h>
#include <nimble/renderdevice-opengles_2_0/vertexbuffer.h>
#include <nimble/renderdevice-opengles_2_0/indexbuffer.h>
#include <nimble/renderdevice-opengles_2_0/texture.h>
#include <nimble/renderdevice-opengles_2_0/mappings.h>
#include <nimble/core/debug.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

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
    if(renderdevice::opengles_2_0::FrameBuffer* pNativeFrameBuffer = dynamic_cast<renderdevice::opengles_2_0::FrameBuffer*>(pFrameBuffer)){
        core::assert_error(pNativeFrameBuffer, "Invalid native frame buffer");
        m_context.m_pFrameBuffer = pFrameBuffer;
        glBindFramebuffer(GL_FRAMEBUFFER, pNativeFrameBuffer->getFrameBufferHandle());
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
    if(renderdevice::opengles_2_0::IndexBuffer* pNativeIndexBuffer = dynamic_cast<renderdevice::opengles_2_0::IndexBuffer*>(pIndexBuffer)){
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
    if(!m_context.m_pShaderProgram){
        core::logWarning("graphics", "No shader program to bind to");
        return false;
    }
    if(!pVertexBuffer){
        core::logWarning("graphics", "NULL vertex buffer parameter being bound");
        return false;
    }
    if(m_context.m_pVertexBuffer == pVertexBuffer && !force){
        return false;
    }
    if(renderdevice::opengles_2_0::VertexBuffer* pNativeVertexBuffer = dynamic_cast<renderdevice::opengles_2_0::VertexBuffer*>(pVertexBuffer)){
        if(renderdevice::opengles_2_0::ShaderProgram *pNativeShaderProgram = dynamic_cast<renderdevice::opengles_2_0::ShaderProgram*>(m_context.m_pShaderProgram)){
            core::assert_error(pNativeVertexBuffer, "Invalid native vertex buffer");
            m_context.m_pVertexBuffer = pVertexBuffer;
            glBindBuffer(GL_ARRAY_BUFFER, pNativeVertexBuffer->getArrayBufferHandle());
            
            // grab our program handle
            GLuint programHandle = pNativeShaderProgram->m_programHandle;
            
            // grab our vertex format
            const renderdevice::VertexFormat &vertexFormat = pVertexBuffer->getVertexFormat();
            assert(vertexFormat.getVertexStride() > 0);
            
            // attach each vertex attribute
            unsigned int numAttributes = vertexFormat.getNumAttributes();
            unsigned int vertexStride = vertexFormat.getVertexStride();
            int originalVertexAttributeEnabledMask = m_context.m_vertexAttributeEnabledMask;
            m_context.m_vertexAttributeEnabledMask = 0x0;
            core::Size offset = 0;
            for(unsigned int i = 0; i < numAttributes; ++i){
                // attribute information
                const char *name = vertexFormat.getAttributeName(i);
                renderdevice::VertexFormat::eAttributeSemantic vertexSemantic = vertexFormat.getAttributeSemantic(i);
                renderdevice::VertexFormat::eAttributeType vertexType = vertexFormat.getAttributeType(i);
                
                // gl attribute information
                GLenum type = gVertexAttributeTypeMap[vertexType];
                unsigned int dimension = gVertexAttributeTypeDimensionMap[vertexType];
                GLint attributeHandle = glGetAttribLocation(programHandle, name);
                
                // bind attribute
                if(attributeHandle >= 0){
                    int vertexSemanticBit = (1 << vertexSemantic);
                    if(!(originalVertexAttributeEnabledMask & vertexSemanticBit)){
                        glEnableVertexAttribArray(attributeHandle);
                    }
                    m_context.m_vertexAttributeEnabledMask |= vertexSemanticBit;
                    glVertexAttribPointer(attributeHandle, dimension, type, GL_TRUE, vertexStride, (char*)offset);
                }
                offset += vertexFormat.getAttributeTypeByteSize(vertexType);
            }
            return true;
        }else{
            core::logWarning("graphics", "Invalid native shader program");
            return false;
        }
    }else{
        core::logWarning("graphics", "Invalid native vertex buffer");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets texture
//! \param textureUnit the index of the texture unit to replace
//! \param pTexture the texture data
bool RenderDevice::bindTexture(core::UInt32 textureUnit, renderdevice::ITexture* pTexture, bool force){
	if(!m_context.m_pShaderProgram){
        core::logWarning("graphics", "No shader program to bind to");
        return false;
    }
    if(!pTexture){
        core::logWarning("graphics", "NULL texture being bound to unit %d", textureUnit);
        return false;
    }
    if(m_context.m_textureArray[textureUnit] == pTexture && !force){
        return false;
    }
    
    if(renderdevice::opengles_2_0::ShaderProgram *pNativeShaderProgram = dynamic_cast<renderdevice::opengles_2_0::ShaderProgram*>(m_context.m_pShaderProgram)){
        if(renderdevice::opengles_2_0::Texture *pNativeTexture = dynamic_cast<renderdevice::opengles_2_0::Texture*>(pTexture)){
            // grab our program handle
            GLuint programHandle = pNativeShaderProgram->m_programHandle;
            
            core::assert_error(pNativeTexture, "Invalid native texture");
            unsigned int dimension  = gTextureDimensionMap[pNativeTexture->getDimension()];
            m_context.m_textureArray[textureUnit] = pTexture;
            if(m_context.m_currentTextureUnitIndex != textureUnit){
                m_context.m_currentTextureUnitIndex = textureUnit;
                unsigned int internalTextureUnit = gTextureUnitMap[textureUnit];
                glActiveTexture(internalTextureUnit);
            }
            
            // gl attribute information
            glBindTexture(dimension, pNativeTexture->getTextureHandle());
            GLint attributeHandle = glGetAttribLocation(programHandle, "nimble_tex[0]");
            glUniform1i(attributeHandle, gTextureUnitMap[textureUnit]);
            return true;
        }else{
            core::logWarning("graphics", "Invalid native texture");
            return false;
        }
    }else{
        core::logWarning("graphics", "Invalid native shader program");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets a program
//! \param pShaderProgram the program
//! \return true if successful
bool RenderDevice::bindShaderProgram(renderdevice::IShaderProgram* pShaderProgram){
    if(renderdevice::opengles_2_0::ShaderProgram *pNativeShaderProgram = dynamic_cast<renderdevice::opengles_2_0::ShaderProgram*>(pShaderProgram)){
        core::assert_error(pNativeShaderProgram, "Invalid native shader program");
        m_context.m_pShaderProgram = pShaderProgram;
        glUseProgram(pNativeShaderProgram->getShaderProgramHandle());
        return true;
    }else{
        core::logError("graphics", "Failed to query and bind a native shader");
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////