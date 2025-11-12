
#include "RenderFont.hpp"
#include <algorithm>
#include "crglCore.hpp"

constexpr int width = 512;
constexpr int height = 512;

#include <SDL3_image/SDL_image.h>

FontLoader::FontLoader( const char* font, const float size ) : m_sucess( false ), m_font( nullptr ), m_atlas( nullptr )
{
    m_font = TTF_OpenFont( font, size );
	if ( !m_font )
    {
        m_sucess = false;
        return;
    }
    
    /// create a gliph attlas surface
    m_atlas = SDL_CreateSurface( width, height, SDL_PIXELFORMAT_RGBA32 );
    if( !m_atlas )
    {
        m_sucess = false;
        return;
    }

    // create a white transparent background
    SDL_PixelFormatDetails formatDetails{};
    formatDetails.format = SDL_PIXELFORMAT_RGBA32;
    SDL_FillSurfaceRect( m_atlas, nullptr, SDL_MapRGBA( &formatDetails, nullptr, 255, 255, 255, 0 ) );

    int x = 0, y = 0, rowHeight = 0;

    for ( int ch = 0; ch < 256; ++ch ) 
    {

        SDL_Surface* glyphSurface = nullptr;
#if 0
        glyphSurface = TTF_RenderGlyph_Blended( m_font, ch, { 255, 255, 255, 255 });
#else
        glyphSurface = TTF_RenderGlyph_Solid( m_font, ch, { 255, 255, 255, 255 } );
#endif 
        if (!glyphSurface) 
            continue;

        if (x + glyphSurface->w >= width ) 
        {
            x = 0;
            y += rowHeight + 1;
            rowHeight = 0;
        }

        SDL_Rect dst = { x, y, glyphSurface->w, glyphSurface->h };
        SDL_BlitSurface( glyphSurface, nullptr, m_atlas, &dst);

        rowHeight = std::max(rowHeight, glyphSurface->h);

        glyph_t& g = m_glyphs[ch];
        g.u0 = float(x) / width;
        g.v0 = float(y) / height;
        g.u1 = float(x + glyphSurface->w) / width;
        g.v1 = float(y + glyphSurface->h) / height;
        g.w = glyphSurface->w;
        g.h = glyphSurface->h;

        TTF_GetGlyphMetrics( m_font, ch, &g.bearingX, nullptr, &g.bearingY, nullptr, &g.advance );

        x += glyphSurface->w + 1;
        SDL_DestroySurface( glyphSurface );
    }

#if 0 // just to help degub
    IMG_SavePNG( m_atlas, "font_data.png" );
#endif

    m_sucess = true;
}

FontLoader::~FontLoader( void )
{
    m_sucess = false;
    
    if ( m_atlas )
    {
        SDL_DestroySurface( m_atlas );
        m_atlas = nullptr;
    }

    if ( m_font != nullptr )
    {
        TTF_CloseFont( m_font );
        m_font = nullptr;
    }    
}

bool FontLoader::IsOpen( void ) const
{
    return m_sucess;
}

void FontLoader::GetRenderFont( renderFont_t* font )
{
    // upload to OpenGL
    
    font->texture.Create( GL_TEXTURE_2D, GL_RGBA8, 1, 1, { m_atlas->w, m_atlas->h, 1 } );
    font->texture.SubImage( 0, {0, 0, 0}, { m_atlas->w, m_atlas->h, 1 }, m_atlas->pixels, false );

    // copy the glyphs
    std::memcpy( font->glyphs, m_glyphs, sizeof( glyph_t ) * 256 );
}
