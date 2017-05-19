//
//  GXFont.cpp
//  GX
//
//  Created by Manuel Deneu on 19/05/2017.
//  Copyright © 2017 Unlimited Development. All rights reserved.
//

#include "GXFont.hpp"
#include "GXContext.hpp"

GXFontManager::GXFontManager(GXContext* context):
_context(context)
{
    
}

GXFontHandle GXFontManager::getFont( const std::string &file)
{
    if( _fontMap.count(file))
    {
        return _fontMap.at(file);
    }
    const GXFontHandle handle = _context->createFont(file);
    
    _fontMap.insert(std::make_pair(file, handle));
    
    return handle;
}
