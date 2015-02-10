//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengl_2_0_shaderparamblock_h__
#define __nimble_renderdevice_openg_2_0_shaderparamblock_h__

//////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <nimble/renderdevice/ishaderparamblock.h>
#include <nimble/renderdevice/opengl/shaderparam.h>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
    namespace renderdevice{
        class IShaderParam;
        
        namespace opengl_2_0{
            
            //! Tracks a set of shader parameters and their values. This
            //! is a much more efficient way of updating the shader program
            //! uniform buffer than setting one at a time.
            class ShaderParamBlock
            : public renderdevice::IShaderParamBlock{
            private:
                
                typedef std::vector<renderdevice::opengl_2_0::ShaderParam> ParamList;
                typedef std::map<int32_t, int32_t> NameToParamIndex;
                
                ParamList m_paramList;
                NameToParamIndex m_nameToParamIndex;
                
            public:
                
                //! Constructor
                ShaderParamBlock();
                //! Constructor
                ShaderParamBlock(renderdevice::opengl_2_0::ShaderParamBlock const &block);
                //! Destructor
                virtual ~ShaderParamBlock();
                
            public:
                
                //! Returns the number of parameters
                virtual size_t getNumShaderParams() const;
                
                //! Returns true if param with name exists
                //! \return true if param with name exists
                virtual bool existsParamWithName(const char *name) const;
                
                //! Returns the shader param by index
                virtual renderdevice::IShaderParam const* getShaderParamByIndex(size_t index) const;
                //! Returns the shader param by name
                virtual renderdevice::IShaderParam const* getShaderParamByName(const char *name) const;
                
                //! Sets shader param by index
                virtual void setShaderParamByIndex(size_t index, void *pData);
                //! Sets shader param by name
                virtual void setShaderParamByName(const char *name, void *pData);
                
                //! Adds a shader param
                virtual void addShaderParam(renderdevice::IShaderParam const &param);
                
                //! Clears all parameters
                virtual void clear();
            };
        };
    };
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////