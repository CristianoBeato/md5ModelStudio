/*
===============================================================
===============================================================
*/

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <SDL3/SDL_video.h>
#include "crglContext.hpp"
#include "dialogs/MainDialog.hpp"

class crContext : public gl::Context
{
public:
    crContext( void );
    ~crContext( void );

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

class crMain
{    
public:
    crMain( void );
    ~crMain( void );

    void    Run( void );
    void    Open( const std::string &in_file );
    void    Save( const std::string &in_file );
    void    Import( const std::string &in_file );
    void    Export( const std::string &in_file );
    void    Clear( void );

private:
    uint8_t         m_state;
    uint32_t        m_frameTime;
    SDL_Mutex*      m_renderLock;
    SDL_Thread*     m_drawThread;
    crContext*      m_glContext;
    crMainDialog*   m_maindDialog;
    
    void    Events( void );
    void    Draw( void );
    void    Renderer( void );

    static int DrawThreadEntryPoint( void* ptr );
};

#endif //__MAIN_HPP__