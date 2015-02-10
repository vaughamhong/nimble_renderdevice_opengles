//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/mappings.h>
#include <nimble/renderdevice-opengles_2_0/texture.h>
#include <nimble/core/ilockable.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;

//////////////////////////////////////////////////////////////////////////

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gTextureDimensionMap[renderdevice::ITexture::kMaxDimentions] ={
	-1,
	GL_TEXTURE_2D,
	-1};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gTextureUnitMap[renderdevice::IRenderDevice::kMaxUnits] ={
	GL_TEXTURE0,
	GL_TEXTURE1,
	-1,
    -1};

//! maps RenderDevice enums to GL
#define GLES20_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) SRCFORMAT,
int nimble::renderdevice::opengles_2_0::gTextureFormatMap[renderdevice::ITexture::kMaxFormats] ={
    GLES20_TEXTURE_TUPLE_SET
};
#undef GLES20_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
#define GLES20_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) DESTFORMAT,
int nimble::renderdevice::opengles_2_0::gTextureInternalFormatMap[renderdevice::ITexture::kMaxFormats] ={
    GLES20_TEXTURE_TUPLE_SET
};
#undef GLES20_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
#define GLES20_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) TYPE,
int nimble::renderdevice::opengles_2_0::gTextureTypeMap[renderdevice::ITexture::kMaxFormats] ={
    GLES20_TEXTURE_TUPLE_SET
};
#undef GLES20_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
#define GLES20_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) PIXELSIZE,
int nimble::renderdevice::opengles_2_0::gTexturePixelSizeMap[renderdevice::ITexture::kMaxFormats] ={
    GLES20_TEXTURE_TUPLE_SET
};
#undef GLES20_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gTextureWrapModeValueMap[renderdevice::ITexture::kMaxWrapModes] ={
	GL_CLAMP_TO_EDGE,
	GL_REPEAT};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gTextureFilterModeValueMap[renderdevice::ITexture::kMaxFilterModes] ={
	GL_NEAREST,
	GL_LINEAR};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gVertexAttributeTypeMap[renderdevice::VertexFormat::kMaxAttributeTypes] ={
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_SHORT,
    GL_SHORT,
    GL_SHORT};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gVertexAttributeTypeDimensionMap[renderdevice::VertexFormat::kMaxAttributeTypes] ={
	1,
	2,
	3,
	4,
	1,
	2,
	3,
	4,
    1,
	2,
	3,
	4};

//! maps RenderDevice enums to GL
//! OpenGL ES 2.0 doesn't support QUAD LISTS
//! TODO proper caps detection
int nimble::renderdevice::opengles_2_0::gIndexFormatMap[renderdevice::IIndexBuffer::kMaxPrimitiveTypes] ={
	GL_TRIANGLES,
	GL_TRIANGLE_FAN,
	GL_TRIANGLE_STRIP,
	0,
	GL_POINTS,
	GL_LINES};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_2_0::gIndexIndexTypeSizeMap[renderdevice::IIndexBuffer::kMaxIndexTypes] ={
    1,
	2};

//////////////////////////////////////////////////////////////////////////