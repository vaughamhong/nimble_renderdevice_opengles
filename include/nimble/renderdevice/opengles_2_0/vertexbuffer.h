//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_vertexbuffer_h__
#define __nimble_renderdevice_opengles_2_0_vertexbuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice-opengles_2_0/arraybuffer.h>
#include <nimble/renderdevice/ivertexbuffer.h>
#include <nimble/core/ilockable.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            class RenderDevice;
                        
            //! Vertex array impl
            class VertexBuffer
            : public renderdevice::IVertexBuffer
            , public core::Lockable
            , public resource::Resource{
            friend class nimble::renderdevice::opengles_2_0::RenderDevice;
            private:

                renderdevice::opengles_2_0::ArrayBuffer m_arrayBuffer;
                renderdevice::VertexFormat              m_vertexFormat;
                
            public:

                //! a constructor
                //! \param format the vertex format
                //! \param numVertices the number of vertices in this buffer
                //! \param usage the usage hints for this buffer
                VertexBuffer(renderdevice::VertexFormat const &vertexFormat, core::UInt32 numVertices, core::UInt32 usage);
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
                virtual core::UInt32 getUsage() const;
                //! returns the vertex format
                virtual const VertexFormat& getVertexFormat() const;
                //! returns the vertex stride
                virtual core::UInt32 getVertexStride() const;
                //! returns the number of vertices
                //! \return the number of vertices
                virtual core::UInt32 getNumVertices() const;
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