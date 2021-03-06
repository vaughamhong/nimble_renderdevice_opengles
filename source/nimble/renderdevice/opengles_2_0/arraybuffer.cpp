//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_2_0/arraybuffer.h>
#include <nimble/core/ilockable.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! a constructor
//! \param numElements the number of elements in this buffer
//! \param elementSize the element size
//! \param usage the usage of this buffer
ArrayBuffer::ArrayBuffer(uint32_t numElements, uint32_t elementSize, uint32_t usage, GLenum target)
:m_handle(0)
,m_target(target)
,m_usage(usage)
,m_created(false)
,m_numElements(numElements)
,m_elementSize(elementSize){
    core::assert_error(m_numElements > 0);
    core::assert_error(m_elementSize > 0);
    createBuffers();
}
//! a copy constructor
//! \param arrayBuffer the arrayBuffer to copy
ArrayBuffer::ArrayBuffer(ArrayBuffer& arrayBuffer)
:m_handle(0)
,m_target(arrayBuffer.m_target)
,m_usage(arrayBuffer.m_usage)
,m_created(false)
,m_numElements(arrayBuffer.m_numElements)
,m_elementSize(arrayBuffer.m_elementSize){
    core::assert_error(m_numElements > 0);
    core::assert_error(m_elementSize > 0);
    createBuffers();

    // copy our source data to our destination
    char* pSrPointer = 0;
    char* pDestPointer = 0;
    if(arrayBuffer.lock(core::kLockTypeRead, (char**)&pSrPointer)){
        if(this->lock(core::kLockTypeWrite, (char**)&pDestPointer)){
            memcpy(pDestPointer, pSrPointer, m_numElements * m_elementSize);
            this->unlock();
            arrayBuffer.unlock();
        }else{
            core::logger_error("graphics", "Failed to lock destination array buffer");
            arrayBuffer.unlock();
        }
    }else{
        core::logger_error("graphics", "Failed to lock source array buffer");
    }
}
//! a destructor
ArrayBuffer::~ArrayBuffer(){
	destroyBuffers();
}

//////////////////////////////////////////////////////////////////////////

//! returns the usage of this array
//! \return the usage of this array
uint32_t ArrayBuffer::getUsage() const{
    return m_usage;
}
//! returns the number of elements in this array
//! \return the number of elements in this array
size_t ArrayBuffer::getNumElements() const{
    return m_numElements;
}
//! returns the size of the elements in this array
//! \return the size of the elements in this array
size_t ArrayBuffer::getElementSize() const{
    return m_elementSize;
}
//! returns the number of bytes in this array
//! \return the number of bytes in this array
size_t ArrayBuffer::getBufferSize() const{
    return m_numElements * m_elementSize;
}

//////////////////////////////////////////////////////////////////////////

//!	creates array buffers
void ArrayBuffer::createBuffers(){
    core::assert_error(!m_created);
    core::assert_error(m_numElements > 0);
    core::assert_error(m_elementSize > 0);
    
    uint32_t bufferSize = getBufferSize();
    core::assert_error(bufferSize > 0);
    
    // track previously bound
    GLint boundHandle = 0;
    if(m_target == GL_ARRAY_BUFFER){
        GLDEBUG(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundHandle));
    }else{
        GLDEBUG(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundHandle));
    }
    
    // create our array
    GLDEBUG(glGenBuffers(1, &m_handle));
    GLDEBUG(glBindBuffer(m_target, m_handle));
    if(m_usage & renderdevice::kArrayUsageVolitile){
        GLDEBUG(glBufferData(m_target, bufferSize, 0, GL_DYNAMIC_DRAW));
    }else if(m_usage & renderdevice::kArrayUsageStatic){
        GLDEBUG(glBufferData(m_target, bufferSize, 0, GL_STATIC_DRAW));
    }else{
        GLDEBUG(glBufferData(m_target, bufferSize, 0, GL_DYNAMIC_DRAW));
    }
    
    // restore previously bound
    GLDEBUG(glBindBuffer(m_target, boundHandle));
    m_created = true;
}
//!	destroys array buffers
void ArrayBuffer::destroyBuffers(){
    core::assert_error(m_created);
    
    if(m_handle){
        GLDEBUG(glDeleteBuffers(1, &m_handle));
        m_handle = 0;
    }
    m_created = false;
}

//////////////////////////////////////////////////////////////////////////

//! returns the handle
GLuint ArrayBuffer::getArrayHandle() const{
    return m_handle;
}
//! returns the target type
GLuint ArrayBuffer::getTargetType() const{
    return m_target;
}

//////////////////////////////////////////////////////////////////////////

//! attempts to map a buffer
char* ArrayBuffer::mapBuffer(core::eLockType lockType){
    core::assert_error(m_created);
    core::assert_error(!isLocked());
    return this->mapBufferRange(lockType, 0, this->getBufferSize());
}
//! attempts to map a buffer
char* ArrayBuffer::mapBufferRange(core::eLockType lockType, uint32_t offset, uint32_t size){
    core::assert_error(m_created);
    core::assert_error(!isLocked());
    core::assert_error(offset < this->getBufferSize());
    core::assert_error((offset + size) <= this->getBufferSize());
    
    //! maps RenderDevice enums to GL
    //! note: OpenGLES 1.1 doesn't support reading on locks
    //! note: use a local buffer for reading
    static int bufferLockMap[core::kMaxLockTypes] ={
        -1,
        GL_WRITE_ONLY_OES,
        -1};
    core::assert_error(bufferLockMap[lockType] != -1);
    
    // bind buffer if not already done so
    GLDEBUG(glBindBuffer(m_target, m_handle));
    
    // tell driver to orphan our previous buffer and
    // allocate a new one. This removes the need to synchronize (which
    // blocks until commands are flushed).
    if(m_usage & renderdevice::kArrayUsageVolitile){
        GLDEBUG(glBufferData(m_target, size, 0, GL_DYNAMIC_DRAW));
    }else if(m_usage & renderdevice::kArrayUsageStatic){
        GLDEBUG(glBufferData(m_target, size, 0, GL_STATIC_DRAW));
    }else{
        GLDEBUG(glBufferData(m_target, size, 0, GL_DYNAMIC_DRAW));
    }
    
    // map our buffer
    GLenum readWrite = bufferLockMap[lockType];
    void *ptr = GLDEBUG(glMapBufferOES(m_target, readWrite));
    if(ptr){
        return &((char*)ptr)[offset];
    }
    core::Lockable::unlock();
    return 0;
}
//! attempts to unmap a buffer
void ArrayBuffer::unmapBuffer(){
    core::assert_error(m_created);
    GLDEBUG(glUnmapBuffer(m_target));
    GLDEBUG(glBindBuffer(m_target, 0));
}

//////////////////////////////////////////////////////////////////////////