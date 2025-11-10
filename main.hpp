/*
===============================================================
===============================================================
*/

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

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
};

#endif //__MAIN_HPP__