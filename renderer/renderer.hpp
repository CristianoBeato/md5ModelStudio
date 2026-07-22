/*
===============================================================
===============================================================
*/
#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <SDL3/SDL_video.h>
#include "crglContext.hpp"

class crRenderContext : public gl::Context
{
public:
    crRenderContext( void );
    ~crRenderContext( void );

    virtual bool    Create( const void* in_windowHandle);
    virtual void    Destroy( void );
    virtual bool    MakeCurrent( void );
    virtual bool    Release( void );
    virtual bool    SwapBuffers( void );
    virtual void*   GetFunctionPointer( const char* in_name ) const;
    virtual void    DebugOuput( const char* in_message ) const;

private:    
    SDL_Window*     m_window;
    SDL_GLContext   m_context;
};

class crRenderer
{
public:
    static crRenderer*  Get( void );

    crRenderer( void );
    ~crRenderer( void );
    void    Create( void );
    void    Destroy( void );
    void    Draw( void );

private:
    bool                m_running;
    uint64_t            m_frameTime;

    /// main aplication window handler
    SDL_Window*         m_window;
    crRenderContext*    m_renderContext;

    void    CreateContext( void );
    void    Run( void );
    static int EntryThread( void* );
};

#endif //!__RENDERER_HPP__