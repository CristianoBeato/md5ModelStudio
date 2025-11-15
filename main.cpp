/*
===============================================================
===============================================================
*/
#include "precompiled.hpp"
#include "main.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Gwen/Renderers/OpenGL.h"
#include "Gwen/Skins/TexturedBase.h"
#include "Gwen/Controls/WindowCanvas.h"
#include "ViewPort.hpp"

static const char* k_NOGUI = "--nogui";
static const char* k_IMPORT = "--import";
static const char* k_EXPORT = "--export";
static const char* k_OUTPUT = "--output";
static const char* k_LOAD = "--load";
static const char* k_HELP = "--help";
static const char* k_HELP_TEXT = 
{
    "md5ModelStudio v0.1\n"
    "--nogui        run on console only ( comand line export ) \n"
    "--import       import transfer model\n"
    "--export       export a loaded model\n"
    "--output       output mesh or anim\n"
    "--load         load a mesh source or a anim source\n"
    "--help         print help\n"
};

crMain::crMain( void ) : Gwen::Controls::Base( nullptr, "main" ),
    m_renderer( nullptr ),
    m_skin( nullptr ),
    m_canvas( nullptr ),
    m_menu( nullptr ),
    m_statusbar( nullptr )
{
}

crMain::~crMain( void )
{   
}

void crMain::Run(void)
{
    bool running = true;
    if ( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) )
        throw std::runtime_error( SDL_GetError() );

    CreateUI();

    while( !m_canvas->WantsQuit() )
    {
        // render our ui
        m_canvas->DoThink();
    }

    DestroyUI();

    SDL_Quit();
}

void crMain::Open( const std::string &in_file )
{
}

void crMain::Save( const std::string &in_file )
{
}

void crMain::Import( const std::string &in_file )
{
}

void crMain::Export( const std::string &in_file )
{
}

void crMain::Clear(void)
{
}

void crMain::CreateUI(void)
{
    // Note: Order is important here. you need to create the WindowCanvas before
	// you setup the skin - because the renderer won't be properly set up
	// if you try to do it before (The window canvas initializes the renderer)
	//
	// Create the skin and renderer
    m_renderer = new Gwen::Renderer::OpenGL();
    m_skin = new Gwen::Skin::TexturedBase( m_renderer );

    // The window canvas is a cross between a window and a canvas
	// It's cool because it takes care of creating an OS specific
	// window - so we don't have to bother with all that crap.
    m_canvas = new Gwen::Controls::WindowCanvas( -1, -1, 800, 600, m_skin, "md5ModelStudio" );

    // Now it's safe to set up the skin
    dynamic_cast<Gwen::Skin::TexturedBase*>(m_skin)->Init( "./assets/images/gwen_dark.png" );    

#if 1
    dynamic_cast<Gwen::Skin::TexturedBase*>(m_skin)->SetDefaultFont( L"./assets/fonts/Inconsolata.ttf", 14.0f );
#else
    dynamic_cast<Gwen::Skin::TexturedBase*>(m_skin)->SetDefaultFont( L"./assets/fonts/OpenSans.ttf", 14.0f );
#endif 

    /// 
    /// Create Program Layout
    /// 

    /// menu barr
    m_menu = new Gwen::Controls::MenuStrip( m_canvas );
    m_menu->Dock( Gwen::Pos::Top );

    Gwen::Controls::MenuItem* pRoot = m_menu->AddItem( L"File" );
    pRoot->GetMenu()->AddItem( L"Load", "", "Ctrl+L" )->SetAction( this, &crMain::MenuLoad );
    pRoot->GetMenu()->AddItem( L"Save", "", "CTRL+S" )->SetAction( this, &crMain::MenuSave );
    pRoot->GetMenu()->AddItem( L"Import", "", "Ctrl+Shift+S" )->SetAction( this, &crMain::MenuImport );
    pRoot->GetMenu()->AddItem( L"Export", "", "Ctrl+Shift+S" )->SetAction( this, &crMain::MenuExport );
    pRoot->GetMenu()->AddItem( L"Quit", "", "Ctrl+Q" )->SetAction( this, &crMain::MenuQui );

    /// Status Barr
    m_statusbar = new Gwen::Controls::StatusBar( m_canvas );
	m_statusbar->Dock( Gwen::Pos::Bottom );

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
    m_viewport = new Gwen::Controls::DrawPanel( pRight ); 
    m_viewport->Dock( Gwen::Pos::Fill );
#else
    m_viewport = new crViewPort( pRight );
    m_viewport->Dock( Gwen::Pos::Fill );
#endif

    m_statusbar->SetText( "Welcome to MD5Studio" );

}

void crMain::DestroyUI(void)
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
        delete m_renderer;
        m_renderer = nullptr;
    }
}

void crMain::MenuLoad(Gwen::Controls::Base *pControl)
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

void crMain::MenuSave(Gwen::Controls::Base *pControl)
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

void crMain::MenuExport(Gwen::Controls::Base *pControl)
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

void crMain::MenuImport(Gwen::Controls::Base *pControl)
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

void crMain::MenuQui(Gwen::Controls::Base *pControl)
{
    m_canvas->InputQuit();
}

int main( int argc, const char* argv[] )
{
    bool gui = true;
    std::string importModel;
    std::string exportModel;
    std::string loadModel;
    std::string saveModel;

    for (int i = 0; i < argc; i++)
    {
        const char* arg = argv[i];
        if ( std::strncmp( arg, k_HELP, strlen( k_HELP ) ) == 0 )
        {
            std::cout << k_HELP_TEXT << std::endl;
            return EXIT_SUCCESS; // just print the help text
        }
        else if ( std::strncmp( arg, k_NOGUI, std::strlen( k_NOGUI ) ) == 0 )
        {
            gui = false;
        }
        else if ( std::strncmp( arg, k_IMPORT, std::strlen( k_IMPORT ) ) == 0)
        {
            // missing cmd arg
            if ( ( i + 1 ) >= argc )
                break;
            
            importModel = argv[++i];
        }
        else if ( std::strncmp( arg, k_EXPORT, std::strlen( k_EXPORT ) ) )
        {
            // missing cmd arg
            if ( ( i + 1 ) >= argc )
                break;
            
            exportModel = argv[++i];
        }
        else if ( std::strncmp( arg, k_OUTPUT, std::strlen( k_OUTPUT ) ) == 0)
        {          
            // missing cmd arg
            if ( ( i + 1 ) >= argc )
                break;
            
            saveModel = argv[++i];
        }
        else if ( std::strncmp( arg, k_LOAD, std::strlen( k_LOAD ) ) == 0)
        {
          // missing cmd arg
            if ( ( i + 1 ) >= argc )
                break;
            
            loadModel = argv[++i];
        }
    }
    
    try
    {
        crMain app = crMain();

        if ( !loadModel.empty() )
            app.Open( loadModel );
        else if( !importModel.empty() )
            app.Import( importModel );
        
        if( !saveModel.empty() )
            app.Save( saveModel );
        else if ( !exportModel.empty() )
            app.Export( exportModel );

        if ( gui )
            app.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}