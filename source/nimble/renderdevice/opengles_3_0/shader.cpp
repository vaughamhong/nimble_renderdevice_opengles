//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/shader.h>
#include <nimble/core/assert.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
Shader::Shader(renderdevice::eShaderType type)
:m_shaderHandle(0)
,m_type(type){
}
//! Destructor
Shader::~Shader(){
    if(m_shaderHandle){
        GLDEBUG(glDeleteShader(m_shaderHandle));
    }
}

//////////////////////////////////////////////////////////////////////////

//! Get shader type
renderdevice::eShaderType Shader::getShaderType() const{
    return m_type;
}

//////////////////////////////////////////////////////////////////////////

//! compiles the shader with source data
//! \param data the data to copy
bool Shader::compileWithSource(const char *pData){
    const char *sharedNimbleShaderSource = "\
    const int NIMBLE_NUMLIGHTS = 4; \n\
    const int NIMBLE_NUMTEXTURES = 4; \n";
    std::string completeSource = std::string(sharedNimbleShaderSource) + std::string(pData);
    const GLchar *source = (const GLchar*)completeSource.c_str();
    
    // create / compile our shader source
    if(getShaderType() == renderdevice::kShaderTypePixel){
        m_shaderHandle = GLDEBUG(glCreateShader(GL_FRAGMENT_SHADER));
    }else if(getShaderType() == renderdevice::kShaderTypeVertex){
        m_shaderHandle = GLDEBUG(glCreateShader(GL_VERTEX_SHADER));
    }else{
        NIMBLE_LOG_ERROR("graphics", "Invalid shader type detected");
    }
    GLDEBUG(glShaderSource(m_shaderHandle, 1, &source, 0));
    GLDEBUG(glCompileShader(m_shaderHandle));
    
    // check for errors
    GLint status;
    GLDEBUG(glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &status));
    if(status == GL_FALSE){
        GLint maxLogLength = 0;
        GLDEBUG(glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &maxLogLength));
        if(maxLogLength > 0){
            char *pLogBuffer = (char*)malloc(maxLogLength);
            GLint loglen = 0;
            GLDEBUG(glGetShaderInfoLog(m_shaderHandle, (GLsizei)maxLogLength, &loglen, pLogBuffer));
            NIMBLE_LOG_WARNING("graphics", "Shader compile log:\n%.*s", loglen, pLogBuffer);
            free(pLogBuffer);
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////

//! Returns the shader handle
GLuint Shader::getShaderHandle() const{
    return m_shaderHandle;
}

//////////////////////////////////////////////////////////////////////////