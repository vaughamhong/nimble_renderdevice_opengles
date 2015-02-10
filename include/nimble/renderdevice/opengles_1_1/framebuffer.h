//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_1_1_framebuffer_h__
#define __nimble_renderdevice_opengles_1_1_framebuffer_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice-opengles_1_1/common.h>
#include <nimble/renderdevice/iframebuffer.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_1_1{

            //! Frame buffer impl
            class FrameBuffer
            : public renderdevice::IFrameBuffer
            , public resource::Resource{
            private:

                GLuint                      m_fboHandle;
                GLuint                      m_depthBufferHandle;
                renderdevice::ITexture*     m_pBuffers[IFrameBuffer::kMaxBufferTypes];

                core::UInt32                m_width;
                core::UInt32                m_height;
                
            public:
                
                //! a constructor
                FrameBuffer(core::UInt32 width, core::UInt32 height);
                //! a destructor
                virtual ~FrameBuffer();

            public:
                
                //! attach a texture to a buffer
                //! \param buffer the buffer to attach to
                //! \param pTexture the texture to attach
                //! \return true if successful
                virtual void attachTarget(renderdevice::IFrameBuffer::eBufferType buffer, renderdevice::ITexture* pTexture);
                //! detaches a texture from a buffer target
                //! \param buffer the buffer to detach from
                //! \return the texture that was detached
                virtual renderdevice::ITexture* detachTarget(renderdevice::IFrameBuffer::eBufferType buffer);
                
                //! returns the render target
                virtual renderdevice::ITexture* getTarget(renderdevice::IFrameBuffer::eBufferType buffer);
                //! returns the width
                //! \return the width
                virtual core::UInt32 getWidth() const;
                //! returns the height
                //! \return the height
                virtual core::UInt32 getHeight() const;

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