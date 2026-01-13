/*
===============================================================
===============================================================
*/

#ifndef __MAIN_DIALOG_HPP__
#define __MAIN_DIALOG_HPP__

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mutex.h>

#include "Gwen/Input/SDL3.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls.h"

class crMainDialog : public Gwen::Controls::Base
{
public:
    crMainDialog( gl::Context *in_pContext );
    ~crMainDialog( void );

    /// @brief handle user input events
    bool    Events( const SDL_Event &in_event );

    /// @brief sumit draw 
    void    Draw( void );
    
    void    SetFrameTime( const uint32_t time );

protected:
    virtual void Clear( void ) {}

private:
    Gwen::Input::SDL3               m_events;
    Gwen::Renderer::Base*           m_renderer;
    Gwen::Skin::Base*               m_skin;
    Gwen::Controls::Canvas*         m_canvas;
    Gwen::Controls::MenuStrip*      m_menu;
    Gwen::Controls::StatusBar*      m_statusbar;
    Gwen::Controls::TabControl*     m_dockControlLeft;
    Gwen::Controls::TabButton*      m_jointsTab;
    Gwen::Controls::TabButton*      m_meshesTab;
    Gwen::Controls::DrawPanel*      m_viewport;

    // Events function
    void MenuLoad( Gwen::Controls::Base* pControl );
    void MenuSave( Gwen::Controls::Base* pControl );
    void MenuExport( Gwen::Controls::Base* pControl );
    void MenuImport( Gwen::Controls::Base* pControl );
    void MenuQui( Gwen::Controls::Base* pControl );
};

#endif //!__MAIN_DIALOG_HPP__