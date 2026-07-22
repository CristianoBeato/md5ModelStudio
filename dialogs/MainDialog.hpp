/*
===============================================================
===============================================================
*/

#ifndef __MAIN_DIALOG_HPP__
#define __MAIN_DIALOG_HPP__

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_mutex.h>

#include "Gwen/Input/SDL3.h"
#include "Gwen/FrameBuffer.h"
#include "Gwen/Texture.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls.h"

#include "ViewStructures.hpp"
#include "Utils.hpp"

class crViewPort : public Gwen::Controls::DrawPanel
{
public:
    enum shadeMode_t
    {
        SM_LINE,        // wireframe
        SM_SHADED,      // normal 
        SM_TEXTURED     // textured
    };

    GWEN_CLASS( crViewPort, Gwen::Controls::DrawPanel )
    GWEN_DYNAMIC( crViewPort, Gwen::Controls::DrawPanel )
    virtual const char* GetTypeName( void ){ return "crViewPort"; }
	virtual const char* GetBaseTypeName( void ){ return BaseClass::GetTypeName(); }
	crViewPort( gl::Context* in_ctx, Gwen::Controls::Base* pParent, const Gwen::String& pName = "" );
    ~crViewPort( void );

    virtual bool InputMouseMoved( int x, int y, int deltaX, int deltaY ) { return false; }
	virtual bool InputMouseButton( int iButton, bool bDown ) { return false; }
	virtual bool InputKey( int iKey, bool bDown ) { return false; }
	virtual bool InputCharacter( Gwen::UnicodeChar chr ) { return false; }
	virtual bool InputMouseWheel( int val ) { return false; }

    virtual void Draw( void ) override;

    void    DrawBackground( void );
    void    DrawCheckboard( void );
    void    DrawMeshes( void );
    void    DrawJointLiks( void );
    void    DrawJointsHeads( void );

private:
    bool                m_drawJointHead;
    bool                m_drawJointLink;
    shadeMode_t         m_mode;             // meshes draw mode
    gl::Buffer*         m_indexes;          // surface indexes
    gl::Buffer*         m_vertex;           // vertices
    gl::Buffer*         m_shaderStorage;    //
    gl::Buffer*         m_drawCmd;          // draw command
    gl::Buffer*         m_textures;    //  
    gl::Program*        m_program;
    gl::VertexArray*    m_vao;
    gl::Context*        m_ctx;

    uint16_t*           m_triangles;
    viewVert_t*         m_vertexes;
    viewUniforms_t*     m_uniforms;
    drawElements_t*     m_drawElements;
    uint64_t*           m_samples;


    void    InitVao( void );
    void    InitBuffers( void );
    void    InitShader( void );
};

class crMainDialog : public Gwen::Controls::Base
{
public:
    static crMainDialog* Get( void );

    crMainDialog( void );
    ~crMainDialog( void );

    /// @brief 
    void    Create( void );

    /// @brief Release 
    void    Destroy( void );

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
    Gwen::Controls::MenuItem*       m_groundGrid;
    Gwen::Controls::MenuItem*       m_jointsHeads;
    Gwen::Controls::MenuItem*       m_jointsLinks;
    Gwen::Controls::MenuItem*       m_modeLine;
    Gwen::Controls::MenuItem*       m_modeSurface;
    Gwen::Controls::MenuItem*       m_modeTextured;
    Gwen::Controls::DrawPanel*      m_viewport;

    // Events function
    void MenuLoad( Gwen::Controls::Base* pControl );
    void MenuSave( Gwen::Controls::Base* pControl );
    void MenuExport( Gwen::Controls::Base* pControl );
    void MenuImport( Gwen::Controls::Base* pControl );
    void MenuQuit( Gwen::Controls::Base* pControl );

    void MenuLine( Gwen::Controls::Base* pControl );
    void MenuSurface( Gwen::Controls::Base* pControl );
    void MenuTextured( Gwen::Controls::Base* pControl );
};

#endif //!__MAIN_DIALOG_HPP__