//
//  GXText.hpp
//  GX
//
//  Created by Manuel Deneu on 19/06/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXText_h
#define GXText_h

typedef enum   : int
{
    
    
    // Horizontal align
    GXTextAlign_LEFT 	  = 1<<0,	// Default, align text horizontally to left.
    GXTextAlign_CENTER 	  = 1<<1,	// Align text horizontally to center.
    GXTextAlign_RIGHT 	  = 1<<2,	// Align text horizontally to right.
    // Vertical align
    GXTextAlign_TOP 	  = 1<<3,	// Align text vertically to top.
    GXTextAlign_MIDDLE	  = 1<<4,	// Align text vertically to middle.
    GXTextAlign_BOTTOM	  = 1<<5,	// Align text vertically to bottom.
    GXTextAlign_BASELINE  = 1<<6, // Default, align text vertically to baseline.
    
    GXTextAlign_Default = GXTextAlign_LEFT | GXTextAlign_TOP
} GXTextAlign;

static inline GXTextAlign operator|( const GXTextAlign &a , const GXTextAlign &b)
{
    return static_cast<GXTextAlign>( static_cast<int>(a) | static_cast<int>(b) );
}

static inline GXTextAlign operator|=( const GXTextAlign &a , const GXTextAlign &b)
{
    return static_cast<GXTextAlign>( static_cast<int>(a) | static_cast<int>(b) );
}

static inline GXTextAlign ResetHorizontal( GXTextAlign a)
{
    int aa = (int)a;
    
    aa &=  ~(1 << 0);
    aa &=  ~(1 << 1);
    aa &=  ~(1 << 2);
    
    return (GXTextAlign) aa;
}


#endif /* GXText_h */
