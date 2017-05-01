//
//  nanovg_gl.c
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//


//#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"


static GLint defaultFBO = -1;

#if defined NANOVG_GL2
void nvgDeleteGL2(NVGcontext* ctx)
#elif defined NANOVG_GL3
void nvgDeleteGL3(NVGcontext* ctx)
#elif defined NANOVG_GLES2
void nvgDeleteGLES2(NVGcontext* ctx)
#elif defined NANOVG_GLES3
void nvgDeleteGLES3(NVGcontext* ctx)
#endif
{
    nvgDeleteInternal(ctx);
}


void nvgluDeleteFramebuffer(NVGLUframebuffer* fb)
{
#ifdef NANOVG_FBO_VALID
    if (fb == NULL) return;
    if (fb->fbo != 0)
        glDeleteFramebuffers(1, &fb->fbo);
    if (fb->rbo != 0)
        glDeleteRenderbuffers(1, &fb->rbo);
    if (fb->image >= 0)
        nvgDeleteImage(fb->ctx, fb->image);
    fb->ctx = NULL;
    fb->fbo = 0;
    fb->rbo = 0;
    fb->texture = 0;
    fb->image = -1;
    free(fb);
#else
    NVG_NOTUSED(fb);
#endif
}

NVGLUframebuffer* nvgluCreateFramebuffer(NVGcontext* ctx, int w, int h, int imageFlags)
{
#ifdef NANOVG_FBO_VALID
    GLint defaultFBO;
    GLint defaultRBO;
    NVGLUframebuffer* fb = NULL;
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &defaultRBO);
    
    fb = (NVGLUframebuffer*)malloc(sizeof(NVGLUframebuffer));
    if (fb == NULL) goto error;
    memset(fb, 0, sizeof(NVGLUframebuffer));
    
    fb->image = nvgCreateImageRGBA(ctx, w, h, imageFlags | NVG_IMAGE_FLIPY | NVG_IMAGE_PREMULTIPLIED, NULL);
    
#if defined NANOVG_GL2
    fb->texture = nvglImageHandleGL2(ctx, fb->image);
#elif defined NANOVG_GL3
    fb->texture = nvglImageHandleGL3(ctx, fb->image);
#elif defined NANOVG_GLES2
    fb->texture = nvglImageHandleGLES2(ctx, fb->image);
#elif defined NANOVG_GLES3
    fb->texture = nvglImageHandleGLES3(ctx, fb->image);
#endif
    
    fb->ctx = ctx;
    
    // frame buffer object
    glGenFramebuffers(1, &fb->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fb->fbo);
    
    // render buffer object
    glGenRenderbuffers(1, &fb->rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, fb->rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, w, h);
    
    // combine all
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D/* GL_TEXTURE_2D*/, fb->texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fb->rbo);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) goto error;
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, defaultRBO);
    return fb;
error:
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, defaultRBO);
    nvgluDeleteFramebuffer(fb);
    return NULL;
#else
    NVG_NOTUSED(ctx);
    NVG_NOTUSED(w);
    NVG_NOTUSED(h);
    NVG_NOTUSED(imageFlags);
    return NULL;
#endif
}


void nvgluBindFramebuffer(NVGLUframebuffer* fb)
{
#ifdef NANOVG_FBO_VALID
    if (defaultFBO == -1) glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, fb != NULL ? fb->fbo : defaultFBO);
#else
    NVG_NOTUSED(fb);
#endif
}

#if defined NANOVG_GL2
NVGcontext* nvgCreateGL2(int flags)
#elif defined NANOVG_GL3
NVGcontext* nvgCreateGL3(int flags)
#elif defined NANOVG_GLES2
NVGcontext* nvgCreateGLES2(int flags)
#elif defined NANOVG_GLES3
NVGcontext* nvgCreateGLES3(int flags)
#endif
{
    NVGparams params;
    NVGcontext* ctx = NULL;
    GLNVGcontext* gl = (GLNVGcontext*)malloc(sizeof(GLNVGcontext));
    if (gl == NULL) goto error;
    memset(gl, 0, sizeof(GLNVGcontext));
    
    memset(&params, 0, sizeof(params));
    params.renderCreate = glnvg__renderCreate;
    params.renderCreateTexture = glnvg__renderCreateTexture;
    params.renderDeleteTexture = glnvg__renderDeleteTexture;
    params.renderUpdateTexture = glnvg__renderUpdateTexture;
    params.renderGetTextureSize = glnvg__renderGetTextureSize;
    params.renderViewport = glnvg__renderViewport;
    params.renderCancel = glnvg__renderCancel;
    params.renderFlush = glnvg__renderFlush;
    params.renderFill = glnvg__renderFill;
    params.renderStroke = glnvg__renderStroke;
    params.renderTriangles = glnvg__renderTriangles;
    params.renderDelete = glnvg__renderDelete;
    params.userPtr = gl;
    params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;
    
    gl->flags = flags;
    
    ctx = nvgCreateInternal(&params);
    if (ctx == NULL) goto error;
    
    return ctx;
    
error:
    // 'gl' is freed by nvgDeleteInternal.
    if (ctx != NULL) nvgDeleteInternal(ctx);
    return NULL;
}


#if defined NANOVG_GL2
GLuint nvglImageHandleGL2(NVGcontext* ctx, int image)
#elif defined NANOVG_GL3
GLuint nvglImageHandleGL3(NVGcontext* ctx, int image)
#elif defined NANOVG_GLES2
GLuint nvglImageHandleGLES2(NVGcontext* ctx, int image)
#elif defined NANOVG_GLES3
GLuint nvglImageHandleGLES3(NVGcontext* ctx, int image)
#endif
{
    GLNVGcontext* gl = (GLNVGcontext*)nvgInternalParams(ctx)->userPtr;
    GLNVGtexture* tex = glnvg__findTexture(gl, image);
    return tex->tex;
}

