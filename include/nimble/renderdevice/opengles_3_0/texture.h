//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_texture_h__
#define __nimble_renderdevice_opengles_3_0_texture_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_3_0/common.h>
#include <nimble/renderdevice/opengles_3_0/arraybuffer.h>
#include <nimble/renderdevice/itexture.h>
#include <nimble/renderdevice/irenderdevice.h>
#include <nimble/core/Lockable.h>
#include <nimble/core/buffer.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

#if !defined(NIMBLE_TARGET_ANDROID)
#define GLES_TEXTURE_TUPLE_SET \
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_UNSIGNED_BYTE,       sizeof(char) * 3) \
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_UNSIGNED_SHORT,      sizeof(short) * 3) \
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_FLOAT,               sizeof(float) * 3) \
\
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_BYTE,       sizeof(char) * 4) \
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_SHORT,      sizeof(short) * 4) \
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_FLOAT,               sizeof(float) * 4) \
\
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB8,                    GL_UNSIGNED_BYTE,       sizeof(char) * 3) \
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_BYTE,       sizeof(char) * 4) \
\
GLES_TEXTURE_TUPLE(GL_DEPTH_COMPONENT16,      GL_DEPTH_COMPONENT16,       GL_UNSIGNED_SHORT,      sizeof(short) * 3)
#else
#define GLES_TEXTURE_TUPLE_SET \
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_UNSIGNED_BYTE,       sizeof(char) * 3) \
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_UNSIGNED_SHORT,      sizeof(short) * 3) \
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB,                     GL_FLOAT,               sizeof(float) * 3) \
\
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_BYTE,       sizeof(char) * 4) \
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_SHORT,      sizeof(short) * 4) \
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_FLOAT,               sizeof(float) * 4) \
\
GLES_TEXTURE_TUPLE(GL_RGB,                    GL_RGB8,                    GL_UNSIGNED_BYTE,       sizeof(char) * 3) \
GLES_TEXTURE_TUPLE(GL_RGBA,                   GL_RGBA,                    GL_UNSIGNED_BYTE,       sizeof(char) * 4) \
\
GLES_TEXTURE_TUPLE(GL_DEPTH_COMPONENT16,      GL_DEPTH_COMPONENT16,       GL_UNSIGNED_SHORT,      sizeof(short) * 3)
#endif

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{
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
                
                renderdevice::opengles_3_0::ArrayBuffer m_arrayBuffer;
                GLuint                                  m_textureHandle;
                bool                                    m_created;
                
                renderdevice::eTextureFilterMode    m_filterMode[renderdevice::kMaxTextureFilterParams];
                renderdevice::eTextureWrapMode      m_wrapMode[renderdevice::kMaxTextureWrapParams];
                
                uint32_t                            m_dimensionValues[renderdevice::kMaxTextureDimentions];
                renderdevice::eTextureDimension     m_dimension;
                renderdevice::eTextureFormat        m_textureFormat;
                
            public:
                
                //! Constructor
                Texture(uint32_t width, uint32_t height, renderdevice::eTextureFormat textureFormat, uint32_t usage);
                //! Constructor
                Texture(image::Image& image, uint32_t usage);
                //! copy constructor
                Texture(Texture& texture);
                //! Destructor
                virtual ~Texture();
                
            public:
                
                //! returns the usage
                //! \return the usage
                virtual uint32_t getUsage() const;
                //! gets the format of this texture
                virtual renderdevice::eTextureFormat getFormat() const;
                //! gets the number of dimensions of this texture
                virtual renderdevice::eTextureDimension getDimension() const;
                //! gets the width of the texture
                virtual uint32_t getWidth() const;
                //! gets the height of the texture
                virtual uint32_t getHeight() const;
                //! gets the depth of the texture
                virtual uint32_t getDepth() const;
                //! gets the bytes per pixel
                virtual uint32_t getBytesPerPixel() const;
                //! gets the texture size in bytes
                virtual uint32_t getBufferSize() const;
                
                //! gets the texture's sort index
                virtual uint32_t getSortIndex() const;
                
                //! sets texture filter mode
                //! \param param the texture filter parameter we are interested in changing
                //! \param value the texture filter value
                virtual void setFilterMode(renderdevice::eTextureFilterParameter param, renderdevice::eTextureFilterMode value);
                //! sets texture wrap mode
                //! \param param the texture wrap parameter we are interested in changing
                //! \param value the texture wrap value
                virtual void setWrapMode(renderdevice::eTextureWrapParameter param, renderdevice::eTextureWrapMode value);
                
                //! gets the texture filter mode
                //! \param param the filter parameter
                virtual renderdevice::eTextureFilterMode getFilterMode(renderdevice::eTextureFilterParameter param) const;
                //! gets the texture wrap mode
                //! \param param the wrap parameeter
                virtual renderdevice::eTextureWrapMode getWrapMode(renderdevice::eTextureWrapParameter param) const;
                
                //! regenerates mipmaps
                virtual void regenerateMipMaps();
                
                //! gets the GL handle
                virtual GLuint getTextureHandle() const;
                
            private:
                
                //!	creates buffers
                void createBuffers();
                //!	destroys buffers
                void destroyBuffers();
                
            protected:
                
                //! attempts to map a buffer
                virtual char* mapBuffer(core::eLockType lockType);
                //! attempts to map a buffer
                virtual char* mapBufferRange(core::eLockType lockType, uint32_t offset, uint32_t size);
                //! attempts to unmap a buffer
                virtual void unmapBuffer();
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////