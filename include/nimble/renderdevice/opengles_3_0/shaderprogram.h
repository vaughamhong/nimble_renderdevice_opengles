//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_shaderprogram__
#define __nimble_renderdevice_opengles_3_0_shaderprogram__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_3_0/common.h>
#include <nimble/renderdevice/opengles_3_0/shaderparam.h>
#include <nimble/renderdevice/opengles_3_0/shaderparamblock.h>
#include <nimble/renderdevice/ishaderprogram.h>
#include <nimble/resource/resource.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_3_0{
            class RenderDevice;
            class Shader;
            
            //! Shader program
            class ShaderProgram
            : public renderdevice::IShaderProgram
            , public resource::Resource{
                friend class nimble::renderdevice::opengles_3_0::RenderDevice;
            private:
                
                GLuint                                          m_programHandle;
                renderdevice::opengles_3_0::ShaderParamBlock    m_paramBlock;
                
            public:
                
                //! Constructor
                ShaderProgram();
                //! Destructor
                virtual ~ShaderProgram();
                
            public:
                
                //! gets the shader program's sort index
                virtual uint32_t getSortIndex() const;
                
            public:
                
                //! compiles this program
                //! \param[in] pVertexShader the vertex shader to compile
                //! \param[in] pPixelShader the pixel shader to compile
                virtual void compile(renderdevice::IShader *pVertexShader, renderdevice::IShader *pPixelShader);
                
            public:
                
                //! Returns true if param with name exists
                //! \return true if param with name exists
                virtual bool existsParamWithName(const char *name) const;
                //! Returns the shader parameter by name
                //! \param[in] name the name of the parameter to retreive
                //! \return returns the shader parameter
                virtual renderdevice::IShaderParam const* getShaderParamByName(const char *name) const;
                //! Returns the shader param block for this program
                //! \return the shader param block for this program
                virtual renderdevice::IShaderParamBlock* createShaderParamBlock(const char *names[] = 0, size_t numNames = 0);
                
            public:
                
                //! Sets a shader parameter by name
                //! \param[in] name the name of the shader parameter to set
                //! \param[in] pData the data to set the shader parameter
                virtual void setShaderParamByName(const char *name, void* pData);
                //! Sets a shader parameter block
                //! \param[in] pBlock the shader parameter block to set
                virtual void setShaderParamBlock(renderdevice::IShaderParamBlock const &block);
                
            public:
                
                //! Returns the shader program handle
                GLuint getShaderProgramHandle() const;
                
            private:
                
                //! Sets a param
                virtual void setGLUniformParam(renderdevice::opengles_3_0::ShaderParam const &param, void *pData);
            };
        };
	};
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////