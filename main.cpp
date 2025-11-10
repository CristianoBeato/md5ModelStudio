/*
===============================================================
===============================================================
*/
#include "precompiled.hpp"
#include "main.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

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

    while ( running )
    {
        SDL_Event evt{};
        while ( SDL_PollEvent( &evt ) )
        {
            switch ( evt.type )
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            
            default:
                break;
            }
        }
    }   

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