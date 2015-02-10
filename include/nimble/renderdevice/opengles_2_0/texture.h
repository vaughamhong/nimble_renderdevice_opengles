//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_texture_h__
#define __nimble_renderdevice_opengles_2_0_texture_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice-opengles_2_0/common.h>
#include <nimble/renderdevice/itexture.h>
#include <nimble/renderdevice/irenderdevice.h>
#include <nimble/core/Lockable.h>
#include <nimble/core/message.h>
#include <nimble/core/buffer.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

#define GLES20_TEXTURE_TUPLE_SET \
GLES20_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_UNSIGNED_BYTE,       sizeof(char) * 3) \
GLES20_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_UNSIGNED_SHORT,      sizeof(short) * 3) \
GLES20_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_FLOAT,               sizeof(float) * 3) \
\
GLES20_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_BYTE,       sizeof(char) * 4) \
GLES20_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_SHORT,      sizeof(short) * 4) \
GLES20_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_FLOAT,               sizeof(float) * 4) \
\
GLES20_TEXTURE_TUPLE(GL_BGRA,                   GL_RGBA,                    GL_UNSIGNED_BYTE,       sizeof(char) * 4) \
\
GLES20_TEXTURE_TUPLE(GL_DEPTH_COMPONENT16,      GL_DEPTH_COMPONENT16,       GL_UNSIGNED_SHORT,      sizeof(short) * 3)

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            class RenderDevice;

            //! OpenGLES Support details
            //! + doesn't support BGR pixel format
            //! + only support BGRA (input) pixel format under APPLE extension
            //! + does not support BGRA as an internal format (so we must map it to one that's supported, like RGBA)
            //! + does not support PBO
            
            //! Texture impl
            class Texture
            : public renderdevice::ITexture
            , public core::Lockable
            , public resource::Resource{
            private:

                GLuint              m_textureHandle;
                GLuint				m_pboHandle;
                
                core::UInt32        m_usage;
                core::Bool          m_created;

                eFilterMode			m_filterMode[renderdevice::ITexture::kMaxFilterParams];
                eWrapMode			m_wrapMode[renderdevice::ITexture::kMaxWrapParams];

                core::UInt32                        m_dimensionValues[renderdevice::ITexture::kMaxDimentions];
                renderdevice::ITexture::eDimension  m_dimension;
                renderdevice::ITexture::eFormat     m_textureFormat;
                
                core::Buffer        m_localBuffer;
                
            public:

                //! Constructor
                Texture(core::UInt32 size, renderdevice::ITexture::eFormat textureFormat, core::UInt32 usage);
                //! Constructor
                Texture(core::UInt32 width, core::UInt32 height, renderdevice::ITexture::eFormat textureFormat, core::UInt32 usage);
                //! Constructor
                Texture(core::UInt32 width, core::UInt32 height, core::UInt32 depth, renderdevice::ITexture::eFormat textureFormat, core::UInt32 usage);
                //! copy constructor
                Texture(Texture& texture);
                //! Destructor
                virtual ~Texture();
                
            public:

                //! returns the usage
                //! \return the usage
                virtual core::UInt32 getUsage() const;
                //! gets the format of this texture
                virtual renderdevice::ITexture::eFormat getFormat() const;
                //! gets the number of dimensions of this texture
                virtual renderdevice::ITexture::eDimension getDimension() const;
                //! gets the width of the texture
                virtual core::UInt32 getWidth() const;
                //! gets the height of the texture
                virtual core::UInt32 getHeight() const;
                //! gets the depth of the texture
                virtual core::UInt32 getDepth() const;
                //! gets the bytes per pixel
                virtual core::UInt32 getBytesPerPixel() const;
                //! gets the texture size in bytes
                virtual core::UInt32 getBufferSize() const;
                
                //! sets texture filter mode
                //! \param param the texture filter parameter we are interested in changing
                //! \param value the texture filter value
                virtual void setFilterMode(renderdevice::ITexture::eFilterParameter param, renderdevice::ITexture::eFilterMode value);
                //! sets texture wrap mode
                //! \param param the texture wrap parameter we are interested in changing
                //! \param value the texture wrap value
                virtual void setWrapMode(renderdevice::ITexture::eWrapParameter param, renderdevice::ITexture::eWrapMode value);

                //! gets the texture filter mode
                //! \param param the filter parameter
                virtual renderdevice::ITexture::eFilterMode getFilterMode(renderdevice::ITexture::eFilterParameter param) const;
                //! gets the texture wrap mode
                //! \param param the wrap parameeter
                virtual renderdevice::ITexture::eWrapMode getWrapMode(renderdevice::ITexture::eWrapParameter param) const;
                
                //! gets the GL handle
                virtual GLuint getTextureHandle() const;
                
            private:

                //!	creates buffers
                void createBuffers();
                //!	destroys buffers
                void destroyBuffers();
                
            protected:
                
                //! attempts to map a buffer
                virtual core::Int8* mapBuffer(eLockType lockType);
                //! attempts to map a buffer
                virtual core::Int8* mapBufferRange(eLockType lockType, core::UInt32 offset, core::UInt32 size);
                //! attempts to unmap a buffer
                virtual void unmapBuffer();
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////