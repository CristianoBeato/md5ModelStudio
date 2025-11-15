/*
===============================================================
===============================================================
*/

#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include "Gwen/Gwen.h"
#include "Gwen/Controls.h"

class crViewPort;
class crMain : public Gwen::Controls::Base
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
    Gwen::Renderer::Base*           m_renderer;
    Gwen::Skin::Base*               m_skin;
    Gwen::Controls::WindowCanvas*   m_canvas;
    Gwen::Controls::MenuStrip*      m_menu;
    Gwen::Controls::StatusBar*      m_statusbar;
    Gwen::Controls::TabControl*     m_dockControlLeft;
    Gwen::Controls::TabButton*      m_jointsTab;
    Gwen::Controls::TabButton*      m_meshesTab;
    Gwen::Controls::DrawPanel*      m_viewport;

    void    CreateUI( void );
    void    DestroyUI( void );

    // Events function
    void MenuLoad( Gwen::Controls::Base* pControl );
    void MenuSave( Gwen::Controls::Base* pControl );
    void MenuExport( Gwen::Controls::Base* pControl );
    void MenuImport( Gwen::Controls::Base* pControl );
    void MenuQui( Gwen::Controls::Base* pControl );

};

#endif //__MAIN_HPP__