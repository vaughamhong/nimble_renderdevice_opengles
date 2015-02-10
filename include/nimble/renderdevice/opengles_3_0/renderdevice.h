//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_renderdevice_h__
#define __nimble_renderdevice_opengles_3_0_renderdevice_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice/opengles_3_0/common.h>
#include <nimble/renderdevice/irenderdevice.h>
#include <nimble/math/rect.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////

#define SHADERPARAM_TUPLESET \
SHADERPARAM_TUPLE(Normal,                "r_normalMatrix") \
SHADERPARAM_TUPLE(Model,                 "r_model") \
SHADERPARAM_TUPLE(View,                  "r_view") \
SHADERPARAM_TUPLE(InvView,               "r_invView") \
SHADERPARAM_TUPLE(Projection,            "r_projection") \
SHADERPARAM_TUPLE(ModelView,             "r_modelView") \
SHADERPARAM_TUPLE(ModelViewProjection,   "r_modelViewProjection") \

//////////////////////////////////////////////////////////////////////////

namespace nimble{
    namespace renderdevice{
        namespace opengles_3_0{
            
            //! lists and iterators
            typedef std::vector<math::Matrix4x4f>   MatrixList;
            typedef MatrixList::iterator			MatrixIterator;
            typedef std::vector<unsigned int>		MatrixModeStack;
            typedef MatrixModeStack::iterator		MatrixModeIterator;
            
            //! fixed state information
            struct renderContext_t{
                // frame buffer state
                IFrameBuffer*                               m_pFrameBuffer;
                
                // vertex / index buffers state
                IVertexBuffer*                              m_pVertexBuffer;
                IIndexBuffer*                               m_pIndexBuffer;
                
                // shader program state
                IShaderProgram*                             m_pShaderProgram;
                IShader*                                    m_pVertexShader;
                IShader*                                    m_pPixelShader;
                
                // flags for shader program matrix params
#define SHADERPARAM_TUPLE(NAME, SHADERNAME) bool m_shaderHas##NAME##ShaderParam;
                SHADERPARAM_TUPLESET
#undef SHADERPARAM_TUPLE
                
                // viewport state
                math::Rect2i                                m_viewport;
                
                // texture state
                int                                         m_currentTextureUnitIndex;
                ITexture                                    *m_textures[renderdevice::kMaxTextureUnits];
                
                // flags for matrix state has changed
#define SHADERPARAM_TUPLE(NAME, SHADERNAME) bool m_final##NAME##MatrixChanged;
                SHADERPARAM_TUPLESET
#undef SHADERPARAM_TUPLE
                
                // model matrix state
                MatrixList                                  m_modelMatrixStack;
                MatrixList                                  m_finalModelMatrixStack;
                math::Matrix4x4f                            m_modelMatrix;
                math::Matrix4x4f                            m_topModelMatrix;
                math::Matrix4x4f                            m_finalModelMatrix;
                math::Matrix3x3f                            m_finalNormalMatrix;
                
                // view matrix state
                MatrixList                                  m_viewMatrixStack;
                MatrixList                                  m_finalViewMatrixStack;
                math::Matrix4x4f                            m_viewMatrix;
                math::Matrix4x4f                            m_topViewMatrix;
                math::Matrix4x4f                            m_finalViewMatrix;
                math::Matrix4x4f                            m_finalInvViewMatrix;
                
                // projection matrix state
                MatrixList                                  m_projectionMatrixStack;
                MatrixList                                  m_finalProjectionMatrixStack;
                math::Matrix4x4f                            m_projectionMatrix;
                math::Matrix4x4f                            m_topProjectionMatrix;
                math::Matrix4x4f                            m_finalProjectionMatrix;
                
                // model view matrix state
                math::Matrix4x4f                            m_finalModelViewMatrix;
                
                // model view projection matrix state
                math::Matrix4x4f                            m_finalModelViewProjectionMatrix;
                
                // matrix mode state
                MatrixModeStack                             m_matrixModeStack;
                renderdevice::eRenderMatrixMode             m_currentMatrixMode;
                
                // render states
                bool                                        m_renderStates[renderdevice::kMaxRenderStates];
                
                // delegate
                renderdevice::IRenderDevice::Delegate*      m_pRenderDeviceDelegate;
            };
            
            //! RenderDevice impl
            class RenderDevice
            : public renderdevice::IRenderDevice{
            private:
                
                renderContext_t m_context;
                
            public:
                
                //! Constructor
                RenderDevice();
                //! Destructor
                virtual ~RenderDevice();
                
            public:
                
                //! set delegate
                virtual void setDelegate(renderdevice::IRenderDevice::Delegate* pRenderDeviceDelegate);
                
                //! returns the width of the frame
                //! \return the width of the frame
                virtual unsigned int getFrameWidth();
                //! returns the height of the frame
                //! \return the height of the frame
                virtual unsigned int getFrameHeight();
                
            public:
                
                //! draws with current state
                virtual void draw();
                //! draws with current state
                virtual void drawElements(uint32_t startIndex, uint32_t numIndices);
                
                //! starts rendering a frame
                virtual void beginFrame();
                //! ends rendering a frame
                virtual void endFrame();
                
            public:
                
                //! clear frame buffer
                //! \param bufferMask the set of buffers to clear
                virtual void clearBuffer(uint32_t bufferMask);
                
                //! sets our clear color
                //! \param r the red component
                //! \param g the green component
                //! \param b the blue component
                //! \param a the alpha component
                virtual void setClearColor(float r, float g, float b, float a);
                
            public:
                
                //! Fill mode functions
                //! \param face the face to set fill mode
                //! \param mode the fill mode
                virtual void setFillMode(renderdevice::eRenderFace face, renderdevice::eRenderFillMode mode);
                
                //! Depth functions
                //! \param func the depth compare mode
                virtual void setDepthFunc(renderdevice::eRenderDepthFunc func);
                //! Stencil functions
                //! \param func the stencil compare mode
                virtual void setStencilFunc(renderdevice::eRenderStencilFunc func, int32_t ref, uint32_t mask);
                //! Stencil operation
                //! \param sfail action taken when stencil pass fails
                //! \param dfail action taken when depth pass fails
                //! \param pass action taken when stencil and depth pass succeed
                virtual void setStencilOp(renderdevice::eRenderStencilOp sfail, renderdevice::eRenderStencilOp dfail, renderdevice::eRenderStencilOp pass);
                //! Stencil operation
                //! \param mask stencil write mask
                virtual void setStencilMask(uint32_t mask);
                //! Cull functions
                //! \param face the face to cull
                virtual void setCullFace(renderdevice::eRenderCullFace face);
                //! Front face orientation function
                //! \param orientation the face orientation
                virtual void setFrontFaceOrientation(renderdevice::eRenderOrientation orientation);
                //! Blend function
                //! \param srcScaler the src scaler component
                //! \param destScaler the dest scaler component
                virtual void setBlendFunc(renderdevice::eRenderBlendFactor srcScaler, renderdevice::eRenderBlendFactor destScaler);
                
                //! Enable function
                //! \param enables a state
                virtual void enableState(renderdevice::eRenderState state);
                //! Disable function
                //! \param disables a state
                virtual void disableState(renderdevice::eRenderState state);
                
                //! sets viewport
                //! \param x the viewport x coordinate
                //! \param y the viewport y coordinate
                //! \param width the viewport width
                //! \param height the viewport height
                virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
                
            public:
                
                //! sets vertex array
                //! \param pVertexBuffer the vertex array to bind
                virtual void bindVertexBuffer(renderdevice::IVertexBuffer *pVertexBuffer);
                //! sets index array
                //! \param pIndexBuffer the index array to bind
                virtual void bindIndexBuffer(renderdevice::IIndexBuffer *pIndexBuffer);
                //! sets light
                //! \param textureUnit the index of the texture
                //! \param pTexture the texture data
                //! \return true if successful
                virtual void bindTexture(uint32_t textureUnit, renderdevice::ITexture *pTexture);
                //! sets frame buffer
                //! \param pFrameBuffer the frame buffer
                //! \return true if successful
                virtual void bindFrameBuffer(renderdevice::IFrameBuffer *pFrameBuffer);
                //! sets a program
                //! \param pShaderProgram the program
                //! \return true if successful
                virtual void bindShaderProgram(renderdevice::IShaderProgram *pShaderProgram);
                
            public:
                
                //! sets our projection matrix
                virtual void setProjectionMatrix(nimble::math::Matrix4x4f const &matrix);
                //! sets our view matrix
                virtual void setViewMatrix(nimble::math::Matrix4x4f const &matrix);
                
                //! sets matrix
                //! \param matrix the matrix data
                virtual void setMatrix(math::Matrix4x4f const &matrix);
                //! gets matrix
                //! \param matrixMode the matrix mode we are interested in
                //! \return the matrix associated with the matrix mode
                virtual math::Matrix4x4f getMatrix(eRenderMatrixMode matrixMode) const;
                
                //! push matrix onto the matrix stack
                virtual void pushMatrix();
                //! pop matrix onto the matrix stack
                virtual void popMatrix();
                //! clears all matrices including stack
                virtual void clearMatrix();
                
                //! sets matrix mode
                //! \param mode the matrix mode to set
                virtual void setMatrixMode(renderdevice::eRenderMatrixMode mode);
                //! gets matrix mode
                //! \return the matrix mode
                virtual renderdevice::eRenderMatrixMode getMatrixMode() const;
                //! pushes matrix mode
                virtual void pushMatrixMode();
                //! pops matrix mode
                virtual void popMatrixMode();
                
            public:
                
                //! creates a frame buffer
                //! \param descriptor the frame buffer's descriptor
                //! \return a frame buffer
                virtual renderdevice::IFrameBuffer* createFrameBuffer(uint32_t width, uint32_t height, uint32_t flags = 0);
                
                //! creates a vertex array
                //! \param format the vertex format
                //! \param numVertices the number of vertices in this buffer
                //! \param usage the usage hints for this buffer
                //! \return a vertex array
                virtual renderdevice::IVertexBuffer* createVertexBuffer(renderdevice::VertexFormat const &format,
                                                                        uint32_t numVertices,
                                                                        uint32_t usage);
                //! creates a index array
                //! \param primitiveType the primitive type
                //! \param indexType the index type
                //! \param numindices the number of indices in this buffer
                //! \param usage the usage hints for this buffer
                //! \return a index array
                virtual renderdevice::IIndexBuffer* createIndexBuffer(renderdevice::ePrimitiveType primitiveType,
                                                                      renderdevice::eIndexType indexType,
                                                                      uint32_t numIndices,
                                                                      uint32_t usage);
                //! creates a texture
                //! \param width the width of the texture
                //! \param height the height of the texture
                //! \param format the format of the texture
                //! \param usage the usage hints for this texture
                //! \return a texture
                virtual renderdevice::ITexture* createTexture(uint32_t width,
                                                              uint32_t height,
                                                              renderdevice::eTextureFormat format,
                                                              uint32_t usage);
                //! creates a texture from an image
                //! \param image the image to create from
                //! \param format the format of the texture
                //! \param usage the usage hints for this texture
                //! \return a texture
                virtual renderdevice::ITexture* createTextureFromImage(image::Image &image,
                                                                       uint32_t usage);
                //! creates a shader
                //! \param type the shader type
                //! \return a shader
                virtual renderdevice::IShader* createShader(renderdevice::eShaderType type);
                //! creates a shader program
                //! \return a shader program
                virtual renderdevice::IShaderProgram* createShaderProgram();
                
            private:
                
                //! Sets our shader program matrix states
                virtual void patchShaderProgramMatrixParams();
                
                //! sets the render state color write
                virtual void setGLRenderStateColorWrite(bool enabled);
                //! sets the render state depth write
                virtual void setGLRenderStateDepthWrite(bool enabled);
                //! sets the render state stencil write
                virtual void setGLRenderStateStencilWrite(bool enabled);
                //! sets the render state depth test mode
                virtual void setGLRenderStateDepthTest(bool enabled);
                //! sets the render state stencil test
                virtual void setGLRenderStateStencilTest(bool enabled);
                //! sets the render state cull test
                virtual void setGLRenderStateCullTest(bool enabled);
                //! sets the blend state
                virtual void setGLRenderStateBlending(bool enabled);
                //! sets the render state light mode
                virtual void setGLRenderStateLighting(bool enabled);
                //! sets the render state texturing
                virtual void setGLRenderStateTexturing(bool enabled);
                
            private:
                
                //! reset our context
                virtual void resetRenderContext();
            };
        };
    };
};

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////