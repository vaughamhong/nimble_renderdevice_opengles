//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/shaderparamblock.h>
#include <nimble/core/logger.h>
#include <nimble/core/hash.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
ShaderParamBlock::ShaderParamBlock(){
}
//! Constructor
ShaderParamBlock::ShaderParamBlock(renderdevice::opengles_3_0::ShaderParamBlock const &block){
    // copy over each parameter one at a time
    size_t numParams = block.getNumShaderParams();
    for(size_t i = 0; i < numParams; i++){
        this->addShaderParam(*block.getShaderParamByIndex(i));
    }
}
//! Destructor
ShaderParamBlock::~ShaderParamBlock(){
}

//////////////////////////////////////////////////////////////////////////

//! Returns the number of parameters
size_t ShaderParamBlock::getNumShaderParams() const{
    return m_paramList.size();
}

//////////////////////////////////////////////////////////////////////////

//! Returns true if param with name exists
//! \return true if param with name exists
bool ShaderParamBlock::existsParamWithName(const char *name) const{
    return m_nameToParamIndex.find(core::hash(name)) != m_nameToParamIndex.end();
}

//////////////////////////////////////////////////////////////////////////

//! Returns the shader param by index
renderdevice::IShaderParam const* ShaderParamBlock::getShaderParamByIndex(size_t index) const{
    ParamList::const_iterator it = m_paramList.begin() + index;
    if(it != m_paramList.end()){
        return dynamic_cast<renderdevice::IShaderParam const*>(&(*it));
    }else{
        return 0;
    }
}
//! Returns the shader param by name
renderdevice::IShaderParam const* ShaderParamBlock::getShaderParamByName(const char *name) const{
    int32_t uniformNameHash = core::hash(name);
    NameToParamIndex::const_iterator it = m_nameToParamIndex.find(uniformNameHash);
    if(it != m_nameToParamIndex.end()){
        return dynamic_cast<renderdevice::IShaderParam const*>(&m_paramList[it->second]);
    }else{
        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////

//! Sets shader param by index
void ShaderParamBlock::setShaderParamByIndex(size_t index, void *pData){
    ParamList::iterator it = m_paramList.begin() + index;
    renderdevice::ShaderParam *pParam = dynamic_cast<renderdevice::ShaderParam*>(&(*it));
    pParam->setData(pData);
}
//! Sets shader param by name
void ShaderParamBlock::setShaderParamByName(const char *name, void *pData){
    int32_t uniformNameHash = core::hash(name);
    NameToParamIndex::iterator it = m_nameToParamIndex.find(uniformNameHash);
    if(it != m_nameToParamIndex.end()){
        renderdevice::ShaderParam *pParam = &m_paramList[it->second];
        pParam->setData(pData);
    }
}

//////////////////////////////////////////////////////////////////////////

//! Adds a shader param
void ShaderParamBlock::addShaderParam(renderdevice::IShaderParam const &param){
    const char *name = param.getName();
    int32_t uniformNameHash = core::hash(name);
    NameToParamIndex::iterator it = m_nameToParamIndex.find(uniformNameHash);
    if(it != m_nameToParamIndex.end()){
        core::logger_error("graphics", "Failed to add shader param with name \"%s\"", name);
        return;
    }
    
    renderdevice::opengles_3_0::ShaderParam const *pShaderParam = dynamic_cast<renderdevice::opengles_3_0::ShaderParam const*>(&param);
    if(pShaderParam == 0){
        core::logger_error("graphics", "Failed to add shader param with name \"%s\"", name);
        return;
    }

    // add param to our index
    m_nameToParamIndex.insert(std::make_pair(uniformNameHash, m_paramList.size()));
    // add param to our list
    m_paramList.push_back(*pShaderParam);
}

//////////////////////////////////////////////////////////////////////////

//! Clears all parameters
void ShaderParamBlock::clear(){
    m_paramList.clear();
    m_nameToParamIndex.clear();
}

//////////////////////////////////////////////////////////////////////////