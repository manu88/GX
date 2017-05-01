#ifndef PIGL_H
#define PIGL_H


#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "bcm_host.h"

#define check() assert(glGetError() == 0)

typedef struct
{
   uint32_t screen_width;
   uint32_t screen_height;
// OpenGL|ES objects
   EGLDisplay display;
   EGLSurface surface;
   EGLContext context;

   GLuint verbose;

} PiGLState;




void init_ogl( PiGLState *state);


#ifdef __cplusplus
}
#endif


#endif /* DEFINED PIGL_H*/
