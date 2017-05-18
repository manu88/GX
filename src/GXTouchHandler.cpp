//
//  GXTouchHandler.cpp
//  GX
//
//  Created by Manuel Deneu on 18/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include "GXTouchHandler.hpp"


GXTouchHandler::GXTouchHandler():
_firstResponder( nullptr )
{
    
}

bool GXTouchHandler::onEvent( const GXEvent* event)
{
    return false;
}
