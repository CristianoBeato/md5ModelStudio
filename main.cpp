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
#include "renderer/renderer.hpp"

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

crMain::crMain( void ) : 
    m_state( 0 ),

{
    std::cout << "Initializing SDL3\n";
    // initialize SDL3 video ( window ) and event management
    if( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) )
        throw std::runtime_error( SDL_GetError() );

    std::cout << "Creating main window\n";

    /// Prepare the renderer
    crRenderer::Get()->Create();

    /// Prepare the interface
    crMainDialog::Get()->Create();

    m_state = 1;
}

crMain::~crMain( void )
{   
    std::cout << "Releasing main window\n";

    /// Release the main dialog
    crMainDialog::Get()->Destroy();

    /// Release the renderer
    crRenderer::Get()->Destroy();

    std::cout << "Release SDL3\n";
    SDL_Quit();
}

void crMain::Run(void)
{
    while( m_state )
    {
        // process event input
        Events();
        Draw();
    }
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

void crMain::Events(void)
{
    SDL_Event Event{};
    SDL_WaitEvent( &Event );

    if ( Event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED || Event.type == SDL_EVENT_WINDOW_DESTROYED )
    {   
        m_state = 0; // done
        return;
    }

     
    // pass input event to the gui
    crMainDialog::Get()->Events( Event );
}

void crMain::Draw(void)
{
    crMainDialog::Get()->Draw();
    crRenderer::Get()->Draw();
}

int main(int argc, char *argv[])
{
    bool gui = true;
    std::string importModel;
    std::string exportModel;
    std::string loadModel;
    std::string saveModel;

    for (int i = 0; i < argc; i++)
    {
        const char* arg = argv[i];
        if ( std::strncmp( arg, k_HELP, std::strlen( k_HELP ) ) == 0 )
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
