//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_2_0/shaderprogram.h>
#include <nimble/renderdevice/opengles_2_0/shader.h>
#include <nimble/core/debug.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
ShaderProgram::ShaderProgram()
:m_pVertexShader(NULL)
,m_pPixelShader(NULL)
,m_programHandle(-1){
    m_programHandle = glCreateProgram();
    for(unsigned int i = 0; i < renderdevice::IShaderProgram::kMaxAttributeTypes; i++){
        m_uniformHandles[i] = -1;
    }
}
//! Destructor
ShaderProgram::~ShaderProgram(){
    if(m_programHandle){
        if(m_pVertexShader != NULL){
            glDetachShader(m_programHandle, m_pVertexShader->getShaderHandle());
        }
        if(m_pPixelShader != NULL){
            glDetachShader(m_programHandle, m_pPixelShader->getShaderHandle());
        }
        glDeleteProgram(m_programHandle);
        m_programHandle = 0;
    }
}

//////////////////////////////////////////////////////////////////////////

//! compiles this program
void ShaderProgram::compile(renderdevice::IShader *pVertexShader, renderdevice::IShader *pPixelShader){
    // clean up previous shaders
    if(m_pVertexShader != NULL){
        glDetachShader(m_programHandle, m_pVertexShader->getShaderHandle());
        m_pVertexShader = NULL;
    }
    if(m_pPixelShader != NULL){
        glDetachShader(m_programHandle, m_pPixelShader->getShaderHandle());
        m_pPixelShader = NULL;
    }
    
    // setup for compile
    m_pVertexShader = dynamic_cast<renderdevice::opengles_2_0::Shader*>(pVertexShader);
    m_pPixelShader = dynamic_cast<renderdevice::opengles_2_0::Shader*>(pPixelShader);
    if(!m_pVertexShader || (m_pVertexShader->getShaderType() != renderdevice::IShader::kTypeVertex)){
        core::logger_error("graphics", "Program compiling with an invalid vertex shader");
        return;
    }
    if(!m_pPixelShader || (m_pPixelShader->getShaderType() != renderdevice::IShader::kTypePixel)){
        core::logger_error("graphics", "Program compiling with an invalid pixel shader");
        return;
    }
    
    // compile
    glAttachShader(m_programHandle, m_pVertexShader->getShaderHandle());
    glAttachShader(m_programHandle, m_pPixelShader->getShaderHandle());
    glLinkProgram(m_programHandle);
    
    // check for errors
    GLint status;
    glGetProgramiv(m_programHandle, GL_LINK_STATUS, &status);
    if(status == GL_FALSE){
        GLint maxLogLength = 0;
        glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &maxLogLength);
        if(maxLogLength > 0){
            char logbuffer[maxLogLength];
            GLint loglen = 0;
            glGetProgramInfoLog(m_programHandle, (GLsizei)sizeof(logbuffer), &loglen, logbuffer);
            core::logger_error("graphics", "Shader program compile log:\n%.*s", loglen, logbuffer);
        }else{
            core::logger_error("graphics", "Shader program compile general failure...");
        }
    }
}

//! sets a param
void ShaderProgram::setParam(const char *name, renderdevice::IShaderProgram::eAttributeType type, void* pData){
    if(!m_programHandle){
        core::logWarning("graphics", "Trying to bind param \"%s\" to uninitialized program", name);
        return;
    }
        
    GLint uniformHandle = -1;
    if(m_uniformHandles[type] == -1){
        m_uniformHandles[type] = glGetUniformLocation(m_programHandle, name);
    }
    uniformHandle = m_uniformHandles[type];
    if(uniformHandle >= 0){
        switch(type){
            case renderdevice::IShaderProgram::kTypeFloat1:
                glUniform1fv(uniformHandle, 1, (GLfloat*)pData);
                break;
            case renderdevice::IShaderProgram::kTypeFloat2:
                glUniform2fv(uniformHandle, 1, (GLfloat*)pData);
                break;
            case renderdevice::IShaderProgram::kTypeFloat3:
                glUniform3fv(uniformHandle, 1, (GLfloat*)pData);
                break;
            case renderdevice::IShaderProgram::kTypeFloat4:
                glUniform4fv(uniformHandle, 1, (GLfloat*)pData);
                break;
                
            case renderdevice::IShaderProgram::TYPE_MATRIX2F:
                glUniformMatrix2fv(uniformHandle, 1, 0, (GLfloat*)pData);
                break;
            case renderdevice::IShaderProgram::TYPE_MATRIX3F:
                glUniformMatrix3fv(uniformHandle, 1, 0, (GLfloat*)pData);
                break;
            case renderdevice::IShaderProgram::TYPE_MATRIX4F:
                glUniformMatrix4fv(uniformHandle, 1, 0, (GLfloat*)pData);
                break;
                
            default:
                core::assert_error(false);
                break;
        };
    }
}

//////////////////////////////////////////////////////////////////////////

//! Returns the shader program handle
GLuint ShaderProgram::getShaderProgramHandle() const{
    return m_programHandle;
}

//////////////////////////////////////////////////////////////////////////