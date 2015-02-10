//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_1_1/arraybuffer.h>
#include <nimble/core/ilockable.h>
#include <nimble/core/debug.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_1_1;

//////////////////////////////////////////////////////////////////////////

//! a constructor
//! \param numElements the number of elements in this buffer
//! \param elementSize the element size
//! \param usage the usage of this buffer
ArrayBuffer::ArrayBuffer(core::UInt32 numElements, core::UInt32 elementSize, core::UInt32 usage, GLenum target)
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
    if(arrayBuffer.lock(core::ILockable::kTypeRead, (char**)&pSrPointer)){
        if(this->lock(core::ILockable::kTypeWrite, (char**)&pDestPointer)){
            memcpy(pDestPointer, pSrPointer, m_numElements * m_elementSize);
            this->unlock();
            arrayBuffer.unlock();
        }else{
            core::logError("graphics", "Failed to lock destination array buffer");
            arrayBuffer.unlock();
        }
    }else{
        core::logError("graphics", "Failed to lock source array buffer");
    }
}
//! a destructor
ArrayBuffer::~ArrayBuffer(){
	destroyBuffers();
}

//////////////////////////////////////////////////////////////////////////

//! returns the usage of this array
//! \return the usage of this array
core::UInt32 ArrayBuffer::getUsage() const{
    return m_usage;
}
//! returns the number of elements in this array
//! \return the number of elements in this array
core::UInt32 ArrayBuffer::getNumElements() const{
    return m_numElements;
}
//! returns the size of the elements in this array
//! \return the size of the elements in this array
core::UInt32 ArrayBuffer::getElementSize() const{
    return m_elementSize;
}
//! returns the number of bytes in this array
//! \return the number of bytes in this array
core::UInt32 ArrayBuffer::getBufferSize() const{
    return m_numElements * m_elementSize;
}

//////////////////////////////////////////////////////////////////////////

//!	creates array buffers
void ArrayBuffer::createBuffers(){
    core::assert_error(!m_created);
    core::assert_error(m_numElements > 0);
    core::assert_error(m_elementSize > 0);

    core::UInt32 bufferSize = getBufferSize();
    core::assert_error(bufferSize > 0);

	// create a local buffer
	if(m_usage & renderdevice::IArrayBuffer::kUsageLocalBuffer){
		m_localBuffer.resize(bufferSize);
	}

    // create hw buffer
    if(m_usage ^ renderdevice::IArrayBuffer::kUsageLocalBuffer){
        // track previously bound
        GLint boundHandle;
        if(m_target == GL_ARRAY_BUFFER){
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundHandle);
        }else{
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundHandle);
        }
        
        // create our array
        glGenBuffers(1, &m_handle);
        glBindBuffer(m_target, m_handle);
        if(m_usage & renderdevice::IArrayBuffer::kUsageVolitile){
            glBufferData(m_target, bufferSize, 0, GL_DYNAMIC_DRAW);
        }else if(m_usage & renderdevice::IArrayBuffer::kUsageStatic){
            glBufferData(m_target, bufferSize, 0, GL_STATIC_DRAW);
        }else{
            glBufferData(m_target, bufferSize, 0, GL_DYNAMIC_DRAW);
        }
        
        // restore previously bound
        glBindBuffer(m_target, boundHandle);
    }
    m_created = true;
}
//!	destroys array buffers
void ArrayBuffer::destroyBuffers(){
    core::assert_error(m_created);
    
	if(m_usage & renderdevice::IArrayBuffer::kUsageLocalBuffer){
		m_localBuffer = core::Buffer();
	}
    if(m_usage ^ renderdevice::IArrayBuffer::kUsageLocalBuffer){
        if(m_handle){
            glDeleteBuffers(1, &m_handle);
            m_handle = 0;
        }
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
core::Int8* ArrayBuffer::mapBuffer(eLockType lockType){
    core::assert_error(m_created);
    core::assert_error(!isLocked());
    return this->mapBufferRange(lockType, 0, this->getBufferSize());
}
//! attempts to map a buffer
core::Int8* ArrayBuffer::mapBufferRange(eLockType lockType, core::UInt32 offset, core::UInt32 size){
    core::assert_error(m_created);
    core::assert_error(!isLocked());
    core::assert_error(offset < this->getBufferSize());
    core::assert_error((offset + size) <= this->getBufferSize());
    
    if(m_usage & renderdevice::IArrayBuffer::kUsageLocalBuffer){
        if(m_localBuffer.getPointer()){
            return m_localBuffer.getPointer(offset);
        }
    }else{
        //! maps RenderDevice enums to GL
        //! note: OpenGLES 1.1 doesn't support reading on locks
        //! note: use a local buffer for reading
        static int bufferLockMap[core::ILockable::kMaxLockTypes] ={
            -1,
            GL_WRITE_ONLY_OES,
            -1};
        core::assert_error(bufferLockMap[lockType] != -1);
        
        // bind buffer if not already done so
        glBindBuffer(m_target, m_handle);
        
        // map our buffer
        GLenum readWrite = bufferLockMap[lockType];
        void* ptr = glMapBufferOES(m_target, readWrite);
        if(ptr){
            return &((char*)ptr)[offset];
        }
        // note: un-binding the buffer before unmapping seems to invalidate our mapped pointer
        // glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    }
    core::Lockable::unlock();
    return NULL;
}
//! attempts to unmap a buffer
void ArrayBuffer::unmapBuffer(){
    core::assert_error(m_created);
    if(m_usage & renderdevice::IArrayBuffer::kUsageLocalBuffer){
        if(m_usage ^ renderdevice::IArrayBuffer::kUsageLocalBuffer){
            core::ILockable::eLockType lockType = getLockType();
            if(lockType != core::ILockable::kTypeRead){
                core::UInt32 bufferSize = getBufferSize();
                glBindBuffer(m_target, m_handle);
                if(lockType == core::ILockable::kTypeRead && m_usage & renderdevice::IArrayBuffer::kUsageVolitile){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_DYNAMIC_DRAW);
                }else if(lockType == core::ILockable::kTypeRead && m_usage & renderdevice::IArrayBuffer::kUsageStatic){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_STATIC_DRAW);
                }else if(lockType == core::ILockable::kTypeRead && m_usage & renderdevice::IArrayBuffer::kUsageDynamic){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_DYNAMIC_DRAW);
                }else if(lockType == core::ILockable::kTypeWrite && m_usage & renderdevice::IArrayBuffer::kUsageVolitile){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_DYNAMIC_DRAW);
                }else if(lockType == core::ILockable::kTypeWrite && m_usage & renderdevice::IArrayBuffer::kUsageStatic){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_STATIC_DRAW);
                }else if(lockType == core::ILockable::kTypeWrite && m_usage & renderdevice::IArrayBuffer::kUsageDynamic){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_DYNAMIC_DRAW);
                }else if(lockType == core::ILockable::kTypeReadWrite && m_usage & renderdevice::IArrayBuffer::kUsageVolitile){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_DYNAMIC_DRAW);
                }else if(lockType == core::ILockable::kTypeReadWrite && m_usage & renderdevice::IArrayBuffer::kUsageStatic){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_STATIC_DRAW);
                }else if(lockType == core::ILockable::kTypeReadWrite && m_usage & renderdevice::IArrayBuffer::kUsageDynamic){
                    glBufferData(m_target, bufferSize, (const void*)m_localBuffer.getPointer(), GL_DYNAMIC_DRAW);
                }
                glBindBuffer(m_target, 0);
            }
        }
    }else{
        glUnmapBufferOES(m_target);
        glBindBuffer(m_target, 0);
    }
}

//////////////////////////////////////////////////////////////////////////