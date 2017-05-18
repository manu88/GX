//
//  GXTouchHandler.hpp
//  GX
//
//  Created by Manuel Deneu on 18/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXTouchHandler_hpp
#define GXTouchHandler_hpp

#include <stdio.h>
#include "GXEvent.h"

class GXTouchResponder;

class GXTouchHandler
{
public:
    GXTouchHandler();
    bool onEvent( const GXEvent* event);
    
    void setFirstResponder( GXTouchResponder* firstResponder )
    {
        _firstResponder = firstResponder;
    }
private:
    GXTouchResponder* _firstResponder;
    
};
#endif /* GXTouchHandler_hpp */
