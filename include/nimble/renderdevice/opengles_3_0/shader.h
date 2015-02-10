//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_shader__
#define __nimble_renderdevice_opengles_3_0_shader__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_3_0/common.h>
#include <nimble/renderdevice/ishader.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{
            class RenderDevice;
            class ShaderProgram;
            
            //! Shader resource
            class Shader
            : public renderdevice::IShader
            , public resource::Resource{
                friend class nimble::renderdevice::opengles_3_0::RenderDevice;
            private:
                
                GLuint                      m_shaderHandle;
                renderdevice::eShaderType   m_type;
                
            public:
                
                //! Constructor
                Shader(renderdevice::eShaderType type);
                //! Destructor
                virtual ~Shader();
                
            public:
                
                //! Get shader type
                virtual eShaderType getShaderType() const;
                //! compiles the shader with source data
                //! \param data the data to copy
                virtual bool compileWithSource(const char *pData);
                
            public:
                
                //! Returns the shader handle
                GLuint getShaderHandle() const;
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////