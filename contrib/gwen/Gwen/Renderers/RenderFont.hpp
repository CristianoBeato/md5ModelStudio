
#ifndef __RENDER_FONT_HPP__
#define __RENDER_FONT_HPP__

#include "crglImage.hpp"
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

struct glyph_t 
{
    float u0 = 0.0f;
    float v0 = 0.0f;
    float u1 = 0.0f;
    float v1 = 0.0f;
    int w = 0;
    int h = 0;
    int advance = 0;
    int bearingX = 0; 
    int bearingY = 0;
};

struct  renderFont_t
{
    gl::Image   texture;
    glyph_t     glyphs[256];
};

class FontLoader
{
public:
    FontLoader( const char* font, const float size );
    ~FontLoader( void );

    bool    IsOpen( void ) const;
    void    GetRenderFont( renderFont_t* font );

private:
    bool            m_sucess;
    TTF_Font*       m_font;
    SDL_Surface*    m_atlas;
    glyph_t         m_glyphs[256];
};

#endif //!__RENDER_FONT_HPP__