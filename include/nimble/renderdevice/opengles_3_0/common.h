//
// Copyright (C) 2011 Vaugham Hong (vaughamhong@gmail.com)
//
// This file is subject to the terms and conditions defined in
// file 'license.txt', which is part of this source code package.
//

#ifndef __nimble_renderdevice_opengles_3_0_common_h__
#define __nimble_renderdevice_opengles_3_0_common_h__

//////////////////////////////////////////////////////////////////////////

#if defined(NIMBLE_TARGET_IOS)
	#include "TargetConditionals.h"
	#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
		#import <OpenGLES/ES3/gl.h>
		#import <OpenGLES/ES3/glext.h>
    #else
        #pragma message("Warning - trying to include OpenGLES drivers for unsupported platform")
	#endif
#elif defined(NIMBLE_TARGET_ANDROID)
    #include <GLES3/gl3.h>
#else
    #pragma message("Warning - trying to include OpenGLES drivers for unsupported platform")
#endif

#if 0//defined(NIMBLE_DEBUG)
    #define GLDEBUG(x) \
        x; \
        { \
            GLenum e; \
            while((e = glGetError()) != GL_NO_ERROR){ \
            NIMBLE_LOG_INFO("graphics", __LINE__, __FILE__, "Error at line number %d, in file %s. glGetError() returned %i for call %s\n",__LINE__, __FILE__, e, #x); \
            fprintf(stderr, "Error at line number %d, in file %s. glGetError() returned %i for call %s\n",__LINE__, __FILE__, e, #x); \
            } \
        }
#else
    #define GLDEBUG(x) x;
#endif


//////////////////////////////////////////////////////////////////////////

#endif

//////////////////////////////////////////////////////////////////////////