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

//! sets our transformation matrix
void RenderDevice::setGLMatrix(eMatrixMode mode){
    assert(mode != RenderDevice::kMatrixModeCurrent);
    
    // early out
    if(mode == RenderDevice::kMatrixModeModel && !m_context.m_finalModelMatrixChanged){
        return;
    }
    if(mode == RenderDevice::kMatrixModeView && !m_context.m_finalViewMatrixChanged){
        return;
    }
    if(mode == RenderDevice::kMatrixModeProjection && !m_context.m_finalProjectionMatrixChanged){
        return;
    }
    
    // set matrix mode
    this->pushMatrixMode();
    this->setMatrixMode(mode);
    m_context.m_finalViewMatrixChanged = false;
    m_context.m_finalModelMatrixChanged = false;
    m_context.m_finalProjectionMatrixChanged = false;
    
    // set our matrix
    if(m_context.m_pShaderProgram){
        math::Matrix4x4f finalMVMatrix, finalMVPMatrix;
        finalMVMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
        finalMVPMatrix = m_context.m_finalProjectionMatrix * finalMVMatrix;
        m_context.m_pShaderProgram->setParam("nimble_modelViewMatrix",
                                       renderdevice::IShaderProgram::TYPE_MATRIX4F,
                                       finalMVMatrix.transpose().getBuffer());
        m_context.m_pShaderProgram->setParam("nimble_modelViewProjectionMatrix",
                                       renderdevice::IShaderProgram::TYPE_MATRIX4F,
                                       finalMVPMatrix.transpose().getBuffer());
    }
    this->popMatrixMode();

//    // set matrix mode
//    this->pushMatrixMode();
//    this->setMatrixMode(mode);
//	setGLRenderStateMatrixMode(mode);
//
//	// construct final matrix
//    nimble::math::Matrix4x4f finalMatrix;
//	switch(mode){
//		case RenderDevice::kMatrixModeModel:
//		case RenderDevice::kMatrixModeView:
//            finalMatrix = m_context.m_finalViewMatrix * m_context.m_finalModelMatrix;
//            m_context.m_finalViewMatrixChanged = false;
//            m_context.m_finalModelMatrixChanged = false;
//            break;
//		case RenderDevice::kMatrixModeProjection:
//            finalMatrix = m_context.m_finalProjectionMatrix;
//            m_context.m_finalProjectionMatrixChanged = false;
//            break;
//		default:
//			//CDebug::errorMessage("bad matrixmode type");
//			break;
//	}
//
//    // set our matrix
//	finalMatrix.transpose();
//	glLoadMatrixf(finalMatrix.getBuffer());
//    this->popMatrixMode();
}

//! matrix stack functions
void RenderDevice::setMatrix(nimble::math::Matrix4x4f& matrix){
	// set the appropriate matrix
	switch(this->getMatrixMode()){
		case RenderDevice::kMatrixModeModel:
            m_context.m_modelMatrix = matrix;
			m_context.m_finalModelMatrix = m_context.m_topModelMatrix * m_context.m_modelMatrix;
            m_context.m_finalModelMatrixChanged = true;
			break;
		case RenderDevice::kMatrixModeView:
			m_context.m_viewMatrix = matrix;
			m_context.m_finalViewMatrix = m_context.m_topViewMatrix * m_context.m_viewMatrix;
            m_context.m_finalViewMatrixChanged = true;
			break;
        case RenderDevice::kMatrixModeProjection:
			m_context.m_projectionMatrix = matrix;
            m_context.m_finalProjectionMatrix = m_context.m_topProjectionMatrix * m_context.m_projectionMatrix;
            m_context.m_finalProjectionMatrixChanged = true;
			break;
		default:
			//CDebug::errorMessage("bad matrixmode type");
			break;
	}
}

//////////////////////////////////////////////////////////////////////////

nimble::math::Matrix4x4f RenderDevice::getMatrix(eMatrixMode matrixMode){
	if(matrixMode == RenderDevice::kMatrixModeCurrent){
		matrixMode = this->getMatrixMode();
	}
	switch(matrixMode){
		case RenderDevice::kMatrixModeModel:
			return m_context.m_modelMatrix;
		case RenderDevice::kMatrixModeView:
			return m_context.m_viewMatrix;
		case RenderDevice::kMatrixModeProjection:
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
		case RenderDevice::kMatrixModeModel:
			// calculate / push final matrix
			m_context.m_finalModelMatrixStack.push_back(m_context.m_finalModelMatrix);
			m_context.m_topModelMatrix *= m_context.m_finalModelMatrix;
			// push current matrix and reset
			m_context.m_modelMatrixStack.push_back(m_context.m_modelMatrix);
			m_context.m_modelMatrix = nimble::math::Matrix4x4f::identity();
			break;
		case RenderDevice::kMatrixModeView:
			// calculate / push final matrix
			m_context.m_finalViewMatrixStack.push_back(m_context.m_finalViewMatrix);
			m_context.m_topViewMatrix *= m_context.m_finalViewMatrix;
			// push current matrix and reset
			m_context.m_viewMatrixStack.push_back(m_context.m_viewMatrix);
			m_context.m_viewMatrix = nimble::math::Matrix4x4f::identity();
			break;
        case RenderDevice::kMatrixModeProjection:
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
		case RenderDevice::kMatrixModeModel:
			// pop last current matrix
			m_context.m_modelMatrix = m_context.m_modelMatrixStack.back();
			m_context.m_modelMatrixStack.pop_back();
			// pop last final matrix
			m_context.m_topModelMatrix = m_context.m_finalModelMatrixStack.back();
			m_context.m_finalModelMatrixStack.pop_back();
            // calculate last final matrix
            m_context.m_finalModelMatrix = m_context.m_topModelMatrix * m_context.m_modelMatrix;
            // set model matrix changed flag
            m_context.m_finalModelMatrixChanged = true;
			break;
		case RenderDevice::kMatrixModeView:
			// pop last current matrix
			m_context.m_viewMatrix = m_context.m_viewMatrixStack.back();
			m_context.m_viewMatrixStack.pop_back();
			// pop last final matrix
			m_context.m_finalViewMatrix = m_context.m_finalViewMatrixStack.back();
			m_context.m_finalViewMatrixStack.pop_back();
            // calculate last final matrix
            m_context.m_finalViewMatrix = m_context.m_topViewMatrix * m_context.m_viewMatrix;
            // set view matrix changed flag
            m_context.m_finalViewMatrixChanged = true;
			break;
        case RenderDevice::kMatrixModeProjection:
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
			break;
        default:
            break;
	}
}
void RenderDevice::clearMatrix(){
	switch(this->getMatrixMode()){
		case RenderDevice::kMatrixModeModel:
			// clear stack, reset identity
			while(!m_context.m_modelMatrixStack.empty()){m_context.m_modelMatrixStack.pop_back();}
			m_context.m_modelMatrix = nimble::math::Matrix4x4f::identity();
			// clear stack, reset identity
			while(!m_context.m_finalModelMatrixStack.empty()){m_context.m_finalModelMatrixStack.pop_back();}
			m_context.m_finalModelMatrix = nimble::math::Matrix4x4f::identity();
            // set model matrix changed flag
            m_context.m_finalModelMatrixChanged = true;
			break;
		case RenderDevice::kMatrixModeView:
			// clear stack, reset identity
			while(!m_context.m_viewMatrixStack.empty()){m_context.m_viewMatrixStack.pop_back();}
			m_context.m_viewMatrix = nimble::math::Matrix4x4f::identity();
			// clear stack, reset identity
			while(!m_context.m_finalViewMatrixStack.empty()){m_context.m_finalViewMatrixStack.pop_back();}
			m_context.m_finalViewMatrix = nimble::math::Matrix4x4f::identity();
            // set view matrix changed flag
            m_context.m_finalViewMatrixChanged = true;
			break;
        case RenderDevice::kMatrixModeProjection:
			// clear stack, reset identity
			while(!m_context.m_projectionMatrixStack.empty()){m_context.m_projectionMatrixStack.pop_back();}
			m_context.m_projectionMatrix = nimble::math::Matrix4x4f::identity();
			// clear stack, reset identity
			while(!m_context.m_finalProjectionMatrixStack.empty()){m_context.m_finalProjectionMatrixStack.pop_back();}
			m_context.m_finalProjectionMatrix = nimble::math::Matrix4x4f::identity();
            // set projection matrix changed flag
            m_context.m_finalProjectionMatrixChanged = true;
			break;
        default:
            break;
	}
}

//////////////////////////////////////////////////////////////////////////

//! sets matrix mode
//! \param mode the matrix mode to set
void RenderDevice::setMatrixMode(renderdevice::IRenderDevice::eMatrixMode mode){
    setGLRenderStateMatrixMode(mode);
}
//! gets matrix mode
//! \return the matrix mode
renderdevice::IRenderDevice::eMatrixMode RenderDevice::getMatrixMode(){
    return m_context.m_currentMatrixMode;
}
// matrix mode stack
void RenderDevice::pushMatrixMode(){
	m_context.m_matrixModeStack.push_back(m_context.m_currentMatrixMode);
}
void RenderDevice::popMatrixMode(){
    m_context.m_currentMatrixMode = (renderdevice::IRenderDevice::eMatrixMode)m_context.m_matrixModeStack.back();
    setMatrixMode(m_context.m_currentMatrixMode);
	m_context.m_matrixModeStack.pop_back();
}

//////////////////////////////////////////////////////////////////////////