//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/shaderprogram.h>
#include <nimble/renderdevice/opengles_3_0/shader.h>
#include <nimble/renderdevice/opengles_3_0/shaderparam.h>
#include <nimble/renderdevice/opengles_3_0/texture.h>
#include <nimble/core/assert.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

// TODO: Ideally shader program parameter sets will be tracked (cached) and only
// applied when all in one go. Batching uniform transfers along with uploading
// uniforms that have changed are sensible optimizations.

//////////////////////////////////////////////////////////////////////////

static const int kMaxUniformNameSize = 64;

//////////////////////////////////////////////////////////////////////////

//! Constructor
ShaderProgram::ShaderProgram()
:m_programHandle(0){
    m_programHandle = GLDEBUG(glCreateProgram());
	NIMBLE_ASSERT_PRINT(m_programHandle != 0, "Failed to create program handle");
}
//! Destructor
ShaderProgram::~ShaderProgram(){
	GLDEBUG(glDeleteProgram(m_programHandle));
	m_programHandle = 0;
}

//////////////////////////////////////////////////////////////////////////

//! gets the shader program's sort index
uint32_t ShaderProgram::getSortIndex() const{
    return m_programHandle;
}

//////////////////////////////////////////////////////////////////////////

//! compiles this program
//! \param[in] pVertexShader the vertex shader to compile
//! \param[in] pPixelShader the pixel shader to compile
void ShaderProgram::compile(renderdevice::IShader *pVertexShader, renderdevice::IShader *pPixelShader){
    // clear our parameter block
    m_paramBlock.clear();

    // setup for compile
    renderdevice::opengles_3_0::Shader *pNativeVertexShader = dynamic_cast<renderdevice::opengles_3_0::Shader*>(pVertexShader);
    renderdevice::opengles_3_0::Shader *pNativePixelShader = dynamic_cast<renderdevice::opengles_3_0::Shader*>(pPixelShader);
    if(!pNativeVertexShader || (pVertexShader->getShaderType() != renderdevice::kShaderTypeVertex)){
        NIMBLE_LOG_ERROR("graphics", "Program compiling with an invalid vertex shader");
        return;
    }
    if(!pNativePixelShader || (pPixelShader->getShaderType() != renderdevice::kShaderTypePixel)){
        NIMBLE_LOG_ERROR("graphics", "Program compiling with an invalid pixel shader");
        return;
    }
    
    // compile shaders into a program
    GLDEBUG(glAttachShader(m_programHandle, pNativeVertexShader->getShaderHandle()));
    GLDEBUG(glAttachShader(m_programHandle, pNativePixelShader->getShaderHandle()));
    GLDEBUG(glLinkProgram(m_programHandle));
    GLDEBUG(glDetachShader(m_programHandle, pNativeVertexShader->getShaderHandle()));
    GLDEBUG(glDetachShader(m_programHandle, pNativePixelShader->getShaderHandle()));
    
    // check for errors
    GLint status;
    GLDEBUG(glGetProgramiv(m_programHandle, GL_LINK_STATUS, &status));
    if(status == GL_FALSE){
        GLint maxLogLength = 0;
        GLDEBUG(glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &maxLogLength));
        if(maxLogLength > 0){
            char *pLogBuffer = (char*)malloc(maxLogLength);
            GLint loglen = 0;
			GLDEBUG(glGetProgramInfoLog(m_programHandle, (GLsizei)maxLogLength, &loglen, pLogBuffer));
            NIMBLE_LOG_ERROR("graphics", "Shader program compile log:\n%.*s", loglen, pLogBuffer);
			free(pLogBuffer);
        }else{
            NIMBLE_LOG_ERROR("graphics", "Shader program compile general failure...");
        }
        return;
    }
    
    // find the number of active uniforms in this program
    GLint numActiveUniforms;
    GLDEBUG(glGetProgramiv(m_programHandle, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    
    // extract information about individual uniform parameters and
    // stuff it into a local param block
    char uniformName[kMaxUniformNameSize];
    int nameLength, uniformSize;
    GLenum uniformType;
    GLint uniformHandle;
    GLint textureUnitCounter = 0;
    renderdevice::eShaderParamType paramType;
    NIMBLE_LOG_INFO("graphics", "numActiveUniforms: %d", numActiveUniforms);
    for(int i = 0; i < numActiveUniforms; i++){
        // retreive the uniform name (with null termination)
        GLDEBUG(glGetActiveUniform(m_programHandle,         //!< handle
                                   GLuint(i),               //!< index
                                   kMaxUniformNameSize - 1, //!< the maximum name buffer size (one less for null termination)
                                   &nameLength,             //!< the returned name length
                                   &uniformSize,            //!< the returned uniform data size
                                   &uniformType,            //!< the returned uniform type
                                   uniformName));            //!< the uniform buffer name
        uniformName[nameLength] = 0;
        NIMBLE_LOG_INFO("graphics", "uniformName: %s", uniformName);

        // map uniform type to internal type
        paramType = mapGLUniformTypeToInternalType(uniformType);
        if(paramType == renderdevice::kTypeNull){
            NIMBLE_LOG_ERROR("graphics", "Failed to map shader uniform type to internal type");
            continue;
        }
        
        // retreive the uniform handle
        uniformHandle = GLDEBUG(glGetUniformLocation(m_programHandle, uniformName));
        
        // create and add our parameter
        if((paramType == renderdevice::kTypeTexture2D) || (paramType == renderdevice::kTypeTexture3D)){
            m_paramBlock.addShaderParam(renderdevice::opengles_3_0::ShaderParam(uniformName, paramType, 0, uniformHandle, textureUnitCounter++));
        }else{
            m_paramBlock.addShaderParam(renderdevice::opengles_3_0::ShaderParam(uniformName, paramType, 0, uniformHandle));
        }
        
        NIMBLE_LOG_INFO("graphics", "uniformName: %s", uniformName);
    }
}

//////////////////////////////////////////////////////////////////////////

//! Returns true if param with name exists
//! \return true if param with name exists
bool ShaderProgram::existsParamWithName(const char *name) const{
    return m_paramBlock.existsParamWithName(name);
}
//! Returns the shader parameter by name
//! \param[in] name the name of the parameter to retreive
//! \return returns the shader parameter
renderdevice::IShaderParam const* ShaderProgram::getShaderParamByName(const char *name) const{
    return m_paramBlock.getShaderParamByName(name);
}
//! Returns the shader param block for this program
//! \return the shader param block for this program
renderdevice::IShaderParamBlock* ShaderProgram::createShaderParamBlock(const char *names[], size_t numNames){
    // note: once the shader program has been loaded an external caller can ask for a parameter block.
    // The caller should know which parameters exists in the shader program, and can call this method
    // to retreive the parameter block.
    
    // note: the shader program owns the allocation of the shader parameter block
    // this allows our shader program to return its own flavor of param block and
    // also make sure the param block is pooled.
    
    renderdevice::IShaderParamBlock *pBlock = 0;
    if(names == 0 || numNames == 0){
        // return block with all parameters
        pBlock = new /*()*/ renderdevice::opengles_3_0::ShaderParamBlock(m_paramBlock);
    }else{
        // return block with the selected parameters
        pBlock = new /*()*/ renderdevice::opengles_3_0::ShaderParamBlock();
        for(size_t i = 0; i < numNames; i++){
            const char *name = names[i];
            renderdevice::IShaderParam const *pParam = m_paramBlock.getShaderParamByName(name);
            if(pParam != 0){
                pBlock->addShaderParam(*pParam);
            }else{
                NIMBLE_LOG_WARNING("graphics", "Failed to find param with name \"%s\"", name);
            }
        }
    }
    return pBlock;
}

//////////////////////////////////////////////////////////////////////////

//! Sets a shader parameter by name
//! \param[in] name the name of the shader parameter to set
//! \param[in] pData the data to set the shader parameter
void ShaderProgram::setShaderParamByName(const char *name, void* pData){
    // try and find our shader parameter by name
    renderdevice::IShaderParam const *pShaderParam = this->getShaderParamByName(name);
    renderdevice::opengles_3_0::ShaderParam const *pNativeShaderParam = dynamic_cast<renderdevice::opengles_3_0::ShaderParam const*>(pShaderParam);
    if(pNativeShaderParam == 0){
        NIMBLE_LOG_WARNING("graphics", "Failed to set shader parameter with name \"%s\"", name);
        return;
    }
    
    // bind data
    setGLUniformParam(*pNativeShaderParam, pData);
    NIMBLE_LOG_INFO("graphics", "setShaderParamByName: %s value: %x", name, pData);
}
//! Sets a shader parameter block
//! \param[in] pBlock the shader parameter block to set
void ShaderProgram::setShaderParamBlock(renderdevice::IShaderParamBlock const &block){
    size_t numParameters = block.getNumShaderParams();
    for(size_t i = 0; i < numParameters; i++){
        renderdevice::IShaderParam const *pParam = block.getShaderParamByIndex(i);
        renderdevice::opengles_3_0::ShaderParam const *pGLParam = dynamic_cast<renderdevice::opengles_3_0::ShaderParam const*>(pParam);
        
        // bind data
        setGLUniformParam(*pGLParam, pGLParam->getData());
    }
}

//////////////////////////////////////////////////////////////////////////

//! Returns the shader program handle
GLuint ShaderProgram::getShaderProgramHandle() const{
    return m_programHandle;
}

//////////////////////////////////////////////////////////////////////////

//! Sets a param
void ShaderProgram::setGLUniformParam(renderdevice::opengles_3_0::ShaderParam const &param, void *pData){
    renderdevice::eShaderParamType type = param.getType();
    GLint uniformHandle = param.getHandle();
    
    // make sure we have a proper handle
    if(uniformHandle < 0){
        return;
    }
    // make sure we have a proper type
    if(type == renderdevice::kTypeNull){
        return;
    }
    // make sure we have a proper data
    if(pData == 0){
        return;
    }
    
    // set uniform
    switch(type){
        case renderdevice::kTypeFloat1:{
            GLDEBUG(glUniform1fv(uniformHandle, 1, (GLfloat*)pData));
            break;
        }
        case renderdevice::kTypeFloat2:{
            GLDEBUG(glUniform2fv(uniformHandle, 1, (GLfloat*)pData));
            break;
        }
        case renderdevice::kTypeFloat3:{
            GLDEBUG(glUniform3fv(uniformHandle, 1, (GLfloat*)pData));
            break;
        }
        case renderdevice::kTypeFloat4:{
            GLDEBUG(glUniform4fv(uniformHandle, 1, (GLfloat*)pData));
            break;
        }
            
        case renderdevice::kTypeFloat2x2:{
            GLDEBUG(glUniformMatrix2fv(uniformHandle, 1, 0, (GLfloat*)pData));
            break;
        }
        case renderdevice::kTypeFloat3x3:{
            GLDEBUG(glUniformMatrix3fv(uniformHandle, 1, 0, (GLfloat*)pData));
            break;
        }
        case renderdevice::kTypeFloat4x4:{
            GLDEBUG(glUniformMatrix4fv(uniformHandle, 1, 0, (GLfloat*)pData));
            break;
        }
            
        case renderdevice::kTypeInt1:{
            GLDEBUG(glUniform1iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
        case renderdevice::kTypeInt2:{
            GLDEBUG(glUniform2iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
        case renderdevice::kTypeInt3:{
            GLDEBUG(glUniform3iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
        case renderdevice::kTypeInt4:{
            GLDEBUG(glUniform4iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
            
        case renderdevice::kTypeBool1:{
            GLDEBUG(glUniform1iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
        case renderdevice::kTypeBool2:{
            GLDEBUG(glUniform2iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
        case renderdevice::kTypeBool3:{
            GLDEBUG(glUniform3iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
        case renderdevice::kTypeBool4:{
            GLDEBUG(glUniform4iv(uniformHandle, 1, (GLint*)pData));
            break;
        }
            
        case renderdevice::kTypeTexture2D:{
            renderdevice::ITexture *pTexture = reinterpret_cast<renderdevice::ITexture*>(pData);
            renderdevice::opengles_3_0::Texture *pNativeTexture = dynamic_cast<renderdevice::opengles_3_0::Texture*>(pTexture);
            GLint textureHandle = pNativeTexture->getTextureHandle();
            GLint textureUnit = param.getTextureUnit();
            GLDEBUG(glActiveTexture(GL_TEXTURE0 + textureUnit));
            GLDEBUG(glBindTexture(GL_TEXTURE_2D, textureHandle));
            GLDEBUG(glUniform1i(uniformHandle, textureUnit));
            break;
        }
        default:{
            NIMBLE_ASSERT(false);
            break;
        }
    };
}

//////////////////////////////////////////////////////////////////////////