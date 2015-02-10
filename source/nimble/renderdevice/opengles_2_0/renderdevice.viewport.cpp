//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#include <nimble/renderdevice-opengles_2_0/renderdevice.h>

//////////////////////////////////////////////////////////////////////////

using namespace nimble;
using namespace nimble::renderdevice::opengles_2_0;

//////////////////////////////////////////////////////////////////////////

//! sets viewport
//! \param x the viewport x coordinate
//! \param y the viewport y coordinate
//! \param width the viewport width
//! \param height the viewport height
void RenderDevice::setViewport(core::UInt32 x, core::UInt32 y, core::UInt32 width, core::UInt32 height){
    m_context.m_viewport.setOrigin(math::Point2i(x, y));
    m_context.m_viewport.setSize(math::Vector2i(width, height));
    glViewport(x, y, width, height);
}

//////////////////////////////////////////////////////////////////////////