//
//  GXColor.hpp
//  nanoVGTest
//
//  Created by Manuel Deneu on 11/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXColor_h
#define GXColor_h

#include "NG.h"

typedef NVGcolor GXColor;


namespace GXColors
{
    static constexpr const GXColor White = {1. , 1. , 1. , 1.};
    static constexpr const GXColor Black = {0. , 0. , 0. , 1.};
    static constexpr const GXColor Red   = {1. , 0. , 0. , 1.};
    static constexpr const GXColor Green = {0. , 1. , 0. , 1.};
    static constexpr const GXColor Blue  = {0. , 0. , 1. , 1.};
    
    static constexpr const GXColor LightGray  = {0.94f , 0.94f , 0.94f, 1.};
    static constexpr const GXColor DarkGray  = {0.4f , 0.4f , 0.4f, 1.};
}

static inline GXColor GXColorMake( float r , float g , float b , float a = 1.f)
{
    return (GXColor) {r , g , b , a};
}

static inline GXColor GXColorMakeWithWhite( float w ,  float a = 1.f)
{
    return (GXColor) {w , w , w , a};
}



static inline bool operator==(const GXColor& lhs, const GXColor& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

#endif /* GXColor_h */
