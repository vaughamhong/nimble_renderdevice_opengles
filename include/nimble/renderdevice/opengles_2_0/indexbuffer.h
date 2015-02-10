//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_indexbuffer_h__
#define __nimble_renderdevice_opengles_2_0_indexbuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_2_0/arraybuffer.h>
#include <nimble/renderdevice/iindexbuffer.h>
#include <nimble/core/ilockable.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            class RenderDevice;
            
            //! Index array impl
            class IndexBuffer
            : public renderdevice::IIndexBuffer
            , public core::Lockable
            , public resource::Resource{
                friend class nimble::renderdevice::opengles_2_0::RenderDevice;
            private:
                
                renderdevice::opengles_2_0::ArrayBuffer m_arrayBuffer;
                renderdevice::ePrimitiveType            m_primitiveType;
                renderdevice::eIndexType                m_indexType;
                
            public:
                
                //! a constructor
                IndexBuffer(renderdevice::ePrimitiveType primitiveType, renderdevice::eIndexType indexType, uint32_t numIndices, uint32_t usage);
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
                virtual uint32_t getUsage() const;
                //! returns the number of bytes for a single index
                //! \return the number of bytes for a single index
                virtual size_t getIndexSize() const;
                //! returns the primitive type of this index array
                //! \return value of type enum ePrimitiveType
                virtual eIndexType getIndexType() const;
                //! returns the primitive type of this index array
                //! \return value of type enum ePrimitiveType
                virtual ePrimitiveType getPrimitiveType() const;
                //! returns the number of indices
                //! \return the number of indices
                virtual size_t getNumIndices() const;
                //! returns the number of bytes in this array
                //! \return the number of bytes in this array
                virtual size_t getBufferSize() const;
                
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