//
//  GXGeometry.hpp
//  nanoVGTest
//
//  Created by Manuel Deneu on 11/04/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXGeometry_h
#define GXGeometry_h

#include <algorithm> // std::max / min
#include <vector>


struct GXPoint
{
    int x;
    int y;
    
    GXPoint operator-() const
    {
        return GXPoint{-this->x, -this->y};
    }
    
};

typedef struct GXPoint GXPoint;

typedef struct
{
    int width;
    int height;
    
}GXSize;

typedef struct
{
    GXPoint origin;
    GXSize  size;
    
} GXRect;

static const GXRect GXRectNull    = { 0,0,0,0};
static const GXRect GXRectInvalid = { -1,-1,-1,-1};

static inline GXSize GXSizeMake(int w , int  h)
{
    return { w , h};
}
static inline GXSize GXSizeMakeNull()
{
    return { 0 , 0};
}

static inline GXPoint GXPointMake(int x , int  y)
{
    return { x , y};
}
static inline GXPoint GXPointMakeNull()
{
    return { 0 , 0};
}

static inline GXRect GXRectMake(const GXPoint &p , const GXSize& s)
{
    return { p.x , p.y , s.width  ,s.height };
}
                                

static inline GXRect GXRectMake(int x , int y , int w , int h)
{
    return { x , y , w  ,h};
}

static inline GXRect GXRectMakeNull()
{
    return { 0,0,0,0};
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline bool operator==(const GXPoint& lhs, const GXPoint& rhs)
{
    return (    ( lhs.x == rhs.x )
            &&  ( lhs.y == rhs.y )
            );
}

static inline bool operator==(const GXSize& lhs, const GXSize& rhs)
{
    return (    ( lhs.width == rhs.width )
            &&  ( lhs.height == rhs.height )
            );
    
}

static inline bool operator==(const GXPoint& lhs, const GXSize& rhs)
{
    return (    ( lhs.x == rhs.width )
            &&  ( lhs.y == rhs.height )
            );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline bool operator!=(const GXPoint& lhs, const GXPoint& rhs)
{
    return !( lhs == rhs);
}

static inline bool operator!=(const GXSize&  lhs, const GXSize&  rhs)
{
    return !( lhs == rhs);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline bool operator==(const GXRect& lhs, const GXRect& rhs)
{
    return (    ( lhs.origin == rhs.origin )
            &&  ( lhs.size   == rhs.size )
            );
}


static inline bool operator!=(const GXRect& lhs, const GXRect& rhs)
{
    return !( lhs == rhs);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline GXRect operator+=(const GXRect& lhs, const GXPoint& rhs)
{
    return  GXRectMake(lhs.origin.x    + rhs.x,
                     lhs.origin.y    + rhs.y,
                     lhs.size.width  ,
                     lhs.size.height );
}



static inline GXRect operator-=(const GXRect& lhs, const GXPoint& rhs)
{
    return  GXRectMake(lhs.origin.x    - rhs.x,
                     lhs.origin.y    - rhs.y,
                     lhs.size.width  ,
                     lhs.size.height );
}

static inline GXRect operator+(const GXRect& lhs, const GXPoint& rhs)
{
    return  GXRectMake(lhs.origin.x    + rhs.x,
                     lhs.origin.y    + rhs.y,
                     lhs.size.width  ,
                     lhs.size.height );
}
static inline GXRect operator-(const GXRect& lhs, const GXPoint& rhs)
{
    return  GXRectMake(lhs.origin.x    - rhs.x,
                     lhs.origin.y    - rhs.y,
                     lhs.size.width ,
                     lhs.size.height);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline GXPoint operator+(const GXPoint& lhs, const GXPoint& rhs)
{
    return GXPoint {  lhs.x + rhs.x ,  lhs.y + rhs.y };
}

static inline const GXPoint& operator+=( GXPoint& lhs, const GXPoint& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
    //return GXPoint {  lhs.x + rhs.x ,  lhs.y + rhs.y };
}

static inline GXPoint operator-(const GXPoint& lhs, const GXPoint& rhs)
{
    return GXPoint {  lhs.x - rhs.x ,  lhs.y - rhs.y };
}

static inline GXPoint operator+(const GXPoint& lhs, const GXSize& rhs)
{
    return GXPoint {  lhs.x + rhs.width ,  lhs.y + rhs.height };
}



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline GXSize operator+(const GXSize& lhs, const GXSize& rhs)
{
    return GXSize {  lhs.width + rhs.width ,  lhs.height + rhs.height };
}

static inline GXSize operator-(const GXSize& lhs, const GXSize& rhs)
{
    return GXSize {  lhs.width - rhs.width ,  lhs.height - rhs.height };
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/
/*
 Geometry tests
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** *****/

static inline bool rectContainsPoint( const GXRect &rect ,const GXPoint &pt )
{
    return (    ( pt.x >= rect.origin.x  ) && ( pt.x <= rect.origin.x + rect.size.width  )
            && ( pt.y >= rect.origin.y  ) && ( pt.y <= rect.origin.y + rect.size.height  )
            );
}



static inline bool rectIntersectsRect(const GXRect &rect1 ,const GXRect &rect2 )
{
    int l1 = rect1.origin.x;
    int r1 = l1;
    if (rect1.size.width < 0)
        l1 += rect1.size.width;
    else
        r1 += rect1.size.width;
    
    if (l1 == r1) // null rect
        return false;
    
    int l2 = rect2.origin.x;
    int r2 = l2;
    if (rect2.size.width < 0)
        l2 += rect2.size.width;
    else
        r2 += rect2.size.width;
    
    if (l2 == r2) // null rect
        return false;
    
    if (l1 >= r2 || l2 >= r1)
        return false;
    
    int t1 = rect1.origin.y;
    int b1 = t1;
    if ( rect1.size.height < 0)
        t1 += rect1.size.height;
    else
        b1 += rect1.size.height;
    
    if (t1 == b1) // null rect
        return false;
    
    int t2 = rect2.origin.y;
    int b2 = t2;
    if (rect2.size.height < 0)
        t2 += rect2.size.height;
    else
        b2 += rect2.size.height;
    
    if (t2 == b2) // null rect
        return false;
    
    if (t1 >= b2 || t2 >= b1)
        return false;
    
    return true;
}

static inline GXRect rectSum( const GXRect &rect1 ,const GXRect &rect2 )
{
    if( rect1 == GXRectInvalid)
        return rect2;
    
    if( rect2 == GXRectInvalid)
        return rect1;
    
    GXPoint min;
    min.x = rect1.origin.x < rect2.origin.x ? rect1.origin.x : rect2.origin.x;
    min.y = rect1.origin.y < rect2.origin.y ? rect1.origin.y : rect2.origin.y;
    
    const GXPoint p1 = GXPointMake( rect1.origin.x + rect1.size.width  , rect1.origin.y + rect1.size.height );
    const GXPoint p2 = GXPointMake( rect2.origin.x + rect2.size.width  , rect2.origin.y + rect2.size.height );
    
    GXPoint max;
    max.x = p1.x > p2.x ? p1.x : p2.x;
    max.y = p1.y > p2.y ? p1.y : p2.y;
    
    
    
    return GXRectMake(min.x , min.y, max.x - min.x , max.y - min.y);
    
}
/*
static inline GXRect rectSum( const std::vector<const GXRect&> &rectangles)
{

    
    GXRect ret = GXRectInvalid;
    
    for (auto const r : rectangles)
    {
        ret = rectSum(ret, r);
    }
    
    return ret;
}
*/

static inline GXRect rectIntersection( const GXRect &rect1 ,const GXRect &rect2 )
{
    /**/
    int l1 = rect1.origin.x;
    int r1 = rect1.origin.x;
    if (rect1.size.width < 0)
        l1 += rect1.size.width;
    else
        r1 += rect1.size.width;
    
    if (l1 == r1) // null rect
        return GXRectMakeNull();
    
    int l2 = rect2.origin.x;
    int r2 = rect2.origin.x;
    if (rect2.size.width < 0)
        l2 += rect2.size.width;
    else
        r2 += rect2.size.width;
    
    if (l2 == r2) // null rect
        return GXRectMakeNull();
    
    if (l1 >= r2 || l2 >= r1)
        return GXRectMakeNull();
    
    int t1 = rect1.origin.y;
    int b1 = rect1.origin.y;
    if (rect1.size.height < 0)
        t1 += rect1.size.height;
    else
        b1 += rect1.size.height;
    if (t1 == b1) // null rect
        return GXRectMakeNull();
    
    int t2 = rect2.origin.y;
    int b2 = rect2.origin.y;
    if (rect2.size.height < 0)
        t2 += rect2.size.height;
    else
        b2 += rect2.size.height;
    
    if (t2 == b2) // null rect
        return GXRectMakeNull();
    
    if (t1 >= b2 || t2 >= b1)
        return GXRectMakeNull();
    
    GXRect tmp;
    tmp.origin.x = std::max(l1, l2);
    tmp.origin.y = std::max(t1, t2);
    tmp.size.width = std::min(r1, r2) - tmp.origin.x;
    tmp.size.height = std::min(b1, b2) - tmp.origin.y;
    return tmp;
    /**/
    
}




#endif /* GXGeometry_h */
