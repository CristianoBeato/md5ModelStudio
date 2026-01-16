/*
===============================================================
===============================================================
*/

#include "precompiled.hpp"
#include "MainDialog.hpp"

#include "Gwen/Renderers/OpenGL.h"
#include "Gwen/Skins/TexturedBase.h"

constexpr GLsizeiptr k_MAX_ELEMENT_BUFFER_SIZE = sizeof( uint16_t ) * 21845;    //
constexpr GLsizeiptr k_MAX_VERTEX_BUFFER_SIZE = sizeof( viewVert_t ) * 65535;   //
constexpr GLsizeiptr k_MAX_SHADER_BUFFER_SIZE = sizeof( viewUniforms_t ) * 256; //
constexpr GLsizeiptr k_MAX_DRAW_BUFFER_SIZE = sizeof( drawElements_t ) * 256;
constexpr GLsizeiptr k_MAX_SAMPLER_BUFFER = sizeof( GLuint64 ) * 512;

constexpr GLbitfield k_BUFFER_ACESS_FLAGS = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT; // persistent coerent mapped 
constexpr GLbitfield k_BUFFER_MAP_FLAGS = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

/*
==========================
crViewPort::crViewPort
==========================
*/
crViewPort::crViewPort( gl::Context* in_ctx, Gwen::Controls::Base* pParent, const Gwen::String& pName  ) : Gwen::Controls::DrawPanel( pParent, pName ),
    m_drawJointHead( false ),
    m_drawJointLink( false ),
    m_mode( SM_SHADED ),
    m_indexes( nullptr ),
    m_vertex( nullptr ),
    m_shaderStorage( nullptr ),
    m_drawCmd( nullptr ),
    m_textures( nullptr ),
    m_program( nullptr ),
    m_vao( nullptr ),
    m_ctx( nullptr ),
    m_triangles( nullptr ),
    m_vertexes( nullptr ),
    m_uniforms( nullptr ),
    m_drawElements( nullptr ),
    m_samples( nullptr )
{
    InitVao();
}

/*
==========================
crViewPort::~crViewPort
==========================
*/
crViewPort::~crViewPort( void )
{
    if ( m_program != nullptr )
    {
        m_program->Destroy();
        delete m_program;
        m_program = nullptr;
    }   

    if ( m_drawCmd != nullptr )
    {
        m_drawCmd->Destroy();
        delete m_drawCmd;
        m_drawCmd = nullptr;
    }   

    if ( m_shaderStorage != nullptr )
    {
        m_shaderStorage->Destroy();
        delete m_shaderStorage;
        m_shaderStorage = nullptr;
    }

    if ( m_vertex != nullptr )
    {
        m_vertex->Destroy();
        delete m_vertex;
        m_vertex = nullptr;
    }

    if ( m_indexes != nullptr )
    {
        m_indexes->Destroy();
        delete m_indexes;
        m_indexes = nullptr;
    }

    if ( m_vao != nullptr )
    {
        m_vao->Destroy();
        delete m_vao;
        m_vao = nullptr;
    }

}

void crViewPort::Draw(void)
{
    // bind indirect draw buffer
    m_ctx->BindIndirectBuffer( *m_drawCmd );

    m_ctx->BindVertexArray( *m_vao );

}

void crViewPort::DrawBackground(void)
{
}

void crViewPort::DrawCheckboard(void)
{
}

void crViewPort::DrawMeshes(void)
{
}

void crViewPort::InitVao(void)
{
    gl::vertexAttrib_t vertexAttribs[3] = 
    {
        { 0, 0, 3, GL_FLOAT, GL_FALSE, offsetof( viewVert_t, position ) }, // vertex position
        { 1, 0, 3, GL_FLOAT, GL_FALSE, offsetof( viewVert_t, normal ) }, // vertex normal
        { 2, 0, 2, GL_FLOAT, GL_FALSE, offsetof( viewVert_t, tcoord ) }, // texture uv
    };

    m_vao = new gl::VertexArray();
    m_vao->Create( vertexAttribs, 3 );
}

void crViewPort::InitBuffers(void)
{

    ///
    /// Face indices buffer 
    m_indexes = new gl::Buffer();
    m_indexes->Create( GL_ELEMENT_ARRAY_BUFFER, k_MAX_ELEMENT_BUFFER_SIZE, nullptr, k_BUFFER_ACESS_FLAGS );
    m_triangles = static_cast<uint16_t*>( m_indexes->Map( 0, k_MAX_ELEMENT_BUFFER_SIZE, k_BUFFER_MAP_FLAGS ) );
    
    /// bind element buffer
    m_vao->BindElementBuffer( *m_indexes );

    /// 
    /// Vertex array buffer
    m_vertex = new gl::Buffer();
    m_vertex->Create( GL_ARRAY_BUFFER, k_MAX_VERTEX_BUFFER_SIZE, nullptr, k_BUFFER_ACESS_FLAGS );
    m_vertexes = static_cast<viewVert_t*>( m_vertex->Map( 0, k_MAX_VERTEX_BUFFER_SIZE, k_BUFFER_MAP_FLAGS ) );

    /// bind vertex buffer 
    GLuint      buffers[1]{ *m_vertex };
    GLintptr    offsets[1]{ 0 };
    GLsizei     strides[1]{ sizeof( viewVert_t ) };
    m_vao->BindeVertexBuffers( buffers, offsets, strides, 0, 1 );

    ///
    /// Shader uniform storage, buffer
    m_shaderStorage = new gl::Buffer();
    m_shaderStorage->Create( GL_SHADER_STORAGE_BUFFER, k_MAX_SHADER_BUFFER_SIZE, nullptr, k_BUFFER_ACESS_FLAGS );
    m_uniforms = static_cast<viewUniforms_t*>( m_shaderStorage->Map( 0, k_MAX_SHADER_BUFFER_SIZE, k_BUFFER_MAP_FLAGS ) );

    ///
    /// Indirect draw commands
    m_drawCmd = new gl::Buffer();
    m_drawCmd->Create( GL_DRAW_INDIRECT_BUFFER, k_MAX_DRAW_BUFFER_SIZE, nullptr, k_BUFFER_ACESS_FLAGS );
    m_drawElements = static_cast<drawElements_t*>( m_drawCmd->Map( 0, k_MAX_DRAW_BUFFER_SIZE, k_BUFFER_MAP_FLAGS ) );

    ///
    /// Draw texture indirect
    m_textures = new gl::Buffer();
    m_textures->Create( GL_SHADER_STORAGE_BUFFER, k_MAX_SAMPLER_BUFFER, nullptr, k_BUFFER_ACESS_FLAGS );
    m_samples = static_cast<uint64_t*>( m_textures->Map( 0, k_MAX_SAMPLER_BUFFER, k_BUFFER_MAP_FLAGS ) );
}

void crViewPort::InitShader(void)
{
}

/*
==========================
crMainDialog::crMainDialog
==========================
*/
crMainDialog::crMainDialog(gl::Context *in_pContext) : Gwen::Controls::Base(nullptr, "mainDialog")
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

    ///
    /// File acess menu
    Gwen::Controls::MenuItem* fileMenu = m_menu->AddItem( L"File" );
    fileMenu->GetMenu()->AddItem( L"Load", "", "Ctrl+L" )->SetAction( this, &crMainDialog::MenuLoad );
    fileMenu->GetMenu()->AddItem( L"Save", "", "CTRL+S" )->SetAction( this, &crMainDialog::MenuSave );
    fileMenu->GetMenu()->AddItem( L"Import", "", "Ctrl+Shift+S" )->SetAction( this, &crMainDialog::MenuImport );
    fileMenu->GetMenu()->AddItem( L"Export", "", "Ctrl+Shift+S" )->SetAction( this, &crMainDialog::MenuExport );
    fileMenu->GetMenu()->AddItem( L"Quit", "", "Ctrl+Q" )->SetAction( this, &crMainDialog::MenuQuit );

    ///
    /// View menu access
    Gwen::Controls::MenuItem* viewMenu = m_menu->AddItem( L"View" );

    // show ground
    m_groundGrid = viewMenu->GetMenu()->AddItem( L"Ground Grid" );
    m_groundGrid->SetCheckable( true );

    // show joints heads
    m_jointsHeads = viewMenu->GetMenu()->AddItem( L"Show Joint Head" );

    // show joints links
    m_jointsLinks = viewMenu->GetMenu()->AddItem( L"Show Joint Link" );

    // the mesh rendering mode
    Gwen::Controls::MenuItem* modeMenu = viewMenu->GetMenu()->AddItem( L"Mode" );

    /// render textured mode
    m_modeLine = modeMenu->GetMenu()->AddItem( L"Line" );
    m_modeLine->SetCheckable( true );

    /// render shaded mode
    m_modeSurface = modeMenu->GetMenu()->AddItem( L"Surface" );
    m_modeSurface->SetCheckable( true );
    
    /// render textured mode ( must load model texture )
    m_modeTextured = modeMenu->GetMenu()->AddItem( L"Texture" );
    m_modeTextured->SetCheckable( true );

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
crMainDialog::MenuQuit
==========================
*/
void crMainDialog::MenuQuit(Gwen::Controls::Base *pControl)
{
}

void crMainDialog::MenuLine(Gwen::Controls::Base *pControl)
{
}

void crMainDialog::MenuSurface(Gwen::Controls::Base *pControl)
{
}

void crMainDialog::MenuTextured(Gwen::Controls::Base *pControl)
{
}
