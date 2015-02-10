//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_1_1_common_h__
#define __nimble_renderdevice_opengles_1_1_common_h__

//////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
	#include "TargetConditionals.h"
	#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
		#import <OpenGLES/ES1/gl.h>
		#import <OpenGLES/ES1/glext.h>
    #else
        #pragma message("Warning - trying to include OpenGLES drivers for unsupported platform")
	#endif
#else
    #pragma message("Warning - trying to include OpenGLES drivers for unsupported platform")
#endif

//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////