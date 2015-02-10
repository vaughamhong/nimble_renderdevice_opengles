//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_1_1/texture.h>
#include <nimble/renderdevice-opengles_1_1/mappings.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_1_1;

//////////////////////////////////////////////////////////////////////////

//! Constructor
Texture::Texture(core::UInt32 size, renderdevice::ITexture::eFormat textureFormat, core::UInt32 usage){
    // note: OpenGLES 1.1 only supports 2D textures
    core::assert_error(m_dimension == renderdevice::ITexture::kDimention2D);
}
//! Constructor
Texture::Texture(core::UInt32 width, core::UInt32 height, renderdevice::ITexture::eFormat textureFormat, core::UInt32 usage)
:m_textureHandle(0)
,m_pboHandle(0)
,m_usage(usage)
,m_created(false)
,m_dimension(renderdevice::ITexture::kDimention2D)
,m_textureFormat(textureFormat){    
    for(core::Int32 i = 0; i < renderdevice::ITexture::kMaxDimentions; i++){
        m_dimensionValues[i] = 1;
    }
    m_dimensionValues[0] = width;
    m_dimensionValues[1] = height;
    setWrapMode(renderdevice::ITexture::kWrapParamU, renderdevice::ITexture::kDefaultWrapMode);
    setWrapMode(renderdevice::ITexture::kWrapParamV, renderdevice::ITexture::kDefaultWrapMode);
    setFilterMode(renderdevice::ITexture::kFilterParamMin, renderdevice::ITexture::kDefaultFilterMode);
    setFilterMode(renderdevice::ITexture::kFilterParamMag, renderdevice::ITexture::kDefaultFilterMode);
	createBuffers();
}
//! Constructor
Texture::Texture(core::UInt32 width, core::UInt32 height, core::UInt32 depth, renderdevice::ITexture::eFormat textureFormat, core::UInt32 usage){
    //! note: OpenGLES 1.1 only supports 2D textures
    core::assert_error(m_dimension == renderdevice::ITexture::kDimention2D);
}
//! Constructor
Texture::Texture(Texture& texture)
:m_textureHandle(0)
,m_pboHandle(0)
,m_usage(texture.m_usage)
,m_created(false)
,m_dimension(texture.m_dimension)
,m_textureFormat(texture.m_textureFormat){
    // note: OpenGLES 1.1 only supports 2D textures
    core::assert_error(m_dimension == renderdevice::ITexture::kDimention2D);
    
    // copy over various attributes and create our buffers
    for(core::Int32 i = 0; i < renderdevice::ITexture::kMaxDimentions; i++){
        m_dimensionValues[i] = texture.m_dimensionValues[i];
    }
    setWrapMode(renderdevice::ITexture::kWrapParamU, texture.getWrapMode(renderdevice::ITexture::kWrapParamU));
    setWrapMode(renderdevice::ITexture::kWrapParamV, texture.getWrapMode(renderdevice::ITexture::kWrapParamV));
    setFilterMode(renderdevice::ITexture::kFilterParamMin, texture.getFilterMode(renderdevice::ITexture::kFilterParamMin));
    setFilterMode(renderdevice::ITexture::kFilterParamMag, texture.getFilterMode(renderdevice::ITexture::kFilterParamMag));
    createBuffers();

    // copy our source data to our destination
    char* pSrPointer = 0;
    char* pDestPointer = 0;
    if(texture.lock(core::ILockable::kTypeRead, (char**)&pSrPointer)){
        if(this->lock(core::ILockable::kTypeWrite, (char**)&pDestPointer)){
            memcpy(pDestPointer, pSrPointer, getBufferSize());
            this->unlock();
            texture.unlock();
        }else{
            core::logError("graphics", "Failed to lock destination texture buffer");
            texture.unlock();
        }
    }else{
        core::logError("graphics", "Failed to lock source texture buffer");
    }
}
//! Destructor
Texture::~Texture(){
	destroyBuffers();
}

//////////////////////////////////////////////////////////////////////////

//! Initialize this object's data
//! \return true if initialization was successful
void Texture::createBuffers(){
    core::assert_error(!m_created);
    core::assert_error(m_dimensionValues[0] > 0);
    core::assert_error(m_dimensionValues[1] > 0);
    core::assert_error(m_dimension != renderdevice::ITexture::kUnknownDimension);
    core::assert_error(m_textureFormat != renderdevice::ITexture::kUnknownFormat);

    core::UInt32 width = m_dimensionValues[0];
    core::UInt32 height = m_dimensionValues[1];
    core::UInt32 bufferSize = getBufferSize();
    core::assert_error(bufferSize > 0);

    // create a local buffer
	if(m_usage & renderdevice::ITexture::kUsageLocalBuffer){
        m_localBuffer.resize(bufferSize);
	}
    
	// generate a texture handle
	glGenTextures(1, &m_textureHandle);
        
	// generate a pixel buffer object handle
    // note: OpenGLES 1.1 only supports 2D textures
	glGenBuffers(1, &m_pboHandle);
    core::UInt32 dimension  = gTextureDimensionMap[m_dimension];
	core::UInt32 format = gTextureFormatMap[m_textureFormat];
	core::UInt32 internalFormat = gTextureInternalFormatMap[m_textureFormat];
	core::UInt32 type = gTextureTypeMap[m_textureFormat];
    core::UInt32 filterModeMag = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMag)];
	core::UInt32 filterModeMin = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMin)];
	core::UInt32 wrapModeU = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamU)];
	core::UInt32 wrapModeV = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamV)];
	GLuint handle = this->getTextureHandle();
    
    // track previously bound
    GLint boundHandle;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundHandle);
    if(boundHandle != handle){
        glBindTexture(dimension, handle);
    }
    
    // create texture
    glBindTexture(dimension, handle);
    glTexParameteri(dimension, GL_TEXTURE_MIN_FILTER, filterModeMag);
	glTexParameteri(dimension, GL_TEXTURE_MAG_FILTER, filterModeMin);
	glTexParameteri(dimension, GL_TEXTURE_WRAP_S, wrapModeU);
	glTexParameteri(dimension, GL_TEXTURE_WRAP_T, wrapModeV);
    glTexImage2D(dimension,			// target
                 0,					// mip-level
                 internalFormat,    // internal format
                 width,				// width
                 height,			// height
                 0,					// border
                 format,			// format
                 type,				// type
                 0);				// data
    
    // restore previously bound
    if(boundHandle != handle){
        glBindTexture(dimension, boundHandle);
    }
    
    m_created = true;
}

//! Destroy this object's data
//! \return true if destruction was successful
void Texture::destroyBuffers(){
    core::assert_error(m_created);
    
	if(m_pboHandle){
		glDeleteBuffers(1, &m_pboHandle);
	}
	if(m_textureHandle){
		glDeleteTextures(1, &m_textureHandle);
	}
    for(core::Int32 i = 0; i < renderdevice::ITexture::kMaxDimentions; i++){
        m_dimensionValues[i] = 0;
    }
    m_created = false;
}

//////////////////////////////////////////////////////////////////////////

//! returns the usage
//! \return the usage
core::UInt32 Texture::getUsage() const{
    return m_usage;
}
//! gets the format of this texture
renderdevice::ITexture::eFormat Texture::getFormat() const{
    return m_textureFormat;
}
//! gets the number of dimensions of this texture
renderdevice::ITexture::eDimension Texture::getDimension() const{
    return m_dimension;
}
//! gets the width of the texture
core::UInt32 Texture::getWidth() const{
    return m_dimensionValues[0];
}
//! gets the height of the texture
core::UInt32 Texture::getHeight() const{
    return m_dimensionValues[1];
}
//! gets the depth of the texture
core::UInt32 Texture::getDepth() const{
    return m_dimensionValues[2];
}
//! gets the bytes per pixel
core::UInt32 Texture::getBytesPerPixel() const{
    core::assert_error(m_textureFormat != renderdevice::ITexture::kUnknownFormat);
    return gTexturePixelSizeMap[m_textureFormat];
}
//! gets the texture size in bytes
core::UInt32 Texture::getBufferSize() const{
    core::assert_error(m_dimensionValues[0] > 0);
    core::assert_error(m_dimensionValues[1] > 0);
    return m_dimensionValues[0] * m_dimensionValues[1] * getBytesPerPixel();
}

//////////////////////////////////////////////////////////////////////////

//! sets the filter mode
void Texture::setFilterMode(renderdevice::ITexture::eFilterParameter param, renderdevice::ITexture::eFilterMode value){
	m_filterMode[param] = value;
}
//! sets the wrap mode
void Texture::setWrapMode(renderdevice::ITexture::eWrapParameter param, renderdevice::ITexture::eWrapMode value){
	m_wrapMode[param] = value;
}
//! gets the texture filter mode
//! \param param the filter parameter
renderdevice::ITexture::eFilterMode Texture::getFilterMode(renderdevice::ITexture::eFilterParameter param) const{
	return m_filterMode[param];
}
//! gets the texture wrap mode
//! \param param the wrap parameeter
renderdevice::ITexture::eWrapMode Texture::getWrapMode(renderdevice::ITexture::eWrapParameter param) const{
	return m_wrapMode[param];
}

//////////////////////////////////////////////////////////////////////////

//! gets the GL handle
GLuint Texture::getTextureHandle() const{
    return m_textureHandle;
}

//////////////////////////////////////////////////////////////////////////

//! attempts to map a buffer
core::Int8* Texture::mapBuffer(eLockType lockType){
    core::assert_error(!isLocked());
    return this->mapBufferRange(lockType, 0, this->getBufferSize());
}
//! attempts to map a buffer
core::Int8* Texture::mapBufferRange(eLockType lockType, core::UInt32 offset, core::UInt32 size){
    core::assert_error(!isLocked());
    core::assert_error(offset < this->getBufferSize());
    core::assert_error((offset + size) <= this->getBufferSize());
    
    char *pBuffer = NULL;
    if(m_usage & renderdevice::ITexture::kUsageLocalBuffer){
        pBuffer = m_localBuffer.getPointer(offset);
    }
    return pBuffer;
}
//! attempts to unmap a buffer
void Texture::unmapBuffer(){
    if(m_usage & renderdevice::ITexture::kUsageLocalBuffer){
        if(char *pData = m_localBuffer.getPointer()){
            core::UInt32 width = this->getWidth();
            core::UInt32 height= this->getHeight();
            core::UInt32 dimension  = gTextureDimensionMap[this->getDimension()];
            core::UInt32 format = gTextureFormatMap[this->getFormat()];
            core::UInt32 internalFormat = gTextureInternalFormatMap[this->getFormat()];
            core::UInt32 type = gTextureTypeMap[this->getFormat()];
            core::UInt32 filterModeMag = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMag)];
            core::UInt32 filterModeMin = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMin)];
            core::UInt32 wrapModeU = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamU)];
            core::UInt32 wrapModeV = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamV)];
            GLuint handle = this->getTextureHandle();
            
            // track previously bound
            GLint boundHandle;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundHandle);
            if(boundHandle != handle){
                glBindTexture(dimension, handle);
            }
            
            // update texture
            glTexParameteri(dimension, GL_TEXTURE_MIN_FILTER, filterModeMag);
            glTexParameteri(dimension, GL_TEXTURE_MAG_FILTER, filterModeMin);
            glTexParameteri(dimension, GL_TEXTURE_WRAP_S, wrapModeU);
            glTexParameteri(dimension, GL_TEXTURE_WRAP_T, wrapModeV);
            glTexImage2D(dimension,			// target
                         0,					// level
                         internalFormat,    // internal format
                         width,				// width
                         height,			// height
                         0,					// border
                         format,			// format
                         type,				// type
                         pData);			// data
            
            // restore previously bound
            if(boundHandle != handle){
                glBindTexture(dimension, boundHandle);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////