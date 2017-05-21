//
//  GXText.hpp
//  GX
//
//  Created by Manuel Deneu on 20/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXTextContainer_hpp
#define GXTextContainer_hpp

#include <string>
#include <vector>
#include "GXGeometry.hpp"
#include "GXFont.hpp"
#include "GXColor.hpp"

class GXContext;

class GXTextContainer
{
public:
    
    struct HitTest
    {
        GXPoint point;
        size_t textOffset;
        GXPoint textPos;
    };
    
    typedef std::vector<GXTextContainer::HitTest> HitPointList;
    
    /* **** **** **** **** **** **** **** */
    
    
    GXTextContainer(GXContext* context);
    
    void setSize( const GXSize &size) noexcept;
    const GXSize &getSize() const noexcept
    {
        return _size;
    }
    
    void setContent( const std::string &c) noexcept;
    const std::string &getContent() const noexcept
    {
        return _content;
    }
    
    void addHitTest( const GXPoint &p )
    {
        _hitList.push_back( GXTextContainer::HitTest{ p , 0 ,GXPointMakeNull() });
        //insert(std::make_pair(p, GXText::HitTest()));
    }
    
    const HitPointList& getHitTestResults() const noexcept
    {
        return _hitList;
    }
    
    void draw( const GXPoint &pos);

    void setFontSize( float) noexcept;
    void setFontId( GXFontHandle id) noexcept;
    void setTextColor( const GXColor &color) noexcept;
private:
    
    
    
    GXContext* _context;
    GXSize _size;
    std::string _content;
    
    HitPointList _hitList;
    
    
};

#endif /* GXTextContainer_hpp */
