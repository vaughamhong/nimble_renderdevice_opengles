//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/renderdevice.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

void RenderDevice::setGLRenderStateDepthFunc(renderdevice::IRenderDevice::eDepthFunc func){
    unsigned int glDepthEnum = 0;
	if(func == RenderDevice::kDepthFuncNever){glDepthEnum |= GL_NEVER;}
	if(func == RenderDevice::kDepthFuncLessEqual){glDepthEnum |= GL_LEQUAL;}
    if(func == RenderDevice::kDepthFuncLess){glDepthEnum |= GL_LESS;}
    if(func == RenderDevice::kDepthFuncEqual){glDepthEnum |= GL_EQUAL;}
    
    if(glDepthEnum != 0){
        glDepthFunc(glDepthEnum);
    }
}
void RenderDevice::setGLRenderStateStencilFunc(renderdevice::IRenderDevice::eStencilFunc func){
    // TODO
}
void RenderDevice::setGLRenderStateCullFace(renderdevice::IRenderDevice::eCullFace face){
	unsigned int glCullFaceEnum = GL_FRONT_AND_BACK;
	if(face == RenderDevice::kCullFaceFront){glCullFaceEnum = GL_FRONT;}
	if(face == RenderDevice::kCullFaceBack){glCullFaceEnum = GL_BACK;}
    glCullFace(glCullFaceEnum);
}
//! sets the render state blend func
void RenderDevice::setGLRenderStateBlendFunc(renderdevice::IRenderDevice::eBlendScaler srcScaler, renderdevice::IRenderDevice::eBlendScaler destScaler){
    unsigned int glSrcScaler = GL_SRC_ALPHA;
    unsigned int glDestScaler = GL_ONE;
    
    if(srcScaler == RenderDevice::kBlendScalerSrcAlpha){glSrcScaler = GL_SRC_ALPHA;}
	if(srcScaler == RenderDevice::kBlendScalerOneMinusSrcAlpha){glSrcScaler = GL_ONE_MINUS_SRC_ALPHA;}
    if(srcScaler == RenderDevice::kBlendScalerOne){glSrcScaler = GL_ONE;}
    if(srcScaler == RenderDevice::kBlendScalerZero){glSrcScaler = GL_ZERO;}
    
    if(destScaler == RenderDevice::kBlendScalerSrcAlpha){glDestScaler = GL_SRC_ALPHA;}
	if(destScaler == RenderDevice::kBlendScalerOneMinusSrcAlpha){glDestScaler = GL_ONE_MINUS_SRC_ALPHA;}
    if(destScaler == RenderDevice::kBlendScalerOne){glDestScaler = GL_ONE;}
    if(destScaler == RenderDevice::kBlendScalerZero){glDestScaler = GL_ZERO;}
    
    glBlendFunc(glSrcScaler, glDestScaler);
}
void RenderDevice::setGLRenderStateFrontFaceOrientation(renderdevice::IRenderDevice::eOrientation orientation){
    unsigned int glFrontFaceOrientationEnum = 0;
	if(orientation == RenderDevice::kOrientationCounterClockwise){glFrontFaceOrientationEnum |= GL_CCW;}
	if(orientation == RenderDevice::kOrientationClockwise){glFrontFaceOrientationEnum |= GL_CW;}
	if(glFrontFaceOrientationEnum != 0){
		glFrontFace(glFrontFaceOrientationEnum);
	}else{
        assert(false);
    }
}
void RenderDevice::setGLRenderStateMatrixMode(nimble::renderdevice::IRenderDevice::eMatrixMode matrixMode){
    if(m_context.m_currentMatrixMode != matrixMode){
        m_context.m_currentMatrixMode = matrixMode;
    }
}

//////////////////////////////////////////////////////////////////////////

void RenderDevice::setGLRenderStateDepthWrite(bool enabled){
    if(enabled){
		glDepthMask(GL_TRUE);
	}else{
		glDepthMask(GL_FALSE);
	}
}
void RenderDevice::setGLRenderStateDepthTest(bool enabled){
    if(enabled){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}
}
void RenderDevice::setGLRenderStateStencilTest(bool enabled){
    if(enabled){
		glEnable(GL_STENCIL_TEST);
	}else{
		glDisable(GL_STENCIL_TEST);
	}
}
void RenderDevice::setGLRenderStateCullTest(bool enabled){
    if(enabled){
        glEnable(GL_CULL_FACE);
    }else{
        glDisable(GL_CULL_FACE);
    }
}
//! sets the blend state
void RenderDevice::setGLRenderStateBlending(bool enabled){
    if(enabled){
        glEnable(GL_BLEND);
    }else{
        glDisable(GL_BLEND);
    }
}
void RenderDevice::setGLRenderStateLighting(bool enabled){
    assert(false);
//	if(enabled){
//		glEnable(GL_LIGHTING);
//	}else{
//		glDisable(GL_LIGHTING);
//	}
}
void RenderDevice::setGLRenderStateTexturing(bool enabled){
	if(enabled){
		//OpenGL ES 2.0 does not support GL_TEXTURE_1D / GL_TEXTURE_3D
		glEnable(GL_TEXTURE_2D);
	}else{
		//OpenGL ES 2.0 does not support GL_TEXTURE_1D / GL_TEXTURE_3D
		glDisable(GL_TEXTURE_2D);
	}
}

//////////////////////////////////////////////////////////////////////////

//! Depth functions
//! \param func the depth compare mode
void RenderDevice::setDepthFunc(renderdevice::IRenderDevice::eDepthFunc func){
    setGLRenderStateDepthFunc(func);
}
//! Stencil functions
//! \param func the stencil compare mode
void RenderDevice::setStencilFunc(renderdevice::IRenderDevice::eStencilFunc func){
    setGLRenderStateStencilFunc(func);
}
//! Cull functions
//! \param mode the cull mode to set
void RenderDevice::setCullFace(renderdevice::IRenderDevice::eCullFace face){
    setGLRenderStateCullFace(face);
}
//! Blend function
//! \param srcScaler the src scaler component
//! \param destScaler the dest scaler component
void RenderDevice::setBlendFunc(renderdevice::IRenderDevice::eBlendScaler srcScaler, renderdevice::IRenderDevice::eBlendScaler destScaler){
    setGLRenderStateBlendFunc(srcScaler, destScaler);
}
//! Front face orientation function
//! \param orientation the face orientation
void RenderDevice::setFrontFaceOrientation(renderdevice::IRenderDevice::eOrientation orientation){
    setGLRenderStateFrontFaceOrientation(orientation);
}

//////////////////////////////////////////////////////////////////////////

//! Enable function
//! \param enables a state
void RenderDevice::enableState(renderdevice::IRenderDevice::eState state){
    if(m_context.m_renderStates[state] == false){
        switch(state){
            case RenderDevice::kStateDepthWrite:
                setGLRenderStateDepthWrite(true);
                break;
            case RenderDevice::kStateDepthTest:
                setGLRenderStateDepthTest(true);
                break;
            case RenderDevice::kStateStencilTest:
                setGLRenderStateStencilTest(true);
                break;
            case RenderDevice::kStateCullTest:
                setGLRenderStateCullTest(true);
                break;
            case RenderDevice::kStateBlending:
                setGLRenderStateBlending(true);
                break;
            case RenderDevice::kStateLighting:
                setGLRenderStateLighting(true);
                break;
            case RenderDevice::kStateTexturing:
                setGLRenderStateTexturing(true);
                break;
            default:
                assert(false);
                break;
        };
        m_context.m_renderStates[state] = true;
    }
}
//! Disable function
//! \param disables a state
void RenderDevice::disableState(renderdevice::IRenderDevice::eState state){
    if(m_context.m_renderStates[state] == true){
        switch(state){
            case RenderDevice::kStateDepthWrite:
                setGLRenderStateDepthWrite(false);
                break;
            case RenderDevice::kStateDepthTest:
                setGLRenderStateDepthTest(false);
                break;
            case RenderDevice::kStateStencilTest:
                setGLRenderStateStencilTest(false);
                break;
            case RenderDevice::kStateCullTest:
                setGLRenderStateCullTest(false);
                break;
            case RenderDevice::kStateBlending:
                setGLRenderStateBlending(false);
                break;
            case RenderDevice::kStateLighting:
                setGLRenderStateLighting(false);
                break;
            case RenderDevice::kStateTexturing:
                setGLRenderStateTexturing(false);
                break;
            default:
                assert(false);
                break;
        };
        m_context.m_renderStates[state] = false;
    }
}

//////////////////////////////////////////////////////////////////////////