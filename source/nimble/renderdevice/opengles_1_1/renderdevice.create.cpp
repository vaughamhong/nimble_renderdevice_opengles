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

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_1_1;

//////////////////////////////////////////////////////////////////////////

//! creates a frame buffer
//! \param width the width of the frame buffer
//! \param height the height of the frame buffer
//! \return a frame buffer
renderdevice::IFrameBuffer* RenderDevice::createFrameBuffer(core::UInt32 width, core::UInt32 height){
    return new renderdevice::opengles_1_1::FrameBuffer(width, height);
}
//! creates a vertex array
//! \param format the vertex format
//! \param numVertices the number of vertices in this buffer
//! \param usage the usage hints for this buffer
//! \return a vertex array
renderdevice::IVertexBuffer* RenderDevice::createVertexBuffer(renderdevice::VertexFormat const &vertexFormat, core::UInt32 numVertices, core::UInt32 usage){
    return new renderdevice::opengles_1_1::VertexBuffer(vertexFormat, numVertices, usage);
}
//! creates a index array
//! \param primitiveType the primitive type
//! \param indexType the index type
//! \param numindices the number of indices in this buffer
//! \param usage the usage hints for this buffer
//! \return a index array
renderdevice::IIndexBuffer* RenderDevice::createIndexBuffer(renderdevice::IIndexBuffer::ePrimitiveType primitiveType, renderdevice::IIndexBuffer::eIndexType indexType, core::UInt32 numIndices, core::UInt32 usage){
    return new renderdevice::opengles_1_1::IndexBuffer(primitiveType, indexType, numIndices, usage);
}
//! creates a texture
//! \param width the width of the texture
//! \param height the height of the texture
//! \param format the format of the texture
//! \param usage the usage hints for this texture
//! \return a texture
renderdevice::ITexture* RenderDevice::createTexture(core::UInt32 width, core::UInt32 height, renderdevice::ITexture::eFormat format, core::UInt32 usage){
    return new renderdevice::opengles_1_1::Texture(width, height, format, usage);
}
//! creates a shader
//! \param type the shader type
//! \return a shader
renderdevice::IShader* RenderDevice::createShader(renderdevice::IShader::eType type){
    core::logError("renderdevice", "Failed to create Shader - OpenGLES 1.1 does not support shaders");
    return NULL;
}
//! creates a program
//! \return a program
renderdevice::IShaderProgram* RenderDevice::createShaderProgram(){
    core::logError("renderdevice", "Failed to create ShaderProgram - OpenGLES 1.1 does not support shaders");
    return NULL;
}
//! creates shader params
//! \return shader params
renderdevice::IShaderParams* RenderDevice::createShaderParams(){
    core::logError("renderdevice", "Failed to create ShaderParams - OpenGLES 1.1 does not support shaders");
    return NULL;
}

//////////////////////////////////////////////////////////////////////////