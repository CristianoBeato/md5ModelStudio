
#ifndef __GWEN_DRAW_PANEL_H__
#define __GWEN_DRAW_PANEL_H__

namespace Gwen
{
	namespace Controls
	{
        class GWEN_EXPORT DrawPanel : public Controls::Base
        {
        public:     
            GWEN_CONTROL( DrawPanel, Controls::Base );
            ~DrawPanel( void );

            virtual void Initialize( void );
            virtual void DoThink( void );
            virtual void Render( Skin::Base* skin );
			virtual void OnBoundsChanged( Gwen::Rect oldBounds );
			// Background
			virtual void SetBackgroundColor( const Gwen::Color & color );

        private:
            Gwen::Texture       m_texture;
            Gwen::FrameBuffer   m_framebuffer;

        };
    };
};

#endif //__GWEN_DRAW_PANEL_H__