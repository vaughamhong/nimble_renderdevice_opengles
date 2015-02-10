//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/shaderparam.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
ShaderParam::ShaderParam()
:renderdevice::ShaderParam()
,m_handle(-1)
,m_textureUnit(-1){
}
//! Constructor
ShaderParam::ShaderParam(const char *name, renderdevice::eShaderParamType type, void *pData, GLint handle, GLint textureUnit)
:renderdevice::ShaderParam(name, type, pData)
,m_handle(handle)
,m_textureUnit(textureUnit){
}
//! Constructor
ShaderParam::ShaderParam(ShaderParam const &param)
:renderdevice::ShaderParam(param)
,m_handle(param.m_handle)
,m_textureUnit(param.m_textureUnit){
}
//! Destructor
ShaderParam::~ShaderParam(){
}

//////////////////////////////////////////////////////////////////////////

//! Returns the shader handle
GLint ShaderParam::getHandle() const{
    return m_handle;
}
//! Returns the texture unit
GLint ShaderParam::getTextureUnit() const{
    return m_textureUnit;
}

//////////////////////////////////////////////////////////////////////////

//! maps opengl uniform types to internal types
renderdevice::eShaderParamType nimble::renderdevice::opengles_3_0::mapGLUniformTypeToInternalType(GLenum uniformType){
    if(uniformType == GL_FLOAT){
        return renderdevice::kTypeFloat1;
    }else if(uniformType == GL_FLOAT_VEC2){
        return renderdevice::kTypeFloat2;
    }else if(uniformType == GL_FLOAT_VEC3){
        return renderdevice::kTypeFloat3;
    }else if(uniformType == GL_FLOAT_VEC4){
        return renderdevice::kTypeFloat4;
    }else if(uniformType == GL_INT){
        return renderdevice::kTypeInt1;
    }else if(uniformType == GL_INT_VEC2){
        return renderdevice::kTypeInt2;
    }else if(uniformType == GL_INT_VEC3){
        return renderdevice::kTypeInt3;
    }else if(uniformType == GL_INT_VEC4){
        return renderdevice::kTypeInt4;
    }else if(uniformType == GL_BOOL){
        return renderdevice::kTypeBool1;
    }else if(uniformType == GL_BOOL_VEC2){
        return renderdevice::kTypeBool2;
    }else if(uniformType == GL_BOOL_VEC3){
        return renderdevice::kTypeBool3;
    }else if(uniformType == GL_BOOL_VEC4){
        return renderdevice::kTypeBool4;
    }else if(uniformType == GL_FLOAT_MAT2){
        return renderdevice::kTypeFloat2x2;
    }else if(uniformType == GL_FLOAT_MAT3){
        return renderdevice::kTypeFloat3x3;
    }else if(uniformType == GL_FLOAT_MAT4){
        return renderdevice::kTypeFloat4x4;
    }else if(uniformType == GL_SAMPLER_2D){
        return renderdevice::kTypeTexture2D;
    }else if(uniformType == GL_SAMPLER_CUBE){
        return renderdevice::kTypeTexture3D;
    }else{
        return renderdevice::kTypeNull;
    }
}

//////////////////////////////////////////////////////////////////////////