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

//! Sets our shader program matrix states
void RenderDevice::patchShaderProgramMatrixParams(){
    if(m_context.m_pShaderProgram){
        // if the shader has a matrix param and it has changed, patch the shader with the new param
#define SHADERPARAM_TUPLE(NAME, SHADERNAME) \
if(m_context.m_shaderHas##NAME##ShaderParam && m_context.m_final##NAME##MatrixChanged){ \
m_context.m_pShaderProgram->setShaderParamByName(SHADERNAME, m_context.m_final##NAME##Matrix.transpose().getBuffer()); \
m_context.m_final##NAME##MatrixChanged = false; \
}
        SHADERPARAM_TUPLESET
#undef SHADERPARAM_TUPLE
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets our projection matrix
void RenderDevice::setProjectionMatrix(nimble::math::Matrix4x4f const &matrix){
    this->pushMatrixMode();
    this->setMatrixMode(renderdevice::kRenderMatrixModeProjection);
    this->clearMatrix();
    this->setMatrix(matrix);
    this->popMatrixMode();
}
//! sets our view matrix
void RenderDevice::setViewMatrix(nimble::math::Matrix4x4f const &matrix){
    this->pushMatrixMode();
    this->setMatrixMode(renderdevice::kRenderMatrixModeView);
    this->clearMatrix();
    this->setMatrix(matrix);
    this->popMatrixMode();
}
//! matrix stack functions
void RenderDevice::setMatrix(nimble::math::Matrix4x4f const &matrix){
    // set the appropriate matrix
    switch(this->getMatrixMode()){
        case renderdevice::kRenderMatrixModeModel:
            // update model matrix state
            m_context.m_modelMatrix = matrix;
            m_context.m_finalModelMatrix = m_context.m_topModelMatrix * m_context.m_modelMatrix;
            m_context.m_finalModelMatrixChanged = true;
            m_context.m_finalNormalMatrix = m_context.m_finalModelMatrix.inverse().transpose();
            m_context.m_finalNormalMatrixChanged = true;
            
            // update model view matrix state
            m_context.m_finalModelViewMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
            m_context.m_finalModelViewMatrixChanged = true;
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        case renderdevice::kRenderMatrixModeView:
            // update view matrix state
            m_context.m_viewMatrix = matrix;
            m_context.m_finalViewMatrix = m_context.m_topViewMatrix * m_context.m_viewMatrix;
            m_context.m_finalViewMatrixChanged = true;
            m_context.m_finalInvViewMatrix = m_context.m_finalViewMatrix.inverse();
            m_context.m_finalInvViewMatrixChanged = true;
            
            // update model view matrix state
            m_context.m_finalModelViewMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
            m_context.m_finalModelViewMatrixChanged = true;
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        case renderdevice::kRenderMatrixModeProjection:
            // update projection matrix state
            m_context.m_projectionMatrix = matrix;
            m_context.m_finalProjectionMatrix = m_context.m_topProjectionMatrix * m_context.m_projectionMatrix;
            m_context.m_finalProjectionMatrixChanged = true;
            
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        default:
            //CDebug::errorMessage("bad matrixmode type");
            break;
    }
}

//////////////////////////////////////////////////////////////////////////

nimble::math::Matrix4x4f RenderDevice::getMatrix(renderdevice::eRenderMatrixMode matrixMode) const{
    if(matrixMode == renderdevice::kRenderMatrixModeCurrent){
        matrixMode = this->getMatrixMode();
    }
    switch(matrixMode){
        case renderdevice::kRenderMatrixModeModel:
            return m_context.m_modelMatrix;
        case renderdevice::kRenderMatrixModeView:
            return m_context.m_viewMatrix;
        case renderdevice::kRenderMatrixModeProjection:
            return m_context.m_projectionMatrix;
        default:
            //CDebug::errorMessage("bad matrixmode type");
            break;
    }
    return nimble::math::Matrix4x4f::identity();
}

//////////////////////////////////////////////////////////////////////////

void RenderDevice::pushMatrix(){
    switch(this->getMatrixMode()){
        case renderdevice::kRenderMatrixModeModel:
            // calculate / push final matrix
            m_context.m_finalModelMatrixStack.push_back(m_context.m_finalModelMatrix);
            m_context.m_topModelMatrix *= m_context.m_finalModelMatrix;
            // push current matrix and reset
            m_context.m_modelMatrixStack.push_back(m_context.m_modelMatrix);
            m_context.m_modelMatrix = nimble::math::Matrix4x4f::identity();
            break;
        case renderdevice::kRenderMatrixModeView:
            // calculate / push final matrix
            m_context.m_finalViewMatrixStack.push_back(m_context.m_finalViewMatrix);
            m_context.m_topViewMatrix *= m_context.m_finalViewMatrix;
            // push current matrix and reset
            m_context.m_viewMatrixStack.push_back(m_context.m_viewMatrix);
            m_context.m_viewMatrix = nimble::math::Matrix4x4f::identity();
            break;
        case renderdevice::kRenderMatrixModeProjection:
            // calculate / push final matrix
            m_context.m_finalProjectionMatrixStack.push_back(m_context.m_finalProjectionMatrix);
            m_context.m_topProjectionMatrix = m_context.m_finalProjectionMatrix;
            // push current matrix and reset
            m_context.m_projectionMatrixStack.push_back(m_context.m_projectionMatrix);
            m_context.m_projectionMatrix = nimble::math::Matrix4x4f::identity();
            break;
        default:
            break;
    }
}
void RenderDevice::popMatrix(){
    switch(this->getMatrixMode()){
        case renderdevice::kRenderMatrixModeModel:
            // pop last current matrix
            m_context.m_modelMatrix = m_context.m_modelMatrixStack.back();
            m_context.m_modelMatrixStack.pop_back();
            // pop last final matrix
            m_context.m_topModelMatrix = m_context.m_finalModelMatrixStack.back();
            m_context.m_finalModelMatrixStack.pop_back();
            // calculate last final matrix
            m_context.m_finalModelMatrix = m_context.m_topModelMatrix * m_context.m_modelMatrix;
            m_context.m_finalModelMatrixChanged = true;
            // calculate last final matrix
            m_context.m_finalNormalMatrix = m_context.m_finalModelMatrix.inverse().transpose();
            m_context.m_finalNormalMatrixChanged = true;
            
            // update model view matrix state
            m_context.m_finalModelViewMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
            m_context.m_finalModelViewMatrixChanged = true;
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        case renderdevice::kRenderMatrixModeView:
            // pop last current matrix
            m_context.m_viewMatrix = m_context.m_viewMatrixStack.back();
            m_context.m_viewMatrixStack.pop_back();
            // pop last final matrix
            m_context.m_finalViewMatrix = m_context.m_finalViewMatrixStack.back();
            m_context.m_finalViewMatrixStack.pop_back();
            // calculate last final matrix
            m_context.m_finalViewMatrix = m_context.m_topViewMatrix * m_context.m_viewMatrix;
            m_context.m_finalViewMatrixChanged = true;
            // calculate last final matrix
            m_context.m_finalInvViewMatrix = m_context.m_finalViewMatrix.inverse();
            m_context.m_finalInvViewMatrixChanged = true;
            
            // update model view matrix state
            m_context.m_finalModelViewMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
            m_context.m_finalModelViewMatrixChanged = true;
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        case renderdevice::kRenderMatrixModeProjection:
            // pop last current matrix
            m_context.m_projectionMatrix = m_context.m_projectionMatrixStack.back();
            m_context.m_projectionMatrixStack.pop_back();
            // pop last top matrix
            m_context.m_topProjectionMatrix = m_context.m_finalProjectionMatrixStack.back();
            m_context.m_finalProjectionMatrixStack.pop_back();
            // calculate last final matrix
            m_context.m_finalProjectionMatrix = m_context.m_topProjectionMatrix * m_context.m_projectionMatrix;
            // set projection matrix changed flag
            m_context.m_finalProjectionMatrixChanged = true;
            
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        default:
            break;
    }
}
void RenderDevice::clearMatrix(){
    switch(this->getMatrixMode()){
        case renderdevice::kRenderMatrixModeModel:
            // clear stack, reset identity
            while(!m_context.m_modelMatrixStack.empty()){m_context.m_modelMatrixStack.pop_back();}
            m_context.m_modelMatrix = nimble::math::Matrix4x4f::identity();
            // clear stack, reset identity
            while(!m_context.m_finalModelMatrixStack.empty()){m_context.m_finalModelMatrixStack.pop_back();}
            m_context.m_finalModelMatrix = nimble::math::Matrix4x4f::identity();
            m_context.m_finalNormalMatrix = nimble::math::Matrix4x4f::identity();
            // set model matrix changed flag
            m_context.m_finalModelMatrixChanged = true;
            m_context.m_finalNormalMatrixChanged = true;
            
            // update model view matrix state
            m_context.m_finalModelViewMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
            m_context.m_finalModelViewMatrixChanged = true;
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        case renderdevice::kRenderMatrixModeView:
            // clear stack, reset identity
            while(!m_context.m_viewMatrixStack.empty()){m_context.m_viewMatrixStack.pop_back();}
            m_context.m_viewMatrix = nimble::math::Matrix4x4f::identity();
            // clear stack, reset identity
            while(!m_context.m_finalViewMatrixStack.empty()){m_context.m_finalViewMatrixStack.pop_back();}
            m_context.m_finalViewMatrix = nimble::math::Matrix4x4f::identity();
            m_context.m_finalInvViewMatrix = nimble::math::Matrix4x4f::identity();
            // set view matrix changed flag
            m_context.m_finalViewMatrixChanged = true;
            m_context.m_finalInvViewMatrixChanged = true;
            
            // update model view matrix state
            m_context.m_finalModelViewMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
            m_context.m_finalModelViewMatrixChanged = true;
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        case renderdevice::kRenderMatrixModeProjection:
            // clear stack, reset identity
            while(!m_context.m_projectionMatrixStack.empty()){m_context.m_projectionMatrixStack.pop_back();}
            m_context.m_projectionMatrix = nimble::math::Matrix4x4f::identity();
            // clear stack, reset identity
            while(!m_context.m_finalProjectionMatrixStack.empty()){m_context.m_finalProjectionMatrixStack.pop_back();}
            m_context.m_finalProjectionMatrix = nimble::math::Matrix4x4f::identity();
            // set projection matrix changed flag
            m_context.m_finalProjectionMatrixChanged = true;
            
            // update model view projection matrix state
            m_context.m_finalModelViewProjectionMatrix = m_context.m_finalProjectionMatrix * m_context.m_finalModelViewMatrix;
            m_context.m_finalModelViewProjectionMatrixChanged = true;
            break;
        default:
            break;
    }
}

//////////////////////////////////////////////////////////////////////////

//! sets matrix mode
//! \param mode the matrix mode to set
void RenderDevice::setMatrixMode(renderdevice::eRenderMatrixMode mode){
    m_context.m_currentMatrixMode = mode;
}
//! gets matrix mode
//! \return the matrix mode
renderdevice::eRenderMatrixMode RenderDevice::getMatrixMode() const{
    return m_context.m_currentMatrixMode;
}
// matrix mode stack
void RenderDevice::pushMatrixMode(){
    m_context.m_matrixModeStack.push_back(m_context.m_currentMatrixMode);
}
void RenderDevice::popMatrixMode(){
    m_context.m_currentMatrixMode = (renderdevice::eRenderMatrixMode)m_context.m_matrixModeStack.back();
    setMatrixMode(m_context.m_currentMatrixMode);
    m_context.m_matrixModeStack.pop_back();
}


//////////////////////////////////////////////////////////////////////////