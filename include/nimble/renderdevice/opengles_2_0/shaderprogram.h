//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_shaderprogram__
#define __nimble_renderdevice_opengles_2_0_shaderprogram__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_2_0/common.h>
#include <nimble/renderdevice/ishaderprogram.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            class RenderDevice;
            class Shader;
            
            //! shader parameter
            class ShaderParam
            : public renderdevice::ShaderParam{
            private:
                
                GLint m_handle;
                GLint m_textureUnit;
                
            public:
                
                //! Constructor
                ShaderParam();
                //! Constructor
                ShaderParam(const char *name, renderdevice::eShaderParamType type, void *pData, GLint handle, GLint textureUnit = -1);
                //! Constructor
                ShaderParam(renderdevice::opengl::ShaderParam const &param);
                //! Destructor
                virtual ~ShaderParam();
                
            public:
                
                //! Returns the shader handle
                GLint getHandle() const;
                //! Returns the texture unit
                GLint getTextureUnit() const;
            };
            
            //! maps opengl uniform types to internal types
            renderdevice::eShaderParamType mapGLUniformTypeToInternalType(GLenum uniformType);
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////