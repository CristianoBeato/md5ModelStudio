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

crMain::crMain( void )
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
        m_canvas->DoThink();
    }

    DestroyUI();

    SDL_Quit();
}

void crMain::Open( const std::string &in_file )
{
    // we always clear the scene before load a new model
    Clear();

}

void crMain::Save( const std::string &in_file )
{
    // save model to file
}

void crMain::Import( const std::string &in_file )
{
    Clear();
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
    Gwen::Renderer::Base* renderer = new Gwen::Renderer::OpenGL();
    Gwen::Skin::Base* skin = new Gwen::Skin::TexturedBase( renderer );

    // The window canvas is a cross between a window and a canvas
	// It's cool because it takes care of creating an OS specific
	// window - so we don't have to bother with all that crap.
    m_canvas = new Gwen::Controls::WindowCanvas( -1, -1, 700, 500, skin, "Gwen's Cross Platform Example" );

    // Now it's safe to set up the skin
#if 1
    dynamic_cast<Gwen::Skin::TexturedBase*>(skin)->Init( "images/gwen_dark.png" );    
#else
    dynamic_cast<Gwen::Skin::TexturedBase*>(skin)->Init( "images/DefaultSkin.png" );
#endif
}

void crMain::DestroyUI(void)
{
    if ( m_canvas )
    {
        delete m_canvas;
        m_canvas = nullptr;
    }
    
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