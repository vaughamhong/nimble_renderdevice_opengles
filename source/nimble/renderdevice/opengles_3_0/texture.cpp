//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/texture.h>
#include <nimble/renderdevice/opengles_3_0/mappings.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! returns the internal texture format
renderdevice::eTextureFormat imageFormatToTextureFormat(image::eImageFormat format){
#define TEXTURE_TUPLE(ENUM, PIXEL_BYTESIZE, PIXEL_BPP, IMAGEFORMAT) if(IMAGEFORMAT == format){return renderdevice::ENUM;}
    TEXTURE_TUPLESET
#undef TEXTURE_TUPLE
    return renderdevice::kUnknownTextureFormat;
}

//////////////////////////////////////////////////////////////////////////

//! Constructor
Texture::Texture(uint32_t width, uint32_t height, renderdevice::eTextureFormat textureFormat, uint32_t usage)
:m_arrayBuffer(width * height, gTexturePixelSizeMap[textureFormat], usage, GL_PIXEL_UNPACK_BUFFER)
,m_textureHandle(0)
,m_created(false)
,m_dimension(renderdevice::kTextureDimention2D)
,m_textureFormat(textureFormat){
    for(int32_t i = 0; i < renderdevice::kMaxTextureDimentions; i++){
        m_dimensionValues[i] = 1;
    }
    m_dimensionValues[0] = width;
    m_dimensionValues[1] = height;
    setWrapMode(renderdevice::kTextureWrapParamU, renderdevice::kDefaultTextureWrapMode);
    setWrapMode(renderdevice::kTextureWrapParamV, renderdevice::kDefaultTextureWrapMode);
    setFilterMode(renderdevice::kTextureFilterParamMin, renderdevice::kDefaultTextureFilterMode);
    setFilterMode(renderdevice::kTextureFilterParamMag, renderdevice::kDefaultTextureFilterMode);
    createBuffers();
}
//! Constructor
Texture::Texture(image::Image& image, uint32_t usage)
:m_arrayBuffer(image.getWidth() * image.getHeight(), image.getBytesPerPixel(), usage, GL_PIXEL_UNPACK_BUFFER)
,m_textureHandle(0)
,m_created(false)
,m_dimension(renderdevice::kTextureDimention2D)
,m_textureFormat(imageFormatToTextureFormat(image.getFormat())){
    for(int32_t i = 0; i < renderdevice::kMaxTextureDimentions; i++){
        m_dimensionValues[i] = 1;
    }
    m_dimensionValues[0] = image.getWidth();
    m_dimensionValues[1] = image.getHeight();
    setWrapMode(renderdevice::kTextureWrapParamU, renderdevice::kDefaultTextureWrapMode);
    setWrapMode(renderdevice::kTextureWrapParamV, renderdevice::kDefaultTextureWrapMode);
    setFilterMode(renderdevice::kTextureFilterParamMin, renderdevice::kDefaultTextureFilterMode);
    setFilterMode(renderdevice::kTextureFilterParamMag, renderdevice::kDefaultTextureFilterMode);
    createBuffers();

    // copy our source data to our destination
    char* pDestPointer = 0;
    if(this->lock(core::kLockTypeWrite, (char**)&pDestPointer)){
        memcpy(pDestPointer, image.getBuffer(), getBufferSize());
        this->unlock();
    }else{
        NIMBLE_LOG_ERROR("graphics", "Failed to lock destination texture buffer");
    }
}
//! Constructor
Texture::Texture(Texture& texture)
:m_arrayBuffer(texture.getWidth() * texture.getHeight(), texture.getBytesPerPixel(), texture.getUsage(), GL_PIXEL_UNPACK_BUFFER)
,m_textureHandle(0)
,m_created(false)
,m_dimension(texture.m_dimension)
,m_textureFormat(texture.m_textureFormat){
    // copy over various attributes and create our buffers
    for(int32_t i = 0; i < renderdevice::kMaxTextureDimentions; i++){
        m_dimensionValues[i] = texture.m_dimensionValues[i];
    }
    setWrapMode(renderdevice::kTextureWrapParamU, texture.getWrapMode(renderdevice::kTextureWrapParamU));
    setWrapMode(renderdevice::kTextureWrapParamV, texture.getWrapMode(renderdevice::kTextureWrapParamV));
    setFilterMode(renderdevice::kTextureFilterParamMin, texture.getFilterMode(renderdevice::kTextureFilterParamMin));
    setFilterMode(renderdevice::kTextureFilterParamMag, texture.getFilterMode(renderdevice::kTextureFilterParamMag));
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
            NIMBLE_LOG_ERROR("graphics", "Failed to lock destination texture buffer");
            texture.unlock();
        }
    }else{
        NIMBLE_LOG_ERROR("graphics", "Failed to lock source texture buffer");
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
    NIMBLE_ASSERT(!m_created);
    NIMBLE_ASSERT(m_dimensionValues[0] > 0);
    NIMBLE_ASSERT(m_dimensionValues[1] > 0);
    NIMBLE_ASSERT(m_dimension != renderdevice::kUnknownTextureDimension);
    NIMBLE_ASSERT(m_textureFormat != renderdevice::kUnknownTextureFormat);
    
    uint32_t width = m_dimensionValues[0];
    uint32_t height = m_dimensionValues[1];
    uint32_t bufferSize = getBufferSize();
    NIMBLE_ASSERT(bufferSize > 0);
    
    // generate a texture handle
    GLDEBUG(glGenTextures(1, &m_textureHandle));
    
    // generate a pixel buffer object handle
    // note: OpenGLES 1.1 only supports 2D textures
    uint32_t dimension  = gTextureDimensionMap[m_dimension];
    uint32_t format = gTextureFormatMap[m_textureFormat];
    uint32_t internalFormat = gTextureInternalFormatMap[m_textureFormat];
    uint32_t type = gTextureTypeMap[m_textureFormat];
    uint32_t filterModeMag = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::kTextureFilterParamMag)];
    uint32_t filterModeMin = gTextureFilterModeValueMap[this->getFilterMode(renderdevice::kTextureFilterParamMin)];
    uint32_t wrapModeU = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::kTextureWrapParamU)];
    uint32_t wrapModeV = gTextureWrapModeValueMap[this->getWrapMode(renderdevice::kTextureWrapParamV)];
    GLuint textureHandle = this->getTextureHandle();
    GLuint pboHandle = this->m_arrayBuffer.getArrayHandle();
    
    // track previously bound
    GLint textureBoundHandle, pboBoundHandle;
    GLDEBUG(glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBoundHandle));
    GLDEBUG(glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pboBoundHandle));
    if(textureBoundHandle != (GLint)textureHandle){
        GLDEBUG(glBindTexture(dimension, textureHandle));
    }
    if(pboBoundHandle != (GLint)pboHandle){
        GLDEBUG(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboHandle));
    }
    
    // create texture
    GLDEBUG(glTexParameteri(dimension, GL_TEXTURE_MIN_FILTER, filterModeMin));
    GLDEBUG(glTexParameteri(dimension, GL_TEXTURE_MAG_FILTER, filterModeMag));
    GLDEBUG(glTexParameteri(dimension, GL_TEXTURE_WRAP_S, wrapModeU));
    GLDEBUG(glTexParameteri(dimension, GL_TEXTURE_WRAP_T, wrapModeV));
    GLDEBUG(glTexImage2D(dimension,			// target
                         0,					// mip-level
                         internalFormat,    // internal format
                         width,				// width
                         height,			// height
                         0,					// border
                         format,			// format
                         type,				// type
                         0));				// data
    
    // restore previously bound
    if(pboBoundHandle != (GLint)pboHandle){
        GLDEBUG(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboBoundHandle));
    }
    if(textureBoundHandle != (GLint)pboHandle){
        GLDEBUG(glBindTexture(dimension, textureBoundHandle));
    }
    
    // mark this texture as being created
    m_created = true;
}

//! Destroy this object's data
//! \return true if destruction was successful
void Texture::destroyBuffers(){
    NIMBLE_ASSERT(m_created);
    
    if(m_textureHandle){
        GLDEBUG(glDeleteTextures(1, &m_textureHandle));
    }
    for(int32_t i = 0; i < renderdevice::kMaxTextureDimentions; i++){
        m_dimensionValues[i] = 0;
    }
    m_created = false;
}

//////////////////////////////////////////////////////////////////////////

//! returns the usage
//! \return the usage
uint32_t Texture::getUsage() const{
    return m_arrayBuffer.getUsage();
}
//! gets the format of this texture
renderdevice::eTextureFormat Texture::getFormat() const{
    return m_textureFormat;
}
//! gets the number of dimensions of this texture
renderdevice::eTextureDimension Texture::getDimension() const{
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
    NIMBLE_ASSERT(m_textureFormat != renderdevice::kUnknownTextureFormat);
    return gTexturePixelSizeMap[m_textureFormat];
}
//! gets the texture size in bytes
uint32_t Texture::getBufferSize() const{
    NIMBLE_ASSERT(m_dimensionValues[0] > 0);
    NIMBLE_ASSERT(m_dimensionValues[1] > 0);
    return m_dimensionValues[0] * m_dimensionValues[1] * getBytesPerPixel();
}
//! gets the texture's sort index
uint32_t Texture::getSortIndex() const{
    return m_textureHandle;
}

//////////////////////////////////////////////////////////////////////////

//! sets the filter mode
void Texture::setFilterMode(renderdevice::eTextureFilterParameter param, renderdevice::eTextureFilterMode value){
    m_filterMode[param] = value;
    
    GLuint handle = this->getTextureHandle();
    uint32_t dimension  = gTextureDimensionMap[this->getDimension()];
    uint32_t filterParam = gTextureFilterParamValueMap[param];
    uint32_t filterMode = gTextureFilterModeValueMap[value];
    
    // track previously bound
    GLint boundHandle;
    GLDEBUG(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundHandle));
    if(boundHandle != (GLint)handle){
        GLDEBUG(glBindTexture(dimension, handle));
    }
    GLDEBUG(glTexParameteri(dimension, filterParam, filterMode));
    // restore previously bound
    if(boundHandle != (GLint)handle){
        GLDEBUG(glBindTexture(dimension, boundHandle));
    }
}
//! sets the wrap mode
void Texture::setWrapMode(renderdevice::eTextureWrapParameter param, renderdevice::eTextureWrapMode value){
    m_wrapMode[param] = value;
    
    GLuint handle = this->getTextureHandle();
    uint32_t dimension  = gTextureDimensionMap[this->getDimension()];
    uint32_t wrapParam = gTextureWrapParamValueMap[param];
    uint32_t wrapMode = gTextureWrapModeValueMap[value];
    
    // track previously bound
    GLint boundHandle;
    GLDEBUG(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundHandle));
    if(boundHandle != (GLint)handle){
        GLDEBUG(glBindTexture(dimension, handle));
    }
    GLDEBUG(glTexParameteri(dimension, wrapParam, wrapMode));
    // restore previously bound
    if(boundHandle != (GLint)handle){
        GLDEBUG(glBindTexture(dimension, boundHandle));
    }
}
//! gets the texture filter mode
//! \param param the filter parameter
renderdevice::eTextureFilterMode Texture::getFilterMode(renderdevice::eTextureFilterParameter param) const{
    return m_filterMode[param];
}
//! gets the texture wrap mode
//! \param param the wrap parameeter
renderdevice::eTextureWrapMode Texture::getWrapMode(renderdevice::eTextureWrapParameter param) const{
    return m_wrapMode[param];
}
//! regenerates mipmaps
void Texture::regenerateMipMaps(){
    GLuint handle = this->getTextureHandle();
    uint32_t dimension  = gTextureDimensionMap[this->getDimension()];
    
    // track previously bound
    GLint boundHandle;
    GLDEBUG(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundHandle));
    if(boundHandle != (GLint)handle){
        GLDEBUG(glBindTexture(dimension, handle));
    }
    GLDEBUG(glGenerateMipmap(dimension));
    // restore previously bound
    if(boundHandle != (GLint)handle){
        GLDEBUG(glBindTexture(dimension, boundHandle));
    }
}

//////////////////////////////////////////////////////////////////////////

//! gets the GL handle
GLuint Texture::getTextureHandle() const{
    return m_textureHandle;
}

//////////////////////////////////////////////////////////////////////////

//! attempts to map array buffer
char* Texture::mapBuffer(core::eLockType lockType){
    return m_arrayBuffer.mapBuffer(lockType);
}
//! attempts to map array buffer
char* Texture::mapBufferRange(core::eLockType lockType, uint32_t offset, uint32_t size){
    return m_arrayBuffer.mapBufferRange(lockType, offset, size);
}
//! attempts to unmap array buffer
void Texture::unmapBuffer(){
    m_arrayBuffer.unmapBuffer();
    
    // once our buffer has been unmapped, we async transfer
    // our pbo data to our texture object.
    uint32_t width = this->getWidth();
    uint32_t height= this->getHeight();
    uint32_t dimension  = gTextureDimensionMap[this->getDimension()];
    uint32_t format = gTextureFormatMap[this->getFormat()];
    uint32_t internalFormat = gTextureInternalFormatMap[this->getFormat()];
    uint32_t type = gTextureTypeMap[this->getFormat()];
    GLuint textureHandle = this->getTextureHandle();
    GLuint pboHandle = m_arrayBuffer.getArrayHandle();
    
    // track previously bound
    GLint textureBoundHandle, pboBoundHandle;
    GLDEBUG(glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBoundHandle));
    GLDEBUG(glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pboBoundHandle));
    if(textureBoundHandle != (GLint)textureHandle){
        GLDEBUG(glBindTexture(dimension, textureHandle));
    }
    if(pboBoundHandle != (GLint)pboHandle){
        GLDEBUG(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboHandle));
    }
    GLDEBUG(glTexImage2D(dimension,			// target
                         0,					// level
                         internalFormat,    // internal format
                         width,				// width
                         height,			// height
                         0,					// border
                         format,			// format
                         type,				// type
                         0));               // data
    // restore previously bound
    if(pboBoundHandle != (GLint)pboHandle){
        GLDEBUG(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboBoundHandle));
    }
    if(textureBoundHandle != (GLint)textureHandle){
        GLDEBUG(glBindTexture(dimension, textureBoundHandle));
    }
}

//////////////////////////////////////////////////////////////////////////