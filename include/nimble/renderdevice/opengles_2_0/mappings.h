//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_mappings_h__
#define __nimble_renderdevice_opengles_2_0_mappings_h__

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            
            //! maps RenderDevice enums to GL
            extern int gTextureDimensionMap[];
            //! maps RenderDevice enums to GL
            extern int gTextureUnitMap[];
            //! maps RenderDevice enums to GL
            extern int gTextureFormatMap[];
            //! maps RenderDevice enums to GL
            extern int gTextureInternalFormatMap[];
            //! maps RenderDevice enums to GL
            extern int gTextureTypeMap[];
            //! maps RenderDevice enums to GL
            extern int gTexturePixelSizeMap[];
            //! maps RenderDevice enums to GL
            extern int gTextureWrapModeValueMap[];
            //! maps RenderDevice enums to GL
            extern int gTextureFilterModeValueMap[];
            
            //! maps RenderDevice enums to GL
            extern int gVertexAttributeTypeMap[];
            //! maps RenderDevice enums to GL
            extern int gVertexAttributeTypeDimensionMap[];
            
            //! maps primitive type enum to GL
            extern int gIndexFormatMap[];
            //! maps RenderDevice enums to GL
            extern int gIndexIndexTypeSizeMap[];
            
            //! maps buffer lock enums to GL
            extern int gBufferLockMap[];
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////