#ifndef PANEL_H_INCLUDED
#define PANEL_H_INCLUDED

#include "base_ui_class.h"
#include "../render.h"

namespace MyOGL{

   class CUIPanel : public CUIBase{
            bool m_border;
            bool m_background;
            Vector4f m_border_color;
            Vector4f m_bg_color;
        public:
            CUIPanel():CUIBase(0,0,10,10){ // defaul constructor
                m_border=true;
                m_background=true;
                m_border_color.set(1.0f,1.0f,0.0f,1.0f);  // yellow
                m_bg_color.set(0.0f,0.0f,0.0f,0.8f);    // black with 80% alfa
            };
            CUIPanel(int x, int y, int w, int h):CUIBase(x,y,w,h){
                m_border=true;
                m_background=true;
                m_border_color.set(1.0f,1.0f,0.0f,1.0f);  // yellow
                m_bg_color.set(0.0f,0.0f,0.0f,0.8f);    // black with 80% alfa
            };
            void SetBorderColor(Vector4f color){
                m_border_color.set(color.r, color.g, color.b, color.a);
            };
            void SetBackgroundColor(Vector4f color){
                m_bg_color.set(color.r, color.g, color.b, color.a);
            };
            void BackgroundEnabled(bool background){
                m_background=background;
            };
            void BorderEnabled(bool border){
                m_border=border;
            };
            void Render(){
                if(!m_border && !m_background)
                    return;
                MyOGL::GL.Disable(GL_TEXTURE_2D);

                if(m_background){
                    MyOGL::Render->SetColor(m_bg_color);
                    MyOGL::Render->SetBlendMode(blSource);

                    glBegin(GL_QUADS);
                        glVertex2i(m_left_top.x, m_left_top.y);
                        glVertex2i(m_right_bottom.x, m_left_top.y);
                        glVertex2i(m_right_bottom.x, m_right_bottom.y);
                        glVertex2i(m_left_top.x, m_right_bottom.y);
                    glEnd();
                }
                if(m_border){
                    MyOGL::Render->SetBlendMode(blNone);
                    MyOGL::Render->SetColor(m_border_color);
                    glBegin(GL_LINE_LOOP);
                        glVertex2i(m_left_top.x, m_left_top.y);
                        glVertex2i(m_right_bottom.x, m_left_top.y);
                        glVertex2i(m_right_bottom.x, m_right_bottom.y);
                        glVertex2i(m_left_top.x, m_right_bottom.y);
                    glEnd();
                }
            MyOGL::GL.Enable(GL_TEXTURE_2D);
        };
    };



}

#endif // PANEL_H_INCLUDED
