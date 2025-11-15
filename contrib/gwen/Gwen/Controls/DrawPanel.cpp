
#include "Gwen/Precompiled.h"
#include "DrawPanel.h"

GWEN_CONTROL_CONSTRUCTOR( DrawPanel )
{
    SetBounds( 0, 0, 10000, 10000 );
    SetBackgroundColor( Color( 255, 255, 255, 255 ) );
}

Gwen::Controls::DrawPanel::~DrawPanel(void)
{
}

void Gwen::Controls::DrawPanel::Initialize(void)
{
}

void Gwen::Controls::DrawPanel::DoThink(void)
{
}

void Gwen::Controls::DrawPanel::Render(Skin::Base *skin)
{
    auto bounds = GetRenderBounds();
    skin->GetRender()->SetDrawColor( { 255, 75, 0, 255 } );
    skin->GetRender()->DrawFilledRect( bounds );
}

void Gwen::Controls::DrawPanel::OnBoundsChanged( Gwen::Rect oldBounds )
{
    auto bounds = GetRenderBounds();
    auto renderer = GetSkin()->GetRender();

#if 0
    // resize frame buffer 
    renderer->FreeFrameBuffer( &m_framebuffer );
    m_framebuffer.width = bounds.w;
    m_framebuffer.height = bounds.h;
    renderer->CreateFrameBuffer( &m_framebuffer );
#endif
}

void Gwen::Controls::DrawPanel::SetBackgroundColor(const Gwen::Color &color)
{
}
