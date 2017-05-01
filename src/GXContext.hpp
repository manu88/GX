//
//  GXContext.h
//  nanoVGTest3
//
//  Created by Manuel Deneu on 27/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXContext_h
#define GXContext_h



#include "NG.h"



typedef struct
{
    NVGcontext* _ctx;
}GXContext;

int GXContextInit( GXContext* ctx);
int GXContextRelease( GXContext* ctx);



#endif /* GXContext_h */
