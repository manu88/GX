//
//  GXFont.hpp
//  GX
//
//  Created by Manuel Deneu on 19/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#ifndef GXFont_hpp
#define GXFont_hpp

#include <string>
#include <unordered_map>


typedef int GXFontHandle;
static const GXFontHandle GXFontInvalid = -1;

class GXContext;

class GXFontManager
{
    typedef std::unordered_map< std::string, GXFontHandle> FontMap;
public:
    GXFontManager(GXContext* context);
    
    
    GXFontHandle getFont( const std::string &file);
    
    const FontMap& getFontMap() const
    {
        return _fontMap;
    }
    
private:
    FontMap _fontMap;
    GXContext* _context;
};

#endif /* GXFont_hpp */
