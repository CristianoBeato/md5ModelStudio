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
//#include "ViewPort.hpp"

constexpr uint32_t FRAMERATE = 60;
constexpr uint32_t FRAMETIME = 1000 / FRAMERATE; // frametime duration in ms ( 16 for 60 fps )

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
    m_glContext( nullptr )
{
    std::cout << "Initializing SDL3\n";
    // initialize SDL3 video ( window ) and event management
    if( !SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) )
        throw std::runtime_error( SDL_GetError() );

    m_renderLock = SDL_CreateMutex();

    std::cout << "Creating main window\n";

    m_glContext = new crContext();
    if( !m_glContext->Create( nullptr ) )
        throw std::runtime_error( "Failed to initialize render context" );

    m_maindDialog = new crMainDialog( m_glContext );

    m_glContext->Release();

    m_drawThread = SDL_CreateThread( DrawThreadEntryPoint, "DrawThread", reinterpret_cast<void*>( this ) );
    SDL_DetachThread( m_drawThread );

    m_state = 1;
}

crMain::~crMain( void )
{   
    std::cout << "Releasing main window\n";

    m_glContext->MakeCurrent();

    if( m_maindDialog != nullptr )
    {
        delete m_maindDialog;
        m_maindDialog = nullptr;
    }

    if( !m_glContext )
    {
        m_glContext->Destroy();
        delete m_glContext;
        m_glContext = nullptr;
    }

    if ( m_renderLock )
    {
        SDL_DestroyMutex( m_renderLock );
        m_renderLock = nullptr;
    }

    std::cout << "Release SDL3\n";
    SDL_Quit();
}

void crMain::Run(void)
{
    bool running = true;
 
    while( m_state )
    {
        // process event input
        Events();
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

    // lock scope
    {
        SDL_ScopedLock lock( m_renderLock );

        m_maindDialog->SetFrameTime( m_frameTime );

        // pass input event to the gui
        m_maindDialog->Events( Event );
    }
}

void crMain::Draw(void)
{
    //
    //
    glClearColor( 0.0f, 0.6f, 0.7f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    
    {
        SDL_ScopedLock lock( m_renderLock );
        m_maindDialog->Draw();
    }

    // swap window buffer
    m_glContext->SwapBuffers();
}

void crMain::Renderer(void)
{
    uint64_t start = 0;
    uint64_t end = 0;
    m_glContext->MakeCurrent();
    
    // render loop
    while ( m_state )
    {
        start = SDL_GetTicks();
        Draw();
        end = SDL_GetTicks();
        
        // limit frametime
        m_frameTime = end - start;
        SDL_Delay( m_frameTime < FRAMETIME ? FRAMETIME - m_frameTime : 0 );
    }
    
    m_glContext->Release();
}

int crMain::DrawThreadEntryPoint(void *ptr)
{
    crMain * mainApp = static_cast<crMain*>( ptr );
    mainApp->Renderer();
    return EXIT_SUCCESS;
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

crContext::crContext( void ) :
    m_window( nullptr ),
    m_context( nullptr )
{
}

crContext::~crContext(void)
{
}

bool crContext::Create(const void *in_windowHandle)
{
    //
    if( !( m_window = SDL_CreateWindow( "md5ModelStudio", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ) ) )
        throw std::runtime_error( SDL_GetError() );

    // 4.5 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    // debug output enable
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );

    // 32bit color
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    // depth stencil 24 8
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

    // sRGB enable
    SDL_GL_SetAttribute( SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1 );

    // 4x MSAA
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );

    // use double buffer
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    // Create the OpenGL Context
    if( !( m_context = SDL_GL_CreateContext( m_window ) ) )
        throw std::runtime_error( SDL_GetError() );

    gl::Context::Init();
    
    return true;
}

void crContext::Destroy(void)
{
    if( m_context != nullptr )
    {
        SDL_GL_DestroyContext( m_context );
        m_context = nullptr;
    }

    if( m_window != nullptr )
    {
        SDL_DestroyWindow( m_window );
        m_window = nullptr;
    }
}

bool crContext::MakeCurrent(void)
{
    return SDL_GL_MakeCurrent( m_window, m_context );
}

bool crContext::Release(void)
{
    return SDL_GL_MakeCurrent( m_window, nullptr );
}

bool crContext::SwapBuffers(void)
{
    return SDL_GL_SwapWindow( m_window );
}

void *crContext::GetFunctionPointer(const char *in_name) const
{
    return reinterpret_cast<void*>( SDL_GL_GetProcAddress( in_name ) );
}

void crContext::DebugOuput(const char *in_message) const
{
    std::cout << in_message << std::endl;
}
