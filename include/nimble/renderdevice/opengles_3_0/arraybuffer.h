//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_arraybuffer_h__
#define __nimble_renderdevice_opengles_3_0_arraybuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <nimble/core/Lockable.h>
#include <nimble/resource/resource.h>
#include <nimble/renderdevice/iarraybuffer.h>
#include <nimble/renderdevice/opengles_3_0/common.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{
            class RenderDevice;
            class IndexBuffer;
            class VertexBuffer;
            class Texture;
            
            //! Array buffer impl
            class ArrayBuffer
            : public virtual renderdevice::IArrayBuffer
            , public core::Lockable
            , public resource::Resource{
                friend class nimble::renderdevice::opengles_3_0::IndexBuffer;
                friend class nimble::renderdevice::opengles_3_0::VertexBuffer;
                friend class nimble::renderdevice::opengles_3_0::Texture;
            private:
                
                GLuint      m_handle;
                GLenum      m_target;
                
                uint32_t    m_usage;
                bool        m_created;
                
                size_t      m_numElements;
                size_t      m_elementSize;
                
            public:
                
                //! a constructor
                //! \param numElements the number of elements in this buffer
                //! \param elementSize the element size
                //! \param usage the usage of this buffer
                ArrayBuffer(uint32_t numElements, uint32_t elementSize, uint32_t usage, GLenum target);
                //! a copy constructor
                //! \param buffer the buffer to copy
                ArrayBuffer(ArrayBuffer& buffer);
                //! a destructor
                virtual ~ArrayBuffer();
                
            public:
                
                //! returns the usage of this array
                //! \return the usage of this array
                virtual uint32_t getUsage() const;
                //! returns the number of elements in this array
                //! \return the number of elements in this array
                virtual size_t getNumElements() const;
                //! returns the size of the elements in this array
                //! \return the size of the elements in this array
                virtual size_t getElementSize() const;
                //! returns the number of bytes in this array
                //! \return the number of bytes in this array
                virtual size_t getBufferSize() const;
                
                //! returns the handle
                GLuint getArrayHandle() const;
                //! returns the target type
                GLuint getTargetType() const;
                
            private:
                
                //!	creates array buffers
                void createBuffers();
                //!	destroys array buffers
                void destroyBuffers();
                
            protected:
                
                //! attempts to map array buffer
                virtual char* mapBuffer(core::eLockType lockType);
                //! attempts to map array buffer
                virtual char* mapBufferRange(core::eLockType lockType, uint32_t offset, uint32_t size);
                //! attempts to unmap array buffer
                virtual void unmapBuffer();
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////