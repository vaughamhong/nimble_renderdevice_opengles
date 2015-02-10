//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/shader.h>
#include <nimble/core/debug.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
Shader::Shader(renderdevice::IShader::eType type)
:m_shaderHandle(0)
,m_type(type){
}
//! Destructor
Shader::~Shader(){
    if(m_shaderHandle){
        glDeleteShader(m_shaderHandle);
    }
}

//////////////////////////////////////////////////////////////////////////

//! Get shader type
renderdevice::IShader::eType Shader::getShaderType() const{
    return m_type;
}

//////////////////////////////////////////////////////////////////////////

//! compiles the shader with source data
//! \param data the data to copy
bool Shader::compileWithSource(const char *pData){
    const char *sharedNimbleShaderSource = "\
    const int NIMBLE_NUMLIGHTS = 4; \n\
    const int NIMBLE_NUMTEXTURES = 4; \n\
    #define NIMBLE_GLES \n";
    std::string completeSource = std::string(sharedNimbleShaderSource) + std::string(pData);
    const GLchar *source = (const GLchar*)completeSource.c_str();
    
    // create / compile our shader source
    if(getShaderType() == renderdevice::IShader::kTypePixel){
        m_shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    }else if(getShaderType() == renderdevice::IShader::kTypeVertex){
        m_shaderHandle = glCreateShader(GL_VERTEX_SHADER);
    }else{
        core::logError("graphics", "Invalid shader type detected");
    }
    glShaderSource(m_shaderHandle, 1, &source, NULL);
    glCompileShader(m_shaderHandle);
    
    // check for errors
    GLint status;
    glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE){
        GLint maxLogLength = 0;
        glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &maxLogLength);
        if(maxLogLength > 0){
            char logbuffer[maxLogLength];
            GLint loglen = 0;
            glGetShaderInfoLog(m_shaderHandle, (GLsizei)sizeof(logbuffer), &loglen, logbuffer);
            core::logWarning("graphics", "Shader compile log:\n%.*s", loglen, logbuffer);
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