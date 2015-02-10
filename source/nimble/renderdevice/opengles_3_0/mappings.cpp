//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/mappings.h>
#include <nimble/renderdevice/opengles_3_0/texture.h>
#include <nimble/core/ilockable.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;

//////////////////////////////////////////////////////////////////////////

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gTextureDimensionMap[renderdevice::kMaxTextureDimentions] ={
    -1,
    GL_TEXTURE_2D,
    -1};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gTextureUnitMap[renderdevice::kMaxTextureUnits] ={
    GL_TEXTURE0,
    GL_TEXTURE1,
    -1,
    -1};

//! maps RenderDevice enums to GL
#define GLES_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) SRCFORMAT,
int nimble::renderdevice::opengles_3_0::gTextureFormatMap[renderdevice::kMaxTextureFormats] ={
    GLES_TEXTURE_TUPLE_SET
};
#undef GLES_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
#define GLES_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) DESTFORMAT,
int nimble::renderdevice::opengles_3_0::gTextureInternalFormatMap[renderdevice::kMaxTextureFormats] ={
    GLES_TEXTURE_TUPLE_SET
};
#undef GLES_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
#define GLES_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) TYPE,
int nimble::renderdevice::opengles_3_0::gTextureTypeMap[renderdevice::kMaxTextureFormats] ={
    GLES_TEXTURE_TUPLE_SET
};
#undef GLES_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
#define GLES_TEXTURE_TUPLE(SRCFORMAT, DESTFORMAT, TYPE, PIXELSIZE) PIXELSIZE,
int nimble::renderdevice::opengles_3_0::gTexturePixelSizeMap[renderdevice::kMaxTextureFormats] ={
    GLES_TEXTURE_TUPLE_SET
};
#undef GLES_TEXTURE_TUPLE

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gTextureWrapParamValueMap[renderdevice::kMaxTextureWrapParams] ={
    GL_TEXTURE_WRAP_S,
    GL_TEXTURE_WRAP_T
};
//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gTextureWrapModeValueMap[renderdevice::kMaxTextureWrapModes] ={
    GL_CLAMP_TO_EDGE,
    -1,
    GL_REPEAT};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gTextureFilterParamValueMap[renderdevice::kMaxTextureFilterParams] ={
    GL_TEXTURE_MAG_FILTER,
    GL_TEXTURE_MIN_FILTER};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gTextureFilterModeValueMap[renderdevice::kMaxTextureFilterModes] ={
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_LINEAR};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gVertexAttributeTypeMap[renderdevice::VertexFormat::kMaxAttributeTypes] ={
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
int nimble::renderdevice::opengles_3_0::gVertexAttributeTypeDimensionMap[renderdevice::VertexFormat::kMaxAttributeTypes] ={
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
int nimble::renderdevice::opengles_3_0::gIndexFormatMap[renderdevice::kMaxPrimitiveTypes] ={
    GL_TRIANGLES,
    GL_TRIANGLE_FAN,
    GL_TRIANGLE_STRIP,
    0,
    GL_POINTS,
    GL_LINES};

//! maps RenderDevice enums to GL
int nimble::renderdevice::opengles_3_0::gIndexIndexTypeSizeMap[renderdevice::kMaxIndexTypes] ={
    1,
    2,
    4};

//////////////////////////////////////////////////////////////////////////