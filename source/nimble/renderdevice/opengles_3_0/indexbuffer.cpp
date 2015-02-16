//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/indexbuffer.h>
#include <nimble/renderdevice/opengles_3_0/mappings.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! a constructor
IndexBuffer::IndexBuffer(renderdevice::ePrimitiveType primitiveType, renderdevice::eIndexType indexType, uint32_t numIndices, uint32_t usage)
:m_arrayBuffer(numIndices, gIndexIndexTypeSizeMap[indexType], usage, GL_ELEMENT_ARRAY_BUFFER)
,m_primitiveType(primitiveType)
,m_indexType(indexType){
}
//! a copy constructor
IndexBuffer::IndexBuffer(IndexBuffer& indexBuffer)
:m_arrayBuffer(indexBuffer.m_arrayBuffer)
,m_primitiveType(indexBuffer.m_primitiveType)
,m_indexType(indexBuffer.m_indexType){
}
//! a destructor
IndexBuffer::~IndexBuffer(){
}

//////////////////////////////////////////////////////////////////////////

//! returns the internal array buffer handle
GLuint IndexBuffer::getArrayBufferHandle() const{
    return m_arrayBuffer.getArrayHandle();
}

//////////////////////////////////////////////////////////////////////////

//! returns the usage of this array
//! \return the usage of this array
uint32_t IndexBuffer::getUsage() const{
    return m_arrayBuffer.getUsage();
}
//! returns the number of bytes for a single index
//! \return the number of bytes for a single index
size_t IndexBuffer::getIndexSize() const{
    return gIndexIndexTypeSizeMap[m_indexType];
}
//! returns the primitive type of this index array
//! \return value of type enum ePrimitiveType
renderdevice::eIndexType IndexBuffer::getIndexType() const{
    return m_indexType;
}
//! returns the primitive type of this index array
//! \return value of type enum ePrimitiveType
renderdevice::ePrimitiveType IndexBuffer::getPrimitiveType() const{
    return m_primitiveType;
}
//! returns the number of indices
//! \return the number of indices
size_t IndexBuffer::getNumIndices() const{
    return m_arrayBuffer.getNumElements();
}
//! returns the number of bytes in this array
//! \return the number of bytes in this array
size_t IndexBuffer::getBufferSize() const{
    return m_arrayBuffer.getBufferSize();
}

//////////////////////////////////////////////////////////////////////////

//! attempts to map array buffer
char* IndexBuffer::mapBuffer(core::eLockType lockType){
    return m_arrayBuffer.mapBuffer(lockType);
}
//! attempts to map array buffer
char* IndexBuffer::mapBufferRange(core::eLockType lockType, uint32_t offset, uint32_t size){
    return m_arrayBuffer.mapBufferRange(lockType, offset, size);
}
//! attempts to unmap array buffer
void IndexBuffer::unmapBuffer(){
    return m_arrayBuffer.unmapBuffer();
}


//////////////////////////////////////////////////////////////////////////