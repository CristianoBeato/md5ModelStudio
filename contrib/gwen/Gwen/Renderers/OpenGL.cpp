
#include "Gwen/Renderers/OpenGL.h"
#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/WindowProvider.h"
#include "FontData.h"

#include <math.h>
#include <SDL3_image/SDL_image.h>
#include "OpenGL.h"

constexpr uint32_t k_MAX_ELEMENTS_COUNT = 8 * 1024;
constexpr uint32_t k_MAX_VERTICES_COUNT = k_MAX_ELEMENTS_COUNT * 4;
constexpr size_t   k_VERTEX_SIZE = sizeof( float ) * 4;

static int ReadShaderSource( const char* path, char** source )
{
    int len = 0;
    FILE* srcf = nullptr;

    srcf = fopen( path, "r" );
    if ( !srcf )
        return -1;

    fseek( srcf, 0, SEEK_END );
    len = ftell( srcf );
    fseek( srcf, 0, SEEK_SET );

    *source = static_cast<char*>( std::malloc( len ) ); 

    fread( *source, 1, len, srcf );

    // close source file 
    fclose( srcf );

    return len;
}

Gwen::Renderer::SDL3Context::SDL3Context( void ) : m_renderContext( nullptr ), m_renderWindown( nullptr )
{
}

Gwen::Renderer::SDL3Context::~SDL3Context( void )
{
}

bool Gwen::Renderer::SDL3Context::Create(const void *in_windowHandle)
{
    m_renderWindown = (SDL_Window*) in_windowHandle;

    int channelcolorbits = 8;
    int tdepthbits = 24;
    int tstencilbits = 8;
    
    // setup window buffer color 
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, channelcolorbits );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, channelcolorbits );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, channelcolorbits );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, channelcolorbits );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, tdepthbits );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, tstencilbits );

    // 4x MSAA
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
	
    // 4.6 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
			
    // debug output
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );
	
    m_renderContext = SDL_GL_CreateContext( m_renderWindown );
    if ( !m_renderContext )
        return false;
    
    Init();

    return true;
}

void Gwen::Renderer::SDL3Context::Destroy(void)
{
    Finalize();

    if ( m_renderContext != nullptr )
    {
        SDL_GL_DestroyContext( m_renderContext );
        m_renderContext = nullptr;
    }
}

bool Gwen::Renderer::SDL3Context::MakeCurrent(void)
{
    return SDL_GL_MakeCurrent( m_renderWindown, m_renderContext );
}

bool Gwen::Renderer::SDL3Context::Release(void)
{
    return SDL_GL_MakeCurrent( m_renderWindown, nullptr );
}

bool Gwen::Renderer::SDL3Context::SwapBuffers(void)
{
    return SDL_GL_SwapWindow( m_renderWindown );
}

void *Gwen::Renderer::SDL3Context::GetFunctionPointer(const char *in_name) const
{
    return (void*)SDL_GL_GetProcAddress( in_name );
}

void Gwen::Renderer::SDL3Context::DebugOuput(const char *in_message) const
{
    std::cout << in_message;
}

Gwen::Renderer::OpenGL::OpenGL( void ) :
	m_mode( RECT_LINE ),
	m_vhead( 0 ),
	m_vtail( 0 ),
	m_ihead( 0 ),
	m_itail( 0 ),
	m_elements( nullptr ),
	m_vertexes( nullptr  ),
	m_pFontTexture( nullptr ),
	m_pContext( nullptr )
{
	m_fLetterSpacing = 1.0f / 16.0f;
	m_fFontScale[0] = 1.5f;
	m_fFontScale[1] = 1.5f;
}

Gwen::Renderer::OpenGL::~OpenGL( void )
{
	DestroyDebugFont();
}

void Gwen::Renderer::OpenGL::Init( void )
{
	CreateDebugFont();
}

void Gwen::Renderer::OpenGL::Begin( void )
{
	m_pContext->BindVertexArray( m_vertexArray );
	m_pContext->BindProgram( m_program );

	GLuint ubo = m_uniformBuffer;
	GLintptr uboOffset = 0;
	GLsizeiptr uboSizes = sizeof( float ) * 20;
	m_pContext->BindUniformBuffers( &ubo, &uboOffset, &uboSizes, 0, 1 );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_BLEND );
}

void Gwen::Renderer::OpenGL::End( void )
{
	Flush();
	m_pContext->BindProgram( 0 );
	m_pContext->BindVertexArray( 0 );
}

void Gwen::Renderer::OpenGL::SetDrawColor( Gwen::Color color )
{
	// convert color and upload to uniform buffer
	glm::vec4 rgba;
	rgba.r = static_cast<float>( color.r ) / 255.0f;
	rgba.g = static_cast<float>( color.g ) / 255.0f;
	rgba.b = static_cast<float>( color.b ) / 255.0f;
	rgba.a = static_cast<float>( color.a ) / 255.0f;
	m_uniformBuffer.Upload( glm::value_ptr( rgba ), 0, sizeof( glm::vec4 ) );
}

void Gwen::Renderer::OpenGL::StartClip( void )
{
	Flush();
	Gwen::Rect rect = ClipRegion();
	rect.y = m_heigth - ( rect.y + rect.h );

	glScissor( rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale() );
	glEnable( GL_SCISSOR_TEST );
};

void Gwen::Renderer::OpenGL::EndClip( void )
{
	Flush();
	glDisable( GL_SCISSOR_TEST );
};

void Gwen::Renderer::OpenGL::DrawPixel(int x, int y)
{
	bounds_t pos{};
	if ( m_mode != RECT_FILL )
	{
		Flush();

		GLuint white = m_white;
		GLuint sample = m_sample;
		m_pContext->BindTextures( &white, &sample, 0, 1 );
		m_mode = RECT_FILL;
	}

	Translate( x, y );

	pos.left = static_cast<float>( x );
	pos.top = static_cast<float>( y );
	pos.right = static_cast<float>( x + 1 );
	pos.bottom = static_cast<float>( y + 1 );	

	AddQuad( pos, { 0.0f, 0.0f, 0.0f, 0.0f} );
}

void Gwen::Renderer::OpenGL::DrawLinedRect(Gwen::Rect rect)
{
	bounds_t pos{};
	if ( m_mode != RECT_LINE )
	{
		Flush();

		GLuint white = m_white;
		GLuint sample = m_sample;
		m_pContext->BindTextures( &white, &sample, 0, 1 );
		m_mode = RECT_LINE;
	}

	Translate( rect );

	pos.left = static_cast<float>( rect.x );
	pos.top = static_cast<float>( rect.y );
	pos.right = static_cast<float>( rect.x + rect.w );
	pos.bottom = static_cast<float>( rect.y + rect.h );	

	AddQuad( pos, { 0.0f, 0.0f, 0.0f, 0.0f} );
}

void Gwen::Renderer::OpenGL::DrawFilledRect(Gwen::Rect rect)
{
	bounds_t pos{};
	if ( m_mode != RECT_FILL )
	{
		Flush();

		GLuint white = m_white;
		GLuint sample = m_sample;
		m_pContext->BindTextures( &white, &sample, 0, 1 );
		m_mode = RECT_FILL;
	}

	Translate( rect );

	pos.left = static_cast<float>( rect.x );
	pos.top = static_cast<float>( rect.y );
	pos.right = static_cast<float>( rect.x + rect.w );
	pos.bottom = static_cast<float>( rect.y + rect.h );	

	AddQuad( pos, { 0.0f, 0.0f, 0.0f, 0.0f} );
}

void Gwen::Renderer::OpenGL::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
{
	bounds_t pos{};
	bounds_t uv{};

	gl::Image* tex = static_cast<gl::Image*>( pTexture->data );

	if ( m_mode != RECT_TEXTURED )
	{
		Flush();
		GLuint white = *tex;
		GLuint sample = m_sample;
		m_pContext->BindTextures( &white, &sample, 0, 1 );
		m_mode = RECT_TEXTURED;
	}
	else
	{
		auto state = m_pContext->CurrentState();

		if ( state.textures.textures[0] == *tex )
		{
			Flush();

			GLuint white = *tex;
			GLuint sample = m_sample;
			m_pContext->BindTextures( &white, &sample, 0, 1 );
		}
	}

	// Missing image, not loaded properly?
	if ( !tex )
		return DrawMissingImage( rect );

	Translate( rect );

	pos.left = rect.x;
	pos.top = rect.y;
	pos.right = rect.x + rect.w;
	pos.bottom = rect.y + rect.h;	

	uv.left = u1;
	uv.right = u2;
	uv.top = v1;
	uv.bottom = v2;

	AddQuad( pos, uv );
}

void Gwen::Renderer::OpenGL::RenderText(Gwen::Font *pFont, Gwen::Point pos, const Gwen::UnicodeString &text)
{
	float fSize = pFont->size * Scale();

	if ( !text.length() )
		return;

	Gwen::String converted_string = Gwen::Utility::UnicodeToString( text );
	float yOffset = 0.0f;

	for ( int i = 0; i < text.length(); i++ )
	{
		char ch = converted_string[i];
		float curSpacing = sGwenDebugFontSpacing[ch] * m_fLetterSpacing * fSize * m_fFontScale[0];
		Gwen::Rect r( pos.x + yOffset, pos.y - fSize * 0.5, ( fSize * m_fFontScale[0] ), fSize * m_fFontScale[1] );

		if ( m_pFontTexture )
		{
			float uv_texcoords[8] = {0., 0., 1., 1.};

			if ( ch >= 0 )
			{
				float cx = ( ch % 16 ) / 16.0;
				float cy = ( ch / 16 ) / 16.0;
				uv_texcoords[0] = cx;
				uv_texcoords[1] = cy;
				uv_texcoords[4] = float( cx + 1.0f / 16.0f );
				uv_texcoords[5] = float( cy + 1.0f / 16.0f );
			}

			DrawTexturedRect( m_pFontTexture, r, uv_texcoords[0], uv_texcoords[5], uv_texcoords[4], uv_texcoords[1] );
			yOffset += curSpacing;
			}
			else
			{
				DrawFilledRect( r );
				yOffset += curSpacing;
			}
	}
}

void Gwen::Renderer::OpenGL::LoadTexture( Gwen::Texture* pTexture )
{
	GLenum internalFormat = 0;
	gl::Image::dimensions_t	dim{};
	gl::Image::offsets_t pos{};
	
	const char* fileName = pTexture->name.Get().c_str();

	SDL_Surface* image = IMG_Load( fileName );
	if ( !image )
	{
		pTexture->failed = true;
		return;
	}
	
	// SDL requires a fliped image 
	//SDL_FlipSurface( image, SDL_FLIP_VERTICAL );

	switch ( image->format )
	{
	case SDL_PIXELFORMAT_RGB24:
	{
		internalFormat = GL_RGB8;
	} break;
	case SDL_PIXELFORMAT_BGR24:
	{
		internalFormat = GL_RGB8;
	} break;
	case SDL_PIXELFORMAT_ARGB8888:
	case SDL_PIXELFORMAT_RGBA32:
	{
		internalFormat = GL_RGBA8;
	} break;
	case SDL_PIXELFORMAT_ARGB32:
		{
		internalFormat = GL_RGBA8;
	} break;
	case SDL_PIXELFORMAT_RGB48:
	{
		internalFormat = GL_RGB16;
	} break;
	case SDL_PIXELFORMAT_BGR48:
		{
		internalFormat = GL_RGB16;
	} break;
	case SDL_PIXELFORMAT_RGBA64:
	{
		internalFormat = GL_RGBA16;
	} break;
	case SDL_PIXELFORMAT_ARGB64:
		{
		internalFormat = GL_RGBA16;
	} break;	
	default:
	{	
		pTexture->failed = true;
		return;

		SDL_DestroySurface( image );
		image = nullptr;
	}break;
	}

	// Create a little texture pointer..
	gl::Image* pglTexture = new gl::Image();
	
	// Sort out our GWEN texture
	pTexture->data = pglTexture;
	pTexture->width = image->w;
	pTexture->height = image->h;

	// Create the opengl texture
	dim.depth = 1;
	dim.width = image->w;
	dim.height = image->h;

	pglTexture->Create( GL_TEXTURE_2D, internalFormat, 1, 1, dim );
	
	pos.xoffset = 0;
	pos.yoffset = 0;
	pos.zoffset = 0;
	pglTexture->SubImage( 0, pos, dim,  image->pixels, false );

	SDL_DestroySurface( image );
	image = nullptr;
	pTexture->failed = false;
}

void Gwen::Renderer::OpenGL::FreeTexture( Gwen::Texture* pTexture )
{
	gl::Image* tex = static_cast<gl::Image*>( pTexture->data );

	if ( !tex )
		return;

	tex->Destroy();
	delete tex;
	pTexture->data = nullptr;
}

Gwen::Color Gwen::Renderer::OpenGL::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default )
{
	gl::Image::dimensions_t dim{};
	gl::Image::offsets_t	off{};
	gl::Image* tex = static_cast<gl::Image*>( pTexture->data );
	if ( !tex ) 
		return col_default;

	uint8_t pixel[4] { 0, 0, 0, 0 };
	unsigned int iPixelSize = sizeof( unsigned char ) * 4;
	dim.depth = 1;
	dim.width = 1;
	dim.height = 1;
	off.xoffset = x;
	off.yoffset = y;
	off.zoffset = 0;
	tex->GetSubImage( 0, off, dim, GL_UNSIGNED_BYTE, iPixelSize, pixel );
	
	// wait for retrieve image data 
	glFinish(); 

	Gwen::Color c;
	c.r = pixel[0];
	c.g = pixel[1];
	c.b = pixel[2];
	c.a = pixel[3];

	return c;
}

Gwen::Point Gwen::Renderer::OpenGL::MeasureText(Gwen::Font *pFont, const Gwen::UnicodeString &text)
{
	Gwen::Point p;
	float fSize = pFont->size * Scale();
	Gwen::String converted_string = Gwen::Utility::UnicodeToString( text );
	float spacing = 0.0f;

	for ( int i = 0; i < text.length(); i++ )
	{
		char ch = converted_string[i];
		spacing += sGwenDebugFontSpacing[ch];
	}

	p.x = spacing * m_fLetterSpacing * fSize * m_fFontScale[0];
	p.y = pFont->size * Scale();
	return p;
}

void Gwen::Renderer::OpenGL::AddQuad(const bounds_t pos, const bounds_t uv)
{
	if ( ( ( m_vhead + 4 ) > k_MAX_VERTICES_COUNT  ) || ( m_ihead + 6 ) > k_MAX_ELEMENTS_COUNT )
	{
		Flush();
		m_ihead = 0;
		m_itail = 0;
		m_vhead = 0;
		m_vtail = 0;
	}

	// TL ____ TR
	//   |   /|
	//	 |T1/ |
	//   | /T2|
	// BL|/___|BR

	const float vertexes[16]
	{
		pos.left, pos.top, uv.left, uv.top,			// 0 TL
		pos.right, pos.top, uv.right, uv.top,		// 1 TR
		pos.left, pos.bottom, uv.left, uv.bottom,	// 2 BL
		pos.right, pos.bottom, uv.right, uv.bottom,	// 3 BR
	};

	const uint16_t indexes[6]
	{
		// T1
		( m_vhead - m_vtail ) + 0, // TL
		( m_vhead - m_vtail ) + 1, // TR
		( m_vhead - m_vtail ) + 2, // BL
		// T2
		( m_vhead - m_vtail ) + 1, // TR
		( m_vhead - m_vtail ) + 2, // BL
		( m_vhead - m_vtail ) + 3, // BR
	};

	// copy vertexes and indices to buffer
	std::memcpy( m_vertexes + ( m_vhead * 4 ), vertexes, k_VERTEX_SIZE * 4 );
	std::memcpy( m_elements + m_itail, indexes, sizeof(uint16_t) * 6 );

	// move 
	m_vhead += 4;
	m_ihead += 6;
}

void Gwen::Renderer::OpenGL::Flush( void )
{
	if ( m_itail == m_ihead )
		return;
	
	size_t offset = sizeof( GLushort ) * m_itail;
	GLenum drawMode = m_mode == RECT_LINE ? GL_LINE_LOOP : GL_TRIANGLES;
	glDrawElementsBaseVertex( drawMode, m_ihead - m_itail, GL_UNSIGNED_SHORT, reinterpret_cast<void*>( sizeof( GLushort ) * m_itail ), m_vtail );

	m_itail = m_ihead;
	m_vtail = m_vhead;
	glFlush();
}

void Gwen::Renderer::OpenGL::CreateDebugFont(void)
{
	if ( m_pFontTexture )
		return;

	m_pFontTexture = new Gwen::Texture();
	
	// Create a little texture pointer..
	gl::Image* pglTexture = new gl::Image();
	
	// Sort out our GWEN texture
	m_pFontTexture->data = pglTexture;
	m_pFontTexture->width = 256;
	m_pFontTexture->height = 256;

	// Create the opengl texture
	gl::Image::dimensions_t dim{};
	dim.width = 256;
	dim.height = 256;
	dim.depth = 0;

	pglTexture->Create( GL_TEXTURE_2D, GL_RGBA8, 1, 1, dim );
			
	unsigned char* texdata = new unsigned char[256 * 256 * 4];

	for ( int i = 0; i < 256 * 256; i++ )
	{
		texdata[i * 4] = sGwenFontData[i];
		texdata[i * 4 + 1] = sGwenFontData[i];
		texdata[i * 4 + 2] = sGwenFontData[i];
		texdata[i * 4 + 3] = sGwenFontData[i];
	}

	pglTexture->SubImage( 0, { 0, 0, 0}, { 256, 256, 0 }, texdata, false );

	delete[]texdata;
}

void Gwen::Renderer::OpenGL::DestroyDebugFont(void)
{
	if ( !m_pFontTexture )
			return;

	gl::Image* tex = static_cast<gl::Image*>( m_pFontTexture->data );
	if ( !tex )
		return;

	tex->Destroy();
	delete tex;
	m_pFontTexture->data = nullptr;
	delete m_pFontTexture;
	m_pFontTexture = nullptr;
}

bool Gwen::Renderer::OpenGL::InitializeContext( Gwen::WindowProvider* pWindow )
{
	int len = 0;
	int w = 0, h = 0;
	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	char * sources = nullptr;
	gl::Shader* shaders[2] { nullptr, nullptr };

	if ( !pWindow )
		return false;

	m_pContext = new SDL3Context();
	m_pContext->Create( pWindow->GetWindow() );

	SDL_GetWindowSizeInPixels( static_cast<SDL_Window*>( pWindow->GetWindow() ), &w, &h );

	m_width = w;
	m_heigth = h;

	///
	/// Create Vertex Array and configure vertex attributes 
	///
	gl::vertexAttrib_t	attrbs[2]
	{
		{ 0, 0, 2, GL_FLOAT, GL_FALSE, 0},
		{ 1, 0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2 }
	};

	m_vertexArray.Create( attrbs, 2 );

	///
	/// Createa a white image whit size 16 x 16
	///
	uint16_t image[16 * 16 * 4];
	gl::Image::dimensions_t dim{ 16, 16, 0 };
	gl::Image::offsets_t	off{ 0, 0, 0};

	// make a full white image
	std::memset( image, 0xFF, 16 * 16 * 4 );

	m_white.Create( GL_TEXTURE_2D, GL_RGBA8, 1, 1, dim );
	m_white.SubImage( 0, off, dim, image, false );

	m_sample.Create();
	m_sample.Parameteri( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	m_sample.Parameteri( GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	///
	/// Load Vertex Shader
	///
	shaders[0] = new gl::Shader();
    len = ReadShaderSource( "./shaders/draw_gui.vs", &sources );
    if ( len < 0 )
        throw std::runtime_error( "Error can't open file" );
    
    if( !shaders[0]->Create( GL_VERTEX_SHADER, &sources, &len, 1 ) )
        throw std::runtime_error( "Failed to create vertex shader" );
    
    if ( sources != nullptr )
    {
        std::free( sources );
        sources = nullptr;
    }
    
	///
	/// Load Fragment shader
	///
    shaders[1] = new gl::Shader();
    len = ReadShaderSource( "./shaders/draw_gui.fs", &sources );
    if ( len < 0 )
        throw std::runtime_error( "Error can't open file" );

    if( !shaders[1]->Create( GL_FRAGMENT_SHADER, &sources, &len, 1 ) )
        throw std::runtime_error( "Failed to create fragment shader" );
    
    if ( sources != nullptr )
    {
        std::free( sources );
        sources = nullptr;
    }

	///
	/// Create and Link Program 
	///
	if( !m_program.Create( const_cast<const gl::Shader**>(shaders), 2 ) )
		throw std::runtime_error("can't create program");

	///
	/// Create Vertex Buffer
	///
	m_vertexBuffer.Create( GL_ARRAY_BUFFER, k_VERTEX_SIZE * k_MAX_VERTICES_COUNT, nullptr, flags );
	m_vertexes = static_cast<GLfloat*>( m_vertexBuffer.Map( 0, k_VERTEX_SIZE * k_MAX_VERTICES_COUNT, flags ) );
	
	GLintptr offset = 0;
	GLsizei stride = k_VERTEX_SIZE;
	GLuint vbo = m_vertexBuffer;
	m_vertexArray.BindeVertexBuffers( &vbo, &offset, &stride, 0, 1 );

	///
	/// Create Index Buffer 
	///
	m_elementBuffer.Create( GL_ELEMENT_ARRAY_BUFFER, sizeof( uint16_t ) * k_MAX_ELEMENTS_COUNT, nullptr, flags );
	m_elements = static_cast<uint16_t*>( m_elementBuffer.Map( 0, sizeof( uint16_t ) * k_MAX_ELEMENTS_COUNT, flags ) );
	m_vertexArray.BindElementBuffer( m_elementBuffer );

	///
	/// Create Uniform Block Buffer
	/// 
	m_uniformBuffer.Create( GL_UNIFORM_BUFFER, sizeof( float ) * 20, nullptr, GL_DYNAMIC_STORAGE_BIT );
	
	/// set the ortographic projection matrix 
	glm::mat4 projection = glm::ortho( 0.0f, static_cast<float>( m_width ), static_cast<float>( m_heigth ), 0.0f, -1.0f, 1.0f );
	m_uniformBuffer.Upload( glm::value_ptr( projection ), sizeof( glm::vec4 ), sizeof( glm::mat4 ) );
	
	return true;
}

bool Gwen::Renderer::OpenGL::ShutdownContext( Gwen::WindowProvider* pWindow )
{
	if( m_sample )
		m_sample.Destroy();

	if( m_white )
		m_white.Destroy();

	if ( m_uniformBuffer )
		m_uniformBuffer.Destroy();	

	if( m_elementBuffer )
		m_elementBuffer.Destroy();

	if( m_vertexBuffer )
		m_vertexBuffer.Destroy();

	if ( m_program )
		m_program.Destroy();
	
	if( m_vertexArray )
		m_vertexArray.Destroy();

	if( m_pContext )
	{
		m_pContext->Destroy();
		delete m_pContext;
	}

	return true;
}

bool Gwen::Renderer::OpenGL::PresentContext( Gwen::WindowProvider* pWindow )
{
	if ( !pWindow )
		return false;

	return SDL_GL_SwapWindow( static_cast<SDL_Window*>( pWindow->GetWindow() ) );
}

bool Gwen::Renderer::OpenGL::ResizedContext( Gwen::WindowProvider* pWindow, int w, int h )
{
	if ( !pWindow )
		return false;

	SDL_GetWindowSizeInPixels( static_cast<SDL_Window*>( pWindow->GetWindow() ), &w, &h );

	m_width = w;
	m_heigth = h;

	glViewport( 0, 0, w, h );
	glm::mat4 projection = glm::ortho( 0.0f, static_cast<float>( m_width ), static_cast<float>( m_heigth ), 0.0f, -1.0f, 1.0f );
	m_uniformBuffer.Upload( glm::value_ptr( projection ), sizeof( glm::vec4 ), sizeof( glm::mat4 ) );
	return true;
}

bool Gwen::Renderer::OpenGL::BeginContext( Gwen::WindowProvider* pWindow )
{
	glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
	glViewport( 0, 0, m_width, m_heigth );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	return true;
}

bool Gwen::Renderer::OpenGL::EndContext( Gwen::WindowProvider* pWindow )
{
	glFinish();
	return true;
}