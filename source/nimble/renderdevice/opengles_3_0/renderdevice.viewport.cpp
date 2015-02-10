//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice/opengles_3_0/renderdevice.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_3_0;

//////////////////////////////////////////////////////////////////////////

//! sets viewport
//! \param x the viewport x coordinate
//! \param y the viewport y coordinate
//! \param width the viewport width
//! \param height the viewport height
void RenderDevice::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
    m_context.m_viewport.setOrigin(math::Vector2i(x, y));
    m_context.m_viewport.setSize(math::Vector2i(width, height));
    GLDEBUG(glViewport(x, y, width, height));
}

//////////////////////////////////////////////////////////////////////////