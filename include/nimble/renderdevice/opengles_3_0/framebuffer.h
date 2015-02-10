//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_framebuffer_h__
#define __nimble_renderdevice_opengles_3_0_framebuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_3_0/common.h>
#include <nimble/renderdevice/iframebuffer.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{

            //! Frame buffer impl
            class FrameBuffer
            : public renderdevice::IFrameBuffer
            , public resource::Resource{
            private:
                
                GLuint                      m_fboHandle;
                GLuint                      m_depthBufferHandle;
                GLuint                      m_stencilBufferHandle;
                renderdevice::ITexture*     m_pBuffers[kMaxFrameBufferTypes];
                
                uint32_t                    m_width;
                uint32_t                    m_height;
                uint32_t                    m_flags;
                
            public:
                
                //! a constructor
                FrameBuffer(uint32_t width, uint32_t height, uint32_t flags);
                //! a destructor
                virtual ~FrameBuffer();
                
            public:
                
                //! returns true if this frame buffer is complete
                virtual bool isComplete() const;
                
                //! attach a texture target
                //! \param type the type to attach
                //! \param pTexture the texture to attach
                virtual void attachTextureTarget(renderdevice::eFrameBufferType type, renderdevice::ITexture *pTexture);
                //! detaches a texture target
                //! \param type the type to detach
                virtual void detachTextureTarget(renderdevice::eFrameBufferType type);
                //! attach a set of texture targets
                //! \param types the types to attach
                //! \param pTextures the textures to attach
                //! \param numTargets the number of textures to attach
                virtual void attachTextureTargets(renderdevice::eFrameBufferType *types, renderdevice::ITexture **pTextures, size_t numTargets);
                //! detaches a set of texture targets
                //! \param types the types to detach
                //! \param numTargets the number of textures to detach
                virtual void detachTextureTargets(renderdevice::eFrameBufferType *types, size_t numTargets);
                //! sets writable targets
                virtual void setWriteTargets(uint32_t bufferMask);
                
                //! returns the render target
                virtual renderdevice::ITexture* getTarget(renderdevice::eFrameBufferType buffer);
                
                //! returns the width
                //! \return the width
                virtual uint32_t getWidth() const;
                //! returns the height
                //! \return the height
                virtual uint32_t getHeight() const;
                
                //! returns the handle
                virtual GLuint getFrameBufferHandle();
                
            public:
                
                //! discards depth buffer
                virtual void discardDepthBuffer();
            };

        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////