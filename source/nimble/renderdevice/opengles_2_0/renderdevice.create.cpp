//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/renderdevice.h>
#include <nimble/renderdevice-opengles_2_0/framebuffer.h>
#include <nimble/renderdevice-opengles_2_0/vertexbuffer.h>
#include <nimble/renderdevice-opengles_2_0/indexbuffer.h>
#include <nimble/renderdevice-opengles_2_0/texture.h>
#include <nimble/renderdevice-opengles_2_0/shader.h>
#include <nimble/renderdevice-opengles_2_0/shaderprogram.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! creates a frame buffer
//! \param width the width of the frame buffer
//! \param height the height of the frame buffer
//! \return a frame buffer
renderdevice::IFrameBuffer* RenderDevice::createFrameBuffer(core::UInt32 width, core::UInt32 height){
    return new renderdevice::opengles_2_0::FrameBuffer(width, height);
}
//! creates a vertex array
//! \param format the vertex format
//! \param numVertices the number of vertices in this buffer
//! \param usage the usage hints for this buffer
//! \return a vertex array
renderdevice::IVertexBuffer* RenderDevice::createVertexBuffer(renderdevice::VertexFormat const &vertexFormat, core::UInt32 numVertices, core::UInt32 usage){
    return new renderdevice::opengles_2_0::VertexBuffer(vertexFormat, numVertices, usage);
}
//! creates a index array
//! \param primitiveType the primitive type
//! \param indexType the index type
//! \param numindices the number of indices in this buffer
//! \param usage the usage hints for this buffer
//! \return a index array
renderdevice::IIndexBuffer* RenderDevice::createIndexBuffer(renderdevice::IIndexBuffer::ePrimitiveType primitiveType, renderdevice::IIndexBuffer::eIndexType indexType, core::UInt32 numIndices, core::UInt32 usage){
    return new renderdevice::opengles_2_0::IndexBuffer(primitiveType, indexType, numIndices, usage);
}
//! creates a texture
//! \param width the width of the texture
//! \param height the height of the texture
//! \param format the format of the texture
//! \param usage the usage hints for this texture
//! \return a texture
renderdevice::ITexture* RenderDevice::createTexture(core::UInt32 width, core::UInt32 height, renderdevice::ITexture::eFormat format, core::UInt32 usage){
    return new renderdevice::opengles_2_0::Texture(width, height, format, usage);
}
//! creates a shader
//! \param type the shader type
//! \return a shader
renderdevice::IShader* RenderDevice::createShader(renderdevice::IShader::eType type){
    return new renderdevice::opengles_2_0::Shader(type);
}
//! creates a program
//! \return a program
renderdevice::IShaderProgram* RenderDevice::createShaderProgram(){
    return new renderdevice::opengles_2_0::ShaderProgram();
}
//! creates shader params
//! \return shader params
renderdevice::IShaderParams* RenderDevice::createShaderParams(){
    core::logError("renderdevice", "Failed to create ShaderParams - OpenGLES 1.1 does not support shaders");
    return NULL;
}

//////////////////////////////////////////////////////////////////////////