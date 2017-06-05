//
//  NG.h
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef NG_h
#define NG_h

#include "nanovg/nanovg_gl.h"
#include "nanovg/nanovg_gl_utils.h"

#ifdef __APPLE__
#	define GLFW_INCLUDE_GLCOREARB
#endif

#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#elif defined USE_DISPMAN
#include "Display/piGL.h"
#endif
#include "nanovg/nanovg.h"

/*
#ifdef USE_GLFW
#define NANOVG_GL3_IMPLEMENTATION
#elif defined USE_DISPMAN
#define NANOVG_GLES2_IMPLEMENTATION
#endif
 */


#endif /* NG_h */
