//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_2_0/texture.h>
#include <nimble/renderdevice/opengles_2_0/mappings.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! Constructor
Texture::Texture(uint32_t size, renderdevice::ITexture::eFormat textureFormat, uint32_t usage){
    // note: OpenGLES 1.1 only supports 2D textures
    core::assert_error(m_dimension == renderdevice::ITexture::kDimention2D);
}
//! Constructor
Texture::Texture(uint32_t width, uint32_t height, renderdevice::ITexture::eFormat textureFormat, uint32_t usage)
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
Texture::Texture(uint32_t width, uint32_t height, uint32_t depth, renderdevice::ITexture::eFormat textureFormat, uint32_t usage){
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
    if(texture.lock(core::kLockTypeRead, (char**)&pSrPointer)){
        if(this->lock(core::kLockTypeWrite, (char**)&pDestPointer)){
            memcpy(pDestPointer, pSrPointer, getBufferSize());
            this->unlock();
            texture.unlock();
        }else{
            core::logger_error("graphics", "Failed to lock destination texture buffer");
            texture.unlock();
        }
    }else{
        core::logger_error("graphics", "Failed to lock source texture buffer");
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

    uint32_t width = m_dimensionValues[0];
    uint32_t height = m_dimensionValues[1];
    uint32_t bufferSize = getBufferSize();
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
    uint32_t dimension  = gTextureDimensionMap[m_dimension];
	uint32_t format = gTextureFormatMap[m_textureFormat];
	uint32_t internalFormat = gTextureInternalFormatMap[m_textureFormat];
	uint32_t type = gTextureTypeMap[m_textureFormat];
    uint32_t filterModeMag = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMag)];
	uint32_t filterModeMin = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMin)];
	uint32_t wrapModeU = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamU)];
	uint32_t wrapModeV = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamV)];
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
uint32_t Texture::getUsage() const{
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
uint32_t Texture::getWidth() const{
    return m_dimensionValues[0];
}
//! gets the height of the texture
uint32_t Texture::getHeight() const{
    return m_dimensionValues[1];
}
//! gets the depth of the texture
uint32_t Texture::getDepth() const{
    return m_dimensionValues[2];
}
//! gets the bytes per pixel
uint32_t Texture::getBytesPerPixel() const{
    core::assert_error(m_textureFormat != renderdevice::ITexture::kUnknownFormat);
    return gTexturePixelSizeMap[m_textureFormat];
}
//! gets the texture size in bytes
uint32_t Texture::getBufferSize() const{
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
char* Texture::mapBuffer(eLockType lockType){
    core::assert_error(!isLocked());
    return this->mapBufferRange(lockType, 0, this->getBufferSize());
}
//! attempts to map a buffer
char* Texture::mapBufferRange(eLockType lockType, uint32_t offset, uint32_t size){
    core::assert_error(!isLocked());
    core::assert_error(offset < this->getBufferSize());
    core::assert_error((offset + size) <= this->getBufferSize());
    
    char *pBuffer = NULL;
    if(m_localBuffer.getPointer()){
        pBuffer = m_localBuffer.getPointer() + offset;
    }
    return pBuffer;
}
//! attempts to unmap a buffer
void Texture::unmapBuffer(){
    if(m_usage & renderdevice::ITexture::kUsageLocalBuffer){
        if(char *pData = m_localBuffer.getPointer()){
            uint32_t width = this->getWidth();
            uint32_t height= this->getHeight();
            uint32_t dimension  = gTextureDimensionMap[this->getDimension()];
            uint32_t format = gTextureFormatMap[this->getFormat()];
            uint32_t internalFormat = gTextureInternalFormatMap[this->getFormat()];
            uint32_t type = gTextureTypeMap[this->getFormat()];
            uint32_t filterModeMag = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMag)];
            uint32_t filterModeMin = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::ITexture::kFilterParamMin)];
            uint32_t wrapModeU = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamU)];
            uint32_t wrapModeV = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::ITexture::kWrapParamV)];
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