//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/renderdevice.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

void RenderDevice::setGLRenderStateColorWrite(bool enabled){
    if(enabled){
        GLDEBUG(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
    }else{
        GLDEBUG(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
    }
}
void RenderDevice::setGLRenderStateDepthWrite(bool enabled){
    if(enabled){
        GLDEBUG(glDepthMask(GL_TRUE));
    }else{
        GLDEBUG(glDepthMask(GL_FALSE));
    }
}
void RenderDevice::setGLRenderStateStencilWrite(bool enabled){
    if(enabled){
        GLDEBUG(glStencilMask(0xFF));
    }else{
        GLDEBUG(glStencilMask(0x00));
    }
}
void RenderDevice::setGLRenderStateDepthTest(bool enabled){
    if(enabled){
        GLDEBUG(glEnable(GL_DEPTH_TEST));
    }else{
        GLDEBUG(glDisable(GL_DEPTH_TEST));
    }
}
void RenderDevice::setGLRenderStateStencilTest(bool enabled){
    if(enabled){
        GLDEBUG(glEnable(GL_STENCIL_TEST));
    }else{
        GLDEBUG(glDisable(GL_STENCIL_TEST));
    }
}
void RenderDevice::setGLRenderStateCullTest(bool enabled){
    if(enabled){
        GLDEBUG(glEnable(GL_CULL_FACE));
    }else{
        GLDEBUG(glDisable(GL_CULL_FACE));
    }
}
//! sets the blend state
void RenderDevice::setGLRenderStateBlending(bool enabled){
    if(enabled){
        GLDEBUG(glEnable(GL_BLEND));
    }else{
        GLDEBUG(glDisable(GL_BLEND));
    }
}
void RenderDevice::setGLRenderStateLighting(bool enabled){
    core::logger_warning("graphics", "Failed to set lighting state - unsupported");
}
void RenderDevice::setGLRenderStateTexturing(bool enabled){
    if(enabled){
        GLDEBUG(glEnable(GL_TEXTURE_2D));
    }else{
        GLDEBUG(glDisable(GL_TEXTURE_2D));
    }
}

//////////////////////////////////////////////////////////////////////////

//! Fill mode functions
//! \param face the face to set fill mode
//! \param mode the fill mode
void RenderDevice::setFillMode(renderdevice::eRenderFace face, renderdevice::eRenderFillMode mode){
    core::logger_warning("graphics", "Failed to set fill mode - unsupported");
}
//! Depth functions
//! \param func the depth compare mode
void RenderDevice::setDepthFunc(renderdevice::eRenderDepthFunc func){
    unsigned int glDepthEnum = 0;
    if(func == renderdevice::kRenderDepthFuncNever){glDepthEnum |= GL_NEVER;}
    if(func == renderdevice::kRenderDepthFuncAlways){glDepthEnum |= GL_ALWAYS;}
    if(func == renderdevice::kRenderDepthFuncLessEqual){glDepthEnum |= GL_LEQUAL;}
    if(func == renderdevice::kRenderDepthFuncLess){glDepthEnum |= GL_LESS;}
    if(func == renderdevice::kRenderDepthFuncGreaterEqual){glDepthEnum |= GL_GEQUAL;}
    if(func == renderdevice::kRenderDepthFuncGreater){glDepthEnum |= GL_GREATER;}
    if(func == renderdevice::kRenderDepthFuncEqual){glDepthEnum |= GL_EQUAL;}
    
    NIMBLE_ASSERT(glDepthEnum != 0);
    GLDEBUG(glDepthFunc(glDepthEnum));
}
//! Stencil functions
//! \param func the stencil compare mode
void RenderDevice::setStencilFunc(renderdevice::eRenderStencilFunc func, int32_t ref, uint32_t mask){
    unsigned int glStencilEnum = 0;
    if(func == renderdevice::kRenderStencilFuncNever){glStencilEnum = GL_NEVER;}
    if(func == renderdevice::kRenderStencilFuncAlways){glStencilEnum = GL_ALWAYS;}
    if(func == renderdevice::kRenderStencilFuncLessEqual){glStencilEnum = GL_LEQUAL;}
    if(func == renderdevice::kRenderStencilFuncLess){glStencilEnum = GL_LESS;}
    if(func == renderdevice::kRenderStencilFuncGreaterEqual){glStencilEnum = GL_GEQUAL;}
    if(func == renderdevice::kRenderStencilFuncGreater){glStencilEnum = GL_GREATER;}
    if(func == renderdevice::kRenderStencilFuncEqual){glStencilEnum = GL_EQUAL;}
    if(func == renderdevice::kRenderStencilFuncNotEqual){glStencilEnum = GL_NOTEQUAL;}
    
    NIMBLE_ASSERT(glStencilEnum != 0);
    GLDEBUG(glStencilFunc(glStencilEnum, ref, mask));
}
//! Stencil operation
//! \param sfail action taken when stencil pass fails
//! \param dfail action taken when depth pass fails
//! \param pass action taken when stencil and depth pass succeed
void RenderDevice::setStencilOp(renderdevice::eRenderStencilOp sfail, renderdevice::eRenderStencilOp dfail, renderdevice::eRenderStencilOp pass){
    unsigned int glStencilFailOp = GL_KEEP;
    if(sfail == renderdevice::kRenderStencilOpKeep){glStencilFailOp = GL_KEEP;}
    if(sfail == renderdevice::kRenderStencilOpZero){glStencilFailOp = GL_ZERO;}
    if(sfail == renderdevice::kRenderStencilOpReplace){glStencilFailOp = GL_REPLACE;}
    if(sfail == renderdevice::kRenderStencilOpIncr){glStencilFailOp = GL_INCR;}
    if(sfail == renderdevice::kRenderStencilOpIncrWrap){glStencilFailOp = GL_INCR_WRAP;}
    if(sfail == renderdevice::kRenderStencilOpDecr){glStencilFailOp = GL_DECR;}
    if(sfail == renderdevice::kRenderStencilOpDecrWrap){glStencilFailOp = GL_DECR_WRAP;}
    if(sfail == renderdevice::kRenderStencilOpInvert){glStencilFailOp = GL_INVERT;}
    
    unsigned int glDepthFailOp = GL_KEEP;
    if(dfail == renderdevice::kRenderStencilOpKeep){glDepthFailOp = GL_KEEP;}
    if(dfail == renderdevice::kRenderStencilOpZero){glDepthFailOp = GL_ZERO;}
    if(dfail == renderdevice::kRenderStencilOpReplace){glDepthFailOp = GL_REPLACE;}
    if(dfail == renderdevice::kRenderStencilOpIncr){glDepthFailOp = GL_INCR;}
    if(dfail == renderdevice::kRenderStencilOpIncrWrap){glDepthFailOp = GL_INCR_WRAP;}
    if(dfail == renderdevice::kRenderStencilOpDecr){glDepthFailOp = GL_DECR;}
    if(dfail == renderdevice::kRenderStencilOpDecrWrap){glDepthFailOp = GL_DECR_WRAP;}
    if(dfail == renderdevice::kRenderStencilOpInvert){glDepthFailOp = GL_INVERT;}
    
    unsigned int glPassOp = GL_KEEP;
    if(pass == renderdevice::kRenderStencilOpKeep){glPassOp = GL_KEEP;}
    if(pass == renderdevice::kRenderStencilOpZero){glPassOp = GL_ZERO;}
    if(pass == renderdevice::kRenderStencilOpReplace){glPassOp = GL_REPLACE;}
    if(pass == renderdevice::kRenderStencilOpIncr){glPassOp = GL_INCR;}
    if(pass == renderdevice::kRenderStencilOpIncrWrap){glPassOp = GL_INCR_WRAP;}
    if(pass == renderdevice::kRenderStencilOpDecr){glPassOp = GL_DECR;}
    if(pass == renderdevice::kRenderStencilOpDecrWrap){glPassOp = GL_DECR_WRAP;}
    if(pass == renderdevice::kRenderStencilOpInvert){glPassOp = GL_INVERT;}
    
    GLDEBUG(glStencilOp(glStencilFailOp, glDepthFailOp, glPassOp));
}
//! Stencil operation
//! \param mask stencil write mask
void RenderDevice::setStencilMask(uint32_t mask){
    GLDEBUG(glStencilMask(mask));
}
//! Cull functions
//! \param mode the cull mode to set
void RenderDevice::setCullFace(renderdevice::eRenderCullFace face){
    unsigned int glCullFaceEnum = GL_FRONT_AND_BACK;
    if(face == renderdevice::kRenderCullFaceFront){glCullFaceEnum = GL_FRONT;}
    if(face == renderdevice::kRenderCullFaceBack){glCullFaceEnum = GL_BACK;}
    
    NIMBLE_ASSERT(glCullFaceEnum != 0);
    GLDEBUG(glCullFace(glCullFaceEnum));
}
//! Blend function
//! \param srcScaler the src scaler component
//! \param destScaler the dest scaler component
void RenderDevice::setBlendFunc(renderdevice::eRenderBlendFactor srcScaler, renderdevice::eRenderBlendFactor destScaler){
    unsigned int glSrcScaler = GL_SRC_ALPHA;
    unsigned int glDestScaler = GL_ONE;
    
    if(srcScaler == renderdevice::kRenderBlendFactorSrcAlpha){glSrcScaler = GL_SRC_ALPHA;}
    if(srcScaler == renderdevice::kRenderBlendFactorOneMinusSrcAlpha){glSrcScaler = GL_ONE_MINUS_SRC_ALPHA;}
    if(srcScaler == renderdevice::kRenderBlendFactorOne){glSrcScaler = GL_ONE;}
    if(srcScaler == renderdevice::kRenderBlendFactorZero){glSrcScaler = GL_ZERO;}
    
    if(destScaler == renderdevice::kRenderBlendFactorSrcAlpha){glDestScaler = GL_SRC_ALPHA;}
    if(destScaler == renderdevice::kRenderBlendFactorOneMinusSrcAlpha){glDestScaler = GL_ONE_MINUS_SRC_ALPHA;}
    if(destScaler == renderdevice::kRenderBlendFactorOne){glDestScaler = GL_ONE;}
    if(destScaler == renderdevice::kRenderBlendFactorZero){glDestScaler = GL_ZERO;}
    
    NIMBLE_ASSERT(glSrcScaler != 0);
    NIMBLE_ASSERT(glDestScaler != 0);
    GLDEBUG(glBlendFunc(glSrcScaler, glDestScaler));
}
//! Front face orientation function
//! \param orientation the face orientation
void RenderDevice::setFrontFaceOrientation(renderdevice::eRenderOrientation orientation){
    unsigned int glFrontFaceOrientationEnum = 0;
    if(orientation == renderdevice::kRenderOrientationCounterClockwise){glFrontFaceOrientationEnum |= GL_CCW;}
    if(orientation == renderdevice::kRenderOrientationClockwise){glFrontFaceOrientationEnum |= GL_CW;}
    
    NIMBLE_ASSERT(glFrontFaceOrientationEnum != 0);
    GLDEBUG(glFrontFace(glFrontFaceOrientationEnum));
}

//////////////////////////////////////////////////////////////////////////

//! Enable function
//! \param enables a state
void RenderDevice::enableState(renderdevice::eRenderState state){
    switch(state){
        case renderdevice::kRenderStateColorWrite:
            setGLRenderStateColorWrite(true);
            break;
        case renderdevice::kRenderStateDepthWrite:
            setGLRenderStateDepthWrite(true);
            break;
        case renderdevice::kRenderStateStencilWrite:
            setGLRenderStateStencilWrite(true);
            break;
        case renderdevice::kRenderStateDepthTest:
            setGLRenderStateDepthTest(true);
            break;
        case renderdevice::kRenderStateStencilTest:
            setGLRenderStateStencilTest(true);
            break;
        case renderdevice::kRenderStateCullTest:
            setGLRenderStateCullTest(true);
            break;
        case renderdevice::kRenderStateBlending:
            setGLRenderStateBlending(true);
            break;
        case renderdevice::kRenderStateLighting:
            setGLRenderStateLighting(true);
            break;
        case renderdevice::kRenderStateTexturing:
            setGLRenderStateTexturing(true);
            break;
        default:
            assert(false);
            break;
    };
    m_context.m_renderStates[state] = true;
}
//! Disable function
//! \param disables a state
void RenderDevice::disableState(renderdevice::eRenderState state){
    switch(state){
        case renderdevice::kRenderStateColorWrite:
            setGLRenderStateColorWrite(false);
            break;
        case renderdevice::kRenderStateDepthWrite:
            setGLRenderStateDepthWrite(false);
            break;
        case renderdevice::kRenderStateStencilWrite:
            setGLRenderStateStencilWrite(false);
            break;
        case renderdevice::kRenderStateDepthTest:
            setGLRenderStateDepthTest(false);
            break;
        case renderdevice::kRenderStateStencilTest:
            setGLRenderStateStencilTest(false);
            break;
        case renderdevice::kRenderStateCullTest:
            setGLRenderStateCullTest(false);
            break;
        case renderdevice::kRenderStateBlending:
            setGLRenderStateBlending(false);
            break;
        case renderdevice::kRenderStateLighting:
            setGLRenderStateLighting(false);
            break;
        case renderdevice::kRenderStateTexturing:
            setGLRenderStateTexturing(false);
            break;
        default:
            assert(false);
            break;
    };
    m_context.m_renderStates[state] = false;
}

//////////////////////////////////////////////////////////////////////////