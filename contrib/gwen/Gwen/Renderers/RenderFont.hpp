
#ifndef __RENDER_FONT_HPP__
#define __RENDER_FONT_HPP__

#include "crglImage.hpp"
#include <SDL3_ttf/SDL_ttf.h>

class RenderFont
{
public:
    struct GlyphInfo 
    {
        float u0, v0, u1, v1;
        int w = 0;
        int h = 0;
        int advance = 0;
        int bearingX = 0; 
        int bearingY = 0;
    };

    RenderFont( void );
    ~RenderFont( void );

    bool        OpenFont( const char* fontFiel, const float size );
    void        Close( void );
    GLuint      GetTexture( void ) const { return texture; }
    GlyphInfo   GetGliph( const int8_t c ) const { return glyphs[c]; } 

private:
    int         width = 0;
    int         height = 0;
    gl::Image   texture;
    GlyphInfo   glyphs[128];
};

#endif //!__RENDER_FONT_HPP__