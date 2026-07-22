/*
===============================================================
===============================================================
*/

#include "precompiled.hpp"
#include "renderer.hpp"

/// Fix windows 
#ifdef CreateWindow
#undef CreateWindow
#endif //CreateWindow

constexpr uint32_t FRAMERATE = 60;
constexpr uint32_t FRAMETIME = 1000 / FRAMERATE; // frametime duration in ms ( 16 for 60 fps )

crRenderContext::crRenderContext(void) :
    m_window( nullptr ),
    m_context( nullptr )
{
}

crRenderContext::~crRenderContext(void)
{
}

bool crRenderContext::Create(const void *in_windowHandle)
{
    // 4.5 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
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

void crRenderContext::Destroy(void)
{
    if( m_context != nullptr )
    {
        SDL_GL_DestroyContext( m_context );
        m_context = nullptr;
    }
}

bool crRenderContext::MakeCurrent(void)
{
    return SDL_GL_MakeCurrent( m_window, m_context );
}

bool crRenderContext::Release(void)
{
    return SDL_GL_MakeCurrent( m_window, nullptr );
}

bool crRenderContext::SwapBuffers(void)
{
    return SDL_GL_SwapWindow( m_window );
}

void *crRenderContext::GetFunctionPointer(const char *in_name) const
{
    return reinterpret_cast<void*>( SDL_GL_GetProcAddress( in_name ) );
}

void crRenderContext::DebugOuput(const char *in_message) const
{
    std::cout << in_message << std::endl;
}

crRenderer *crRenderer::Get(void)
{
    static crRenderer gRenderer = crRenderer();
    return &gRenderer;
}

crRenderer::crRenderer(void)
{
}

crRenderer::~crRenderer( void )
{
}

void crRenderer::Create( void )
{
    /// We must load the system OpenGL library, before initialize the window
    if( !SDL_GL_LoadLibrary( nullptr ) )
        throw std::runtime_error( SDL_GetError() );

    /// Create main window 
    if( !( m_window = SDL_CreateWindow( "md5ModelStudio", 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE ) ) )
        throw std::runtime_error( SDL_GetError() );

    CreateContext();
}

void crRenderer::Destroy(void)
{
    /// Destroy the main window render context 
    if( m_renderContext != nullptr )
    {
        m_renderContext->Destroy();
        delete m_renderContext;
        m_renderContext = nullptr;
    }

    /// Destroy main dialog window 
    if( m_window != nullptr )
    {
        SDL_DestroyWindow( m_window );
        m_window = nullptr;
    }

    /// Release system OpenGL window
    SDL_GL_UnloadLibrary();
}

void crRenderer::Draw(void)
{
    //
    //
    glClearColor( 0.0f, 0.6f, 0.7f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    
    // swap window buffer
    m_renderContext->SwapBuffers();
}

void crRenderer::CreateContext(void)
{
    m_renderContext = new crRenderContext();
    m_renderContext->Create( reinterpret_cast<void*>(m_window) );
}

void crRenderer::Run(void)
{
    uint64_t start = 0;
    uint64_t end = 0;
    m_renderContext->MakeCurrent();
    
    // render loop
    while ( m_running )
    {
        start = SDL_GetTicks();
        Draw();
        end = SDL_GetTicks();
        
        // limit frametime
        m_frameTime = end - start;
        SDL_Delay( m_frameTime < FRAMETIME ? FRAMETIME - m_frameTime : 0 );
    }
    
    m_renderContext->Release();
}

int crRenderer::EntryThread(void *entry)
{
    crRenderer* renderer = static_cast<crRenderer*>( entry );
    renderer->Run();
    return 0;
}
