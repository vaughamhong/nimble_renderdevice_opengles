//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_vertexbuffer_h__
#define __nimble_renderdevice_opengles_3_0_vertexbuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_3_0/arraybuffer.h>
#include <nimble/renderdevice/ivertexbuffer.h>
#include <nimble/core/ilockable.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{
            class RenderDevice;
                        
            //! Vertex array impl
            class VertexBuffer
            : public renderdevice::IVertexBuffer
            , public core::Lockable
            , public resource::Resource{
                friend class nimble::renderdevice::opengles_3_0::RenderDevice;
            private:
                
                renderdevice::opengles_3_0::ArrayBuffer m_arrayBuffer;
                renderdevice::VertexFormat              m_vertexFormat;
                
            public:
                
                //! a constructor
                //! \param format the vertex format
                //! \param numVertices the number of vertices in this buffer
                //! \param usage the usage hints for this buffer
                VertexBuffer(renderdevice::VertexFormat const &vertexFormat, uint32_t numVertices, uint32_t usage);
                //! a copy constructor
                //! \param vertexBuffer the vertexBuffer to copy
                VertexBuffer(VertexBuffer& vertexBuffer);
                //! a destructor
                virtual ~VertexBuffer();
                
            public:
                
                //! returns the internal array buffer handle
                virtual GLuint getArrayBufferHandle() const;
                
            public:
                
                //! returns the usage of this array
                //! \return the usage of this array
                virtual uint32_t getUsage() const;
                //! returns the vertex format
                virtual const renderdevice::VertexFormat& getVertexFormat() const;
                //! returns the vertex stride
                virtual size_t getVertexStride() const;
                //! returns the number of vertices
                //! \return the number of vertices
                virtual size_t getNumVertices() const;
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