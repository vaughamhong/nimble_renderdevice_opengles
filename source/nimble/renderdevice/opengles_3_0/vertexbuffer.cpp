//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/vertexbuffer.h>
#include <nimble/renderdevice/opengles_3_0/mappings.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! a constructor
//! \param format the vertex format
//! \param numVertices the number of vertices in this buffer
//! \param usage the usage hints for this buffer
VertexBuffer::VertexBuffer(renderdevice::VertexFormat const &vertexFormat, uint32_t numVertices, uint32_t usage)
:m_arrayBuffer(numVertices, vertexFormat.getVertexStride(), usage, GL_ARRAY_BUFFER)
,m_vertexFormat(vertexFormat){
}
//! a copy constructor
//! \param vertexBuffer the vertexBuffer to copy
VertexBuffer::VertexBuffer(VertexBuffer& vertexBuffer)
:m_arrayBuffer(vertexBuffer.m_arrayBuffer)
,m_vertexFormat(vertexBuffer.getVertexFormat()){
}
//! a destructor
VertexBuffer::~VertexBuffer(){
}

//////////////////////////////////////////////////////////////////////////

//! returns the internal array buffer handle
GLuint VertexBuffer::getArrayBufferHandle() const{
    return m_arrayBuffer.getArrayHandle();
}

//////////////////////////////////////////////////////////////////////////

//! returns the usage of this array
//! \return the usage of this array
uint32_t VertexBuffer::getUsage() const{
    return m_arrayBuffer.getUsage();
}
//! returns the vertex format
const renderdevice::VertexFormat& VertexBuffer::getVertexFormat() const{
    return m_vertexFormat;
}
//! returns the vertex stride
size_t VertexBuffer::getVertexStride() const{
    return m_vertexFormat.getVertexStride();
}
//! returns the number of vertices
//! \return the number of vertices
size_t VertexBuffer::getNumVertices() const{
    return m_arrayBuffer.getNumElements();
}
//! returns the number of bytes in this array
//! \return the number of bytes in this array
size_t VertexBuffer::getBufferSize() const{
    return m_arrayBuffer.getBufferSize();
}

//////////////////////////////////////////////////////////////////////////

//! attempts to map array buffer
char* VertexBuffer::mapBuffer(core::eLockType lockType){
    return m_arrayBuffer.mapBuffer(lockType);
}
//! attempts to map array buffer
char* VertexBuffer::mapBufferRange(core::eLockType lockType, uint32_t offset, uint32_t size){
    return m_arrayBuffer.mapBufferRange(lockType, offset, size);
}
//! attempts to unmap array buffer
void VertexBuffer::unmapBuffer(){
    return m_arrayBuffer.unmapBuffer();
}


//////////////////////////////////////////////////////////////////////////