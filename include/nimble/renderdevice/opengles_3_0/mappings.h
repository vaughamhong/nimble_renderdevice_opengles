//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_mappings_h__
#define __nimble_renderdevice_opengles_3_0_mappings_h__

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{
            
            //! maps RenderDevice enums to GL
            extern int gTextureDimensionMap[];
            extern int gTextureUnitMap[];
            extern int gTextureFormatMap[];
            extern int gTextureInternalFormatMap[];
            extern int gTextureTypeMap[];
            extern int gTexturePixelSizeMap[];
            extern int gTextureWrapParamValueMap[];
            extern int gTextureWrapModeValueMap[];
            extern int gTextureFilterParamValueMap[];
            extern int gTextureFilterModeValueMap[];
            extern int gVertexAttributeTypeMap[];
            extern int gVertexAttributeTypeDimensionMap[];
            extern int gIndexFormatMap[];
            extern int gIndexIndexTypeSizeMap[];
            extern int gBufferLockMap[];
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////