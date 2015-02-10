//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_1_1_renderdevice_h__
#define __nimble_renderdevice_opengles_1_1_renderdevice_h__

//////////////////////////////////////////////////////////////////////////

#include <nimble/renderdevice-opengles_1_1/common.h>
#include <nimble/renderdevice/irenderdevice.h>
#include <nimble/core/allocator.stl.h>
#include <nimble/math/rect.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////

namespace nimble{
	namespace renderdevice{
        namespace opengles_1_1{

            //! lists and iterators
            typedef std::vector<ITexture*, core::Allocator<ITexture*> >                 TextureList;
            typedef TextureList::iterator												TextureIterator;
            typedef std::vector<math::Matrix4x4f, core::Allocator<math::Matrix4x4f > >  MatrixList;
            typedef MatrixList::iterator												MatrixIterator;
            typedef std::vector<unsigned int, core::Allocator<unsigned int> >			MatrixModeStack;
            typedef MatrixModeStack::iterator											MatrixModeIterator;

            //! fixed state information
            struct renderContext_t{
                IFrameBuffer*                               m_pFrameBuffer;

                IVertexBuffer*                              m_pVertexBuffer;
                IIndexBuffer*                               m_pIndexBuffer;
                
                int                                         m_vertexAttributeEnabledMask;
                
                IShader*                                    m_pVertexShader;
                IShader*                                    m_pPixelShader;
                
                math::Rect2i                                m_viewport;
                
                int                                         m_currentTextureUnitIndex;
                TextureList                                 m_textureArray;

                MatrixList                                  m_modelMatrixStack;
                MatrixList                                  m_finalModelMatrixStack;
                math::Matrix4x4f                            m_modelMatrix;
                math::Matrix4x4f                            m_topModelMatrix;
                math::Matrix4x4f                            m_finalModelMatrix;
                bool                                        m_finalModelMatrixChanged;

                MatrixList                                  m_viewMatrixStack;
                MatrixList                                  m_finalViewMatrixStack;
                math::Matrix4x4f                            m_viewMatrix;
                math::Matrix4x4f                            m_topViewMatrix;
                math::Matrix4x4f                            m_finalViewMatrix;
                bool                                        m_finalViewMatrixChanged;

                MatrixList                                  m_projectionMatrixStack;
                MatrixList                                  m_finalProjectionMatrixStack;
                math::Matrix4x4f                            m_projectionMatrix;
                math::Matrix4x4f                            m_topProjectionMatrix;
                math::Matrix4x4f                            m_finalProjectionMatrix;
                bool                                        m_finalProjectionMatrixChanged;

                math::Matrix4x4f                            m_finalMatrix;
                MatrixModeStack                             m_matrixModeStack;
                renderdevice::IRenderDevice::eMatrixMode    m_currentMatrixMode;

                bool                                        m_renderStates[renderdevice::IRenderDevice::kMaxStates];
                
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
                virtual void draw(core::UInt32 startIndex, core::UInt32 numIndices);
                
                //! starts rendering a frame
                virtual void beginFrame();
                //! ends rendering a frame
                virtual void endFrame();
                
            public:

                //! clear frame buffer
                //! \param bufferMask the set of buffers to clear
                virtual void clearBuffer(core::UInt32 bufferMask);
                
                //! sets our clear color
                //! \param r the red component
                //! \param g the green component
                //! \param b the blue component
                //! \param a the alpha component
                virtual void setClearColor(core::Float r, core::Float g, core::Float b, core::Float a);

            public:

                //! Depth functions
                //! \param func the depth compare mode
                virtual void setDepthFunc(renderdevice::IRenderDevice::eDepthFunc func);
                //! Stencil functions
                //! \param func the stencil compare mode
                virtual void setStencilFunc(renderdevice::IRenderDevice::eStencilFunc func);
                //! Cull functions
                //! \param face the face to cull
                virtual void setCullFace(renderdevice::IRenderDevice::eCullFace face);
                //! Front face orientation function
                //! \param orientation the face orientation
                virtual void setFrontFaceOrientation(renderdevice::IRenderDevice::eOrientation orientation);
                //! Blend function
                //! \param srcScaler the src scaler component
                //! \param destScaler the dest scaler component
                virtual void setBlendFunc(renderdevice::IRenderDevice::eBlendScaler srcScaler, renderdevice::IRenderDevice::eBlendScaler destScaler);
                
                //! Enable function
                //! \param enables a state
                virtual void enableState(renderdevice::IRenderDevice::eState state);
                //! Disable function
                //! \param disables a state
                virtual void disableState(renderdevice::IRenderDevice::eState state);
                
                //! sets viewport
                //! \param x the viewport x coordinate
                //! \param y the viewport y coordinate
                //! \param width the viewport width
                //! \param height the viewport height
                virtual void setViewport(core::UInt32 x, core::UInt32 y, core::UInt32 width, core::UInt32 height);
                
            public:
                
                //! sets vertex array
                //! \param pVertexBuffer the vertex array to bind
                virtual bool bindVertexBuffer(renderdevice::IVertexBuffer *pVertexBuffer, core::Bool force = false);
                //! sets index array
                //! \param pIndexBuffer the index array to bind
                virtual bool bindIndexBuffer(renderdevice::IIndexBuffer *pIndexBuffer, core::Bool force = false);
                //! sets light
                //! \param textureUnit the index of the texture
                //! \param pTexture the texture data
                //! \return true if successful
                virtual bool bindTexture(core::UInt32 textureUnit, renderdevice::ITexture *pTexture, core::Bool force = false);
                //! sets frame buffer
                //! \param pFrameBuffer the frame buffer
                //! \return true if successful
                virtual bool bindFrameBuffer(renderdevice::IFrameBuffer *pFrameBuffer, core::Bool force = false);
                //! sets a program
                //! \param pShaderProgram the program
                //! \return true if successful
                virtual bool bindShaderProgram(renderdevice::IShaderProgram *pShaderProgram);

            public:

                //! sets matrix
                //! \param matrix the matrix data
                virtual void setMatrix(math::Matrix4x4f& matrix);
                //! gets matrix 
                //! \param matrixMode the matrix mode we are interested in
                //! \return the matrix associated with the matrix mode
                virtual math::Matrix4x4f getMatrix(eMatrixMode matrixMode);

                //! push matrix onto the matrix stack
                virtual void pushMatrix();
                //! pop matrix onto the matrix stack
                virtual void popMatrix();
                //! clears all matrices including stack
                virtual void clearMatrix();

                //! sets matrix mode
                //! \param mode the matrix mode to set
                virtual void setMatrixMode(renderdevice::IRenderDevice::eMatrixMode mode);
                //! gets matrix mode
                //! \return the matrix mode
                virtual renderdevice::IRenderDevice::eMatrixMode getMatrixMode();
                //! pushes matrix mode
                virtual void pushMatrixMode();
                //! pops matrix mode
                virtual void popMatrixMode();
                
            public:
                
                //! creates a frame buffer
                //! \param descriptor the frame buffer's descriptor
                //! \return a frame buffer
                virtual renderdevice::IFrameBuffer* createFrameBuffer(core::UInt32 width, core::UInt32 height);
                
                //! creates a vertex array
                //! \param format the vertex format
                //! \param numVertices the number of vertices in this buffer
                //! \param usage the usage hints for this buffer
                //! \return a vertex array
                virtual renderdevice::IVertexBuffer* createVertexBuffer(renderdevice::VertexFormat const &format,
                                                                        core::UInt32 numVertices,
                                                                        core::UInt32 usage);
                //! creates a index array
                //! \param primitiveType the primitive type
                //! \param indexType the index type
                //! \param numindices the number of indices in this buffer
                //! \param usage the usage hints for this buffer
                //! \return a index array
                virtual renderdevice::IIndexBuffer* createIndexBuffer(renderdevice::IIndexBuffer::ePrimitiveType primitiveType,
                                                                      renderdevice::IIndexBuffer::eIndexType indexType,
                                                                      core::UInt32 numIndices,
                                                                      core::UInt32 usage);
                //! creates a texture
                //! \param width the width of the texture
                //! \param height the height of the texture
                //! \param format the format of the texture
                //! \param usage the usage hints for this texture
                //! \return a texture
                virtual renderdevice::ITexture* createTexture(core::UInt32 width,
                                                              core::UInt32 height,
                                                              renderdevice::ITexture::eFormat format,
                                                              core::UInt32 usage);
                
                //! creates a shader
                //! \param type the shader type
                //! \return a shader
                virtual renderdevice::IShader* createShader(renderdevice::IShader::eType type);
                //! creates a shader program
                //! \return a shader program
                virtual renderdevice::IShaderProgram* createShaderProgram();
                //! creates shader params
                //! \return shader params
                virtual renderdevice::IShaderParams* createShaderParams();

            private:
                
                //! sets the render state depth func
                virtual void setGLRenderStateDepthFunc(renderdevice::IRenderDevice::eDepthFunc func);
                //! sets the render state stencil func
                virtual void setGLRenderStateStencilFunc(renderdevice::IRenderDevice::eStencilFunc func);
                //! sets the render state cull face
                virtual void setGLRenderStateCullFace(renderdevice::IRenderDevice::eCullFace face);
                //! sets the render state blend func
                virtual void setGLRenderStateBlendFunc(renderdevice::IRenderDevice::eBlendScaler srcScaler, renderdevice::IRenderDevice::eBlendScaler destScaler);
                //! sets the render state front face orientation
                virtual void setGLRenderStateFrontFaceOrientation(renderdevice::IRenderDevice::eOrientation orientation);
                
                //! sets our transformation matrix
                virtual void setGLMatrix(renderdevice::IRenderDevice::eMatrixMode matrixMode);
                //! sets the render state matrix mode
                virtual void setGLRenderStateMatrixMode(renderdevice::IRenderDevice::eMatrixMode matrixMode);
                
                //! sets the render state depth write
                virtual void setGLRenderStateDepthWrite(core::Bool enabled);
                //! sets the render state depth test mode
                virtual void setGLRenderStateDepthTest(core::Bool enabled);
                //! sets the render state stencil test
                virtual void setGLRenderStateStencilTest(core::Bool enabled);
                //! sets the render state cull test
                virtual void setGLRenderStateCullTest(core::Bool enabled);
                //! sets the blend state
                virtual void setGLRenderStateBlending(core::Bool enabled);
                //! sets the render state light mode
                virtual void setGLRenderStateLighting(core::Bool enabled);
                //! sets the render state texturing
                virtual void setGLRenderStateTexturing(core::Bool enabled);
                
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