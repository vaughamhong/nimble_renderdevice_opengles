//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_1_1_indexbuffer_h__
#define __nimble_renderdevice_opengles_1_1_indexbuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice-opengles_1_1/arraybuffer.h>
#include <nimble/renderdevice/iindexbuffer.h>
#include <nimble/core/ilockable.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_1_1{
            class RenderDevice;
            
            //! Index array impl
            class IndexBuffer
            : public renderdevice::IIndexBuffer
            , public core::Lockable
            , public resource::Resource{
            friend class nimble::renderdevice::opengles_1_1::RenderDevice;
            private:
                
                renderdevice::opengles_1_1::ArrayBuffer     m_arrayBuffer;
                renderdevice::IIndexBuffer::ePrimitiveType  m_primitiveType;
                renderdevice::IIndexBuffer::eIndexType      m_indexType;
                
            public:

                //! a constructor
                IndexBuffer(renderdevice::IIndexBuffer::ePrimitiveType primitiveType, renderdevice::IIndexBuffer::eIndexType indexType, core::UInt32 numIndices, core::UInt32 usage);
                //! a copy constructor
                IndexBuffer(IndexBuffer& IndexBuffer);
                //! a destructor
                virtual ~IndexBuffer();
                
            public:
                
                //! returns the internal array buffer handle
                virtual GLuint getArrayBufferHandle() const;

            public:

                //! returns the usage of this array
                //! \return the usage of this array
                virtual core::UInt32 getUsage() const;
                //! returns the number of bytes for a single index
                //! \return the number of bytes for a single index
                virtual core::Int32 getIndexSize() const;
                //! returns the primitive type of this index array
                //! \return value of type enum ePrimitiveType
                virtual eIndexType getIndexType() const;
                //! returns the primitive type of this index array
                //! \return value of type enum ePrimitiveType
                virtual ePrimitiveType getPrimitiveType() const;
                //! returns the number of indices
                //! \return the number of indices
                virtual core::UInt32 getNumIndices() const;
                //! returns the number of bytes in this array
                //! \return the number of bytes in this array
                virtual core::UInt32 getBufferSize() const;
                
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