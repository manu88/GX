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
#include "GXGeometry.hpp"

class GXContext
{
public:
    GXContext();
    ~GXContext();
    
    bool beginFrame( const GXSize &size , float pixRatio) noexcept;
    bool endFrame() noexcept;
    
    NVGcontext* _ctx;
};





#endif /* GXContext_h */
