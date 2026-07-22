/*
===============================================================
===============================================================
*/

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include <SDL3/SDL_video.h>

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
    void    Events( void );
    void    Draw( void );
};

#endif //__MAIN_HPP__