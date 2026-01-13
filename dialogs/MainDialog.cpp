/*
===============================================================
===============================================================
*/

#include "precompiled.hpp"
#include "MainDialog.hpp"

#include "Gwen/Renderers/OpenGL.h"
#include "Gwen/Skins/TexturedBase.h"

/*
==========================
crMainDialog::crMainDialog
==========================
*/
crMainDialog::crMainDialog( gl::Context *in_pContext ) : Gwen::Controls::Base( nullptr, "mainDialog" )
{
    // Note: Order is important here. you need to create the WindowCanvas before
	// you setup the skin - because the renderer won't be properly set up
	// if you try to do it before (The window canvas initializes the renderer)
	
    //
	// Create the skin and renderer
    m_renderer = new Gwen::Renderer::OpenGL( in_pContext );
    m_renderer->Init();
    m_renderer->ResizedContext( nullptr, 800, 600 );

    //
    // Load skin texture and fonts
    m_skin = new Gwen::Skin::TexturedBase( m_renderer );

#if 0
    // The window canvas is a cross between a window and a canvas
	// It's cool because it takes care of creating an OS specific
	// window - so we don't have to bother with all that crap.
    m_canvas = new Gwen::Controls::WindowCanvas( -1, -1, 800, 600, m_skin, "md5ModelStudio" );
#else
    m_canvas = new Gwen::Controls::Canvas( m_skin );
    m_canvas->SetBackgroundColor( { 77, 77, 77, 255} );
    m_canvas->SetDrawBackground( true  );
    m_canvas->SetSize( { 800, 600 } );
#endif

    // Now it's safe to set up the skin
    dynamic_cast<Gwen::Skin::TexturedBase*>(m_skin)->Init( "./assets/images/gwen_dark.png" );    

#if 1
    dynamic_cast<Gwen::Skin::TexturedBase*>(m_skin)->SetDefaultFont( L"./assets/fonts/Inconsolata.ttf", 14.0f );
#else
    dynamic_cast<Gwen::Skin::TexturedBase*>(m_skin)->SetDefaultFont( L"./assets/fonts/OpenSans.ttf", 14.0f );
#endif 

    //
    // Create a Windows Control helper
	// (Processes Windows MSG's and fires input at GWEN)
    m_events.Initialize( m_canvas );
	
    /// 
    /// Create Program Layout
    /// 

    /// 
    /// menu barr
    m_menu = new Gwen::Controls::MenuStrip( m_canvas );
    m_menu->Dock( Gwen::Pos::Top );

    Gwen::Controls::MenuItem* pRoot = m_menu->AddItem( L"File" );
    pRoot->GetMenu()->AddItem( L"Load", "", "Ctrl+L" )->SetAction( this, &crMainDialog::MenuLoad );
    pRoot->GetMenu()->AddItem( L"Save", "", "CTRL+S" )->SetAction( this, &crMainDialog::MenuSave );
    pRoot->GetMenu()->AddItem( L"Import", "", "Ctrl+Shift+S" )->SetAction( this, &crMainDialog::MenuImport );
    pRoot->GetMenu()->AddItem( L"Export", "", "Ctrl+Shift+S" )->SetAction( this, &crMainDialog::MenuExport );
    pRoot->GetMenu()->AddItem( L"Quit", "", "Ctrl+Q" )->SetAction( this, &crMainDialog::MenuQui );

    ///
    /// Status Barr
    m_statusbar = new Gwen::Controls::StatusBar( m_canvas );
	m_statusbar->Dock( Gwen::Pos::Bottom );

    ///
    /// left table 
    Gwen::Controls::Layout::Table* pCenter = new Gwen::Controls::Layout::Table( m_canvas );
    pCenter->Dock( Gwen::Pos::Fill );

    Gwen::Controls::Layout::TableRow* pLeft = new Gwen::Controls::Layout::TableRow( pCenter );
    pLeft->SetBounds( 5, 5, 200, 200 );
    pLeft->Dock( Gwen::Pos::Left );

    Gwen::Controls::Layout::TableRow* pRight = new Gwen::Controls::Layout::TableRow( pCenter );
    pRight->Dock( Gwen::Pos::Fill );

    m_dockControlLeft = new Gwen::Controls::TabControl( pLeft );
	m_jointsTab = m_dockControlLeft->AddPage( L"Joints" );
	m_meshesTab = m_dockControlLeft->AddPage( L"Meshes" );
    m_dockControlLeft->Dock( Gwen::Pos::Fill );
    
#if 0		
	Gwen::Controls::Base* pPage = pButton->GetPage();
	Controls::RadioButtonController* pRadio = new Controls::RadioButtonController( pPage );
	pRadio->SetBounds( 10, 10, 100, 100 );
	pRadio->AddOption( "Top" )->Select();
	pRadio->AddOption( "Bottom" );
	pRadio->AddOption( "Left" );
	pRadio->AddOption( "Right" );
	pRadio->onSelectionChange.Add( m_canvas, &ThisClass::OnDockChange );
#endif

#if 1
    m_viewport = new Gwen::Controls::DrawPanel( pRight, "Viewport" ); 
    m_viewport->Dock( Gwen::Pos::Fill );
#else
    m_viewport = new crViewPort( pRight );
    m_viewport->Dock( Gwen::Pos::Fill );
#endif

    m_statusbar->SetText( "Welcome to MD5Studio" );
}


/*
==========================
crMainDialog::~crMainDialog
==========================
*/
crMainDialog::~crMainDialog( void )
{
    if ( m_canvas )
    {
        delete m_canvas;
        m_canvas = nullptr;
    }

    if( m_skin )
    {
        delete m_skin;
        m_skin = nullptr;
    }

    if ( m_renderer )
    {
        m_renderer->Release();
        delete m_renderer;
        m_renderer = nullptr;
    }    
}

/*
==========================
crMainDialog::MenuLoad
==========================
*/
bool crMainDialog::Events( const SDL_Event &in_event )
{    
    // if window resize, update renderer  
    if ( in_event.type == SDL_EVENT_WINDOW_RESIZED )
    {
        m_renderer->ResizedContext( nullptr, in_event.window.data1, in_event.window.data2 );
    }

    m_events.ProcessEvent( const_cast<SDL_Event*>( &in_event ) );
    
    return true;
}

void crMainDialog::Draw(void)
{
    m_canvas->RenderCanvas();
}

void crMainDialog::SetFrameTime(const uint32_t time)
{
    char frametime[64]{};
    SDL_snprintf( frametime, 64, "Frame time %i ms", time );
    m_statusbar->SetText( frametime );
}

/*
==========================
crMainDialog::MenuLoad
==========================
*/
void crMainDialog::MenuLoad(Gwen::Controls::Base *pControl)
{
    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuckk" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Loading model %s", file.c_str() ) );
    }

    // we always clear the scene before load a new model
    Clear();

    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuckk" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Model %s loaded sucefully", file.c_str() ) );
    }
}

/*
==========================
crMainDialog::MenuSave
==========================
*/
void crMainDialog::MenuSave(Gwen::Controls::Base *pControl)
{
    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuckk" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Saving model %s", file.c_str() ) );
    }

    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuckk" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Model %s saved sucefully", file.c_str() ) );
    }
}

/*
==========================
crMainDialog::MenuExport
==========================
*/
void crMainDialog::MenuExport(Gwen::Controls::Base *pControl)
{
    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuck" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Exporting model %s", file.c_str() ) );
    }

    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuck" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Model %s exported sucefully", file.c_str() ) );
    }    

}

/*
==========================
crMainDialog::MenuImport
==========================
*/
void crMainDialog::MenuImport(Gwen::Controls::Base *pControl)
{
    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuckk" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Importing model %s", file.c_str() ) );
    }

    Clear();

    if ( m_statusbar )
    {
        Gwen::UnicodeString file = Gwen::Utility::StringToUnicode( "fuckk" );
        m_statusbar->SetText( Gwen::Utility::Format( L"Model %s imported sucefully", file.c_str() ) );
    }
}

/*
==========================
crMainDialog::MenuQui
==========================
*/
void crMainDialog::MenuQui(Gwen::Controls::Base *pControl)
{
    m_canvas->InputQuit();
}

