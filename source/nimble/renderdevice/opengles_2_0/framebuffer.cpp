//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/framebuffer.h>
#include <nimble/renderdevice-opengles_2_0/texture.h>
#include <nimble/core/debug.h>
#include <nimble/core/locator.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! a constructor
FrameBuffer::FrameBuffer(core::UInt32 width, core::UInt32 height)
:m_fboHandle(0)
,m_depthBufferHandle(0)
,m_width(width)
,m_height(height){
    // set up our frame buffer object
	glGenFramebuffers(1, &m_fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
    
    // set up depth buffer
    glGenRenderbuffers(1, &m_depthBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferHandle);
    
	// clear our texture array
	for(int i = 0; i < kMaxBufferTypes; i++){
		m_pBuffers[i] = 0;
	}
    
//    GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
//    if(status != GL_FRAMEBUFFER_COMPLETE_OES){
//        core::logError("graphics", "[renderdevice::framebuffer] frame buffer is incomplete");
//    }
}
//! a destructor
FrameBuffer::~FrameBuffer(){
    glDeleteRenderbuffers(1, &m_depthBufferHandle);
	glDeleteFramebuffers(1, &m_fboHandle);
}

//////////////////////////////////////////////////////////////////////////

//! attach a texture to a buffer
//! \param buffer the buffer to attach to
//! \param pTexture the texture to attach
//! \return true if successful
void FrameBuffer::attachTarget(renderdevice::IFrameBuffer::eBufferType bufferType, renderdevice::ITexture* pTexture){
    core::assert_error(pTexture->getDimension() == renderdevice::ITexture::kDimention2D);
    
    core::UInt32 width = pTexture->getWidth();
    core::UInt32 height = pTexture->getHeight();
    core::assert_error(width == this->getWidth() && height == this->getHeight());
    
    if(renderdevice::opengles_2_0::Texture* pNativeTexture = dynamic_cast<renderdevice::opengles_2_0::Texture*>(pTexture)){
        // bind buffer if not already done so
        GLint boundHandle;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundHandle);
        if(boundHandle != m_fboHandle){
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
        }
        
        // bind texture to buffer target
        if(bufferType == FrameBuffer::kBufferDepth){
            GLuint handle = pNativeTexture->getTextureHandle();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, handle, 0);
            m_pBuffers[FrameBuffer::kBufferColor0] = pTexture;
        }else if(bufferType == FrameBuffer::kBufferColor0){
            GLuint handle = pNativeTexture->getTextureHandle();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, handle, 0);
            m_pBuffers[FrameBuffer::kBufferColor0] = pTexture;
        }else{
            core::logError("graphics", "Failed to attach texture to invalid target");
        }
        
        // restore previous frame buffer
        if(boundHandle != m_fboHandle){
            glBindFramebuffer(GL_FRAMEBUFFER, boundHandle);
        }
    }else{
        core::logError("graphics", "Failed to attach invalid texture");
    }
}
//! detaches a texture from a buffer target
//! \param buffer the buffer to detach from
//! \return the texture that was detached
renderdevice::ITexture* FrameBuffer::detachTarget(renderdevice::IFrameBuffer::eBufferType buffer){
    renderdevice::ITexture *pTexture = m_pBuffers[buffer];
    
    if(dynamic_cast<renderdevice::opengles_2_0::Texture*>(pTexture)){
        // bind buffer if not already done so
        GLint boundHandle;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundHandle);
        if(boundHandle != m_fboHandle){
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
        }
        
        // unbind
        if(buffer == FrameBuffer::kBufferDepth){
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        }else if(buffer == FrameBuffer::kBufferColor0){
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        }else{
            core::logError("graphics", "Failed to detach invalid target");
        }
        
        // restore previous frame buffer
        if(boundHandle != m_fboHandle){
            glBindFramebuffer(GL_FRAMEBUFFER, boundHandle);
        }
        
        // return our bound texture
        m_pBuffers[buffer] = NULL;
        return pTexture;
    }else{
        core::logError("graphics", "Failed to detach invalid texture");
        return NULL;
    }
}

//////////////////////////////////////////////////////////////////////////

//! returns the render target
renderdevice::ITexture* FrameBuffer::getTarget(renderdevice::IFrameBuffer::eBufferType buffer){
    if(buffer == renderdevice::IFrameBuffer::kBufferDepth){return m_pBuffers[0];}
	if(buffer == renderdevice::IFrameBuffer::kBufferStencil){return m_pBuffers[1];}
	if(buffer == renderdevice::IFrameBuffer::kBufferAccumulation){return m_pBuffers[2];}
    if(buffer == renderdevice::IFrameBuffer::kBufferColor0){return m_pBuffers[3];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor1){return m_pBuffers[4];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor2){return m_pBuffers[5];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor3){return m_pBuffers[6];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor4){return m_pBuffers[7];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor5){return m_pBuffers[8];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor6){return m_pBuffers[9];}
	if(buffer == renderdevice::IFrameBuffer::kBufferColor7){return m_pBuffers[10];}
    return NULL;
}
//! returns the width
//! \return the width
unsigned int FrameBuffer::getWidth() const{
    return m_width;
}
//! returns the height
//! \return the height
unsigned int FrameBuffer::getHeight() const{
    return m_height;
}
//! returns the handle
GLuint FrameBuffer::getFrameBufferHandle(){
    return m_fboHandle;
}

//////////////////////////////////////////////////////////////////////////

//! discards depth buffer
void FrameBuffer::discardDepthBuffer(){
    renderdevice::IRenderDevice *pRenderDevice = core::Locator<renderdevice::IRenderDevice>::acquire();
    pRenderDevice->bindFrameBuffer(this, true);
    const GLenum discards[] = {GL_DEPTH_ATTACHMENT};
    glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, discards);
}

//////////////////////////////////////////////////////////////////////////