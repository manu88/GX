//
//  GXEvent.h
//  GX
//
//  Created by Manuel Deneu on 02/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXEvent_h
#define GXEvent_h

#include "GXKey.h"

/* *** *** *** *** *** *** *** *** *** */

typedef enum
{
    GXEventTypeUnkown   = 0,
    GXEventTypeKey      = 1,
    GXEventTypeMouse    = 2,
    
} GXEventType;

typedef struct
{
    uint16_t type;
} GXEvent;

typedef struct
{
    uint16_t type;
    
    GXKeyAction action;
    GXKeyCode   code;
    GXKeyMod    mod;
    int         scanCode;
} GXEventKey;

/* *** *** *** *** *** *** *** *** *** */

typedef enum
{
    GXMouseButtonLeft  = 0,
    GXMouseButtonRight = 1
}GXMouseButton;


typedef enum
{
    GXMouseStatePressed  = 0,
    GXMouseStateReleased = 1
}GXMouseState;


typedef struct
{
    uint16_t type;
    float x;
    float y;
    GXMouseButton button;
    GXMouseState state;
} GXEventMouse;


#endif /* GXEvent_h */
