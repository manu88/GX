//
//  GXColor.hpp
//  nanoVGTest
//
//  Created by Manuel Deneu on 11/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXColor_h
#define GXColor_h

//#include "NG.h"
#include <unordered_map>

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} GXColor;


static constexpr GXColor GXColorInvalid = { -1.f ,-1.f,-1.f,-1.f };


namespace GXColors
{
    static constexpr const GXColor White = {1. , 1. , 1. , 1.};
    static constexpr const GXColor Black = {0. , 0. , 0. , 1.};
    static constexpr const GXColor Red   = {1. , 0. , 0. , 1.};
    static constexpr const GXColor Green = {0. , 1. , 0. , 1.};
    static constexpr const GXColor Blue  = {0. , 0. , 1. , 1.};
    
    static constexpr const GXColor Gray  = {0.7f , 0.7f , 0.7f, 1.};
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

namespace GXColors
{
    const std::unordered_map< std::string, const GXColor> ColorsNames =
    {
        { "white"       , GXColors::White},
        { "red"         , GXColors::Red},
        { "blue"        , GXColors::Blue},
        { "green"       , GXColorMake( 0 , 0.5 , 0)},
        { "yellow"      , GXColorMake( 1.0 , 1. , 0)},
        { "magenta"     , GXColorMake(0.96, 0, 0.96)},
        { "cyan"        , GXColorMake(0., 1, 1)},
        { "gray"        , GXColorMakeWithWhite(0.5)},
        { "lightgray"   , GXColorMakeWithWhite(0.83)},
        
    };
};



static inline bool operator==(const GXColor& lhs, const GXColor& rhs)
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
}

static inline bool operator!=(const GXColor& lhs, const GXColor& rhs)
{
    return !(lhs == rhs);
}

#endif /* GXColor_h */
