//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_2_0_shaderprogram__
#define __nimble_renderdevice_opengles_2_0_shaderprogram__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice-opengles_2_0/common.h>
#include <nimble/renderdevice/ishaderprogram.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_2_0{
            class RenderDevice;
            class Shader;
            
            //! Shader program
            class ShaderProgram
            : public renderdevice::IShaderProgram
            , public resource::Resource{
            friend class nimble::renderdevice::opengles_2_0::RenderDevice;
            private:
                
                renderdevice::opengles_2_0::Shader *m_pVertexShader;
                renderdevice::opengles_2_0::Shader *m_pPixelShader;
                
                GLuint  m_programHandle;
                GLint   m_uniformHandles[renderdevice::IShaderProgram::kMaxAttributeTypes];
                
            public:
                
                //! Constructor
                ShaderProgram();
                //! Destructor
                virtual ~ShaderProgram();
                
            public:
                
                //! compiles this program
                virtual void compile(renderdevice::IShader *pVertexShader, renderdevice::IShader *pPixelShader);
                //! sets a param
                virtual void setParam(const char *name, renderdevice::IShaderProgram::eAttributeType type, void* pData);
                
            public:
                
                //! Returns the shader program handle
                GLuint getShaderProgramHandle() const;
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////