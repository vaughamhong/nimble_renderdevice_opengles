//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_1_1/framebuffer.h>
#include <nimble/renderdevice-opengles_1_1/texture.h>
#include <nimble/core/debug.h>
#include <nimble/core/locator.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_1_1;

//////////////////////////////////////////////////////////////////////////

//! a constructor
FrameBuffer::FrameBuffer(core::UInt32 width, core::UInt32 height)
:m_fboHandle(0)
,m_depthBufferHandle(0)
,m_width(width)
,m_height(height){
    // set up our frame buffer object
	glGenFramebuffersOES(1, &m_fboHandle);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_fboHandle);
    
    // set up depth buffer
    glGenRenderbuffersOES(1, &m_depthBufferHandle);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_depthBufferHandle);
    glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, width, height);
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, m_depthBufferHandle);
    
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
    glDeleteRenderbuffersOES(1, &m_depthBufferHandle);
	glDeleteFramebuffersOES(1, &m_fboHandle);
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
    
    if(renderdevice::opengles_1_1::Texture* pNativeTexture = dynamic_cast<renderdevice::opengles_1_1::Texture*>(pTexture)){
        // bind buffer if not already done so
        GLint boundFrameBufferHandle;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &boundFrameBufferHandle);
        if(boundFrameBufferHandle != m_fboHandle){
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_fboHandle);
        }
        
        // bind texture to buffer target
        if(bufferType == FrameBuffer::kBufferColor0){
            GLuint handle = pNativeTexture->getTextureHandle();
            glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, handle, 0);
            m_pBuffers[FrameBuffer::kBufferColor0] = pTexture;
        }else{
            core::logError("graphics", "Failed to attach texture to invalid target");
        }
        
        // restore previous frame buffer
        if(boundFrameBufferHandle != m_fboHandle){
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, boundFrameBufferHandle);
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
    
    if(dynamic_cast<renderdevice::opengles_1_1::Texture*>(pTexture)){
        // bind buffer if not already done so
        GLint boundFrameBufferHandle;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, &boundFrameBufferHandle);
        if(boundFrameBufferHandle != m_fboHandle){
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_fboHandle);
        }
        
        // unbind
        if(buffer == FrameBuffer::kBufferColor0){
            glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, 0, 0);
        }else{
            core::logError("graphics", "Failed to detach invalid target");
        }
        
        // restore previous frame buffer
        if(boundFrameBufferHandle != m_fboHandle){
            glBindFramebufferOES(GL_FRAMEBUFFER_OES, boundFrameBufferHandle);
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
    const GLenum discards[] = {GL_DEPTH_ATTACHMENT_OES};
    glDiscardFramebufferEXT(GL_FRAMEBUFFER_OES, 1, discards);
}

//////////////////////////////////////////////////////////////////////////