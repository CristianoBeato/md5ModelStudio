
#include "RenderFont.hpp"
#include <algorithm>
#include "crglCore.hpp"

RenderFont::RenderFont( void )
{
}

RenderFont::~RenderFont( void )
{
}

bool RenderFont::OpenFont(const char *fontFace, const float size)
{   

    TTF_Font* font = TTF_OpenFont( fontFace, size );
	if ( !font )
		return false;

    width = 512;
    height = 512;

    /// create a gliph attlas surface
    SDL_Surface* atlasSurface = SDL_CreateSurface( width, height, SDL_PIXELFORMAT_RGBA32 );

    // fill background as black
    SDL_PixelFormatDetails formatDetails{};
    formatDetails.format = SDL_PIXELFORMAT_RGBA32;
    Uint32 bgColor = SDL_MapRGBA( &formatDetails, nullptr, 0, 0, 0, 0 );
    SDL_FillSurfaceRect( atlasSurface, nullptr, bgColor );

    int x = 0, y = 0, rowHeight = 0;

    for (int ch = 32; ch < 128; ++ch) 
    {
        SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(font, ch, {255, 255, 255, 255});
        if (!glyphSurface) 
            continue;

        if (x + glyphSurface->w >= width ) 
        {
            x = 0;
            y += rowHeight + 1;
            rowHeight = 0;
        }

        SDL_Rect dst = {x, y, glyphSurface->w, glyphSurface->h};
        SDL_BlitSurface(glyphSurface, nullptr, atlasSurface, &dst);

        rowHeight = std::max(rowHeight, glyphSurface->h);

        GlyphInfo& g = glyphs[ch];
        g.u0 = float(x) / width;
        g.v0 = float(y) / height;
        g.u1 = float(x + glyphSurface->w) / width;
        g.v1 = float(y + glyphSurface->h) / height;
        g.w = glyphSurface->w;
        g.h = glyphSurface->h;

        TTF_GetGlyphMetrics( font, ch, &g.bearingX, nullptr, &g.bearingY, nullptr, &g.advance );

        x += glyphSurface->w + 1;
        SDL_DestroySurface( glyphSurface );
    }

    // upload to OpenGL
    texture.Create( GL_TEXTURE_2D, GL_RGBA8, 1, 1, { width, height, 1 } );
    texture.SubImage( 0, {0, 0, 0}, { width, height, 1 }, atlasSurface->pixels, false );

    SDL_DestroySurface( atlasSurface );
    TTF_CloseFont( font );

    return true;
}

void RenderFont::Close(void)
{
    texture.Destroy();
}
