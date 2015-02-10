//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_arraybuffer_h__
#define __nimble_renderdevice_opengles_2_0_arraybuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/core/types.h>
#include <nimble/core/Lockable.h>
#include <nimble/core/buffer.h>
#include <nimble/resource/resource.h>
#include <nimble/renderdevice/iarraybuffer.h>
#include <nimble/renderdevice-opengles_2_0/common.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            class RenderDevice;
            class IndexBuffer;
            class VertexBuffer;
            
            //! Array buffer impl
            class ArrayBuffer
            : public virtual renderdevice::IArrayBuffer
            , public core::Lockable
            , public resource::Resource{
            friend class nimble::renderdevice::opengles_2_0::IndexBuffer;
            friend class nimble::renderdevice::opengles_2_0::VertexBuffer;
            private:

                GLuint          m_handle;
                GLenum          m_target;
                
                core::UInt32    m_usage;
                core::Bool      m_created;

                core::UInt32    m_numElements;
                core::UInt32    m_elementSize;
                
                core::Buffer    m_localBuffer;
                
            public:

                //! a constructor
                //! \param numElements the number of elements in this buffer
                //! \param elementSize the element size
                //! \param usage the usage of this buffer
                ArrayBuffer(core::UInt32 numElements, core::UInt32 elementSize, core::UInt32 usage, GLenum target);
                //! a copy constructor
                //! \param arrayBuffer the arrayBuffer to copy
                ArrayBuffer(ArrayBuffer& arrayBuffer);
                //! a destructor
                virtual ~ArrayBuffer();
                
            public:
                
                //! returns the usage of this array
                //! \return the usage of this array
                virtual core::UInt32 getUsage() const;
                //! returns the number of elements in this array
                //! \return the number of elements in this array
                virtual core::UInt32 getNumElements() const;
                //! returns the size of the elements in this array
                //! \return the size of the elements in this array
                virtual core::UInt32 getElementSize() const;
                //! returns the number of bytes in this array
                //! \return the number of bytes in this array
                virtual core::UInt32 getBufferSize() const;
                
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
                virtual core::Int8* mapBuffer(eLockType lockType);
                //! attempts to map array buffer
                virtual core::Int8* mapBufferRange(eLockType lockType, core::UInt32 offset, core::UInt32 size);
                //! attempts to unmap array buffer
                virtual void unmapBuffer();
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////