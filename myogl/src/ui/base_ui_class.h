#ifndef BASE_UI_CLASS_H_INCLUDED
#define BASE_UI_CLASS_H_INCLUDED

#include "../vector_types.h"

namespace MyOGL{

    class CUIBase{
        protected:
            Vector2i m_left_top;
            Vector2i m_right_bottom;
        public:
            CUIBase(int x, int y, int w, int h){
                m_left_top.x=x;
                m_left_top.y=y;
                m_right_bottom.x=x+w;
                m_right_bottom.y=y+h;
            };
            void SetPosition(int x, int y){
                m_left_top.x=x;
                m_left_top.y=y;
            };
            void SetDimension(int w, int h){
                m_right_bottom.x=m_left_top.x+w;
                m_right_bottom.y=m_left_top.y+h;
            };
    };


}


#endif // BASE_UI_CLASS_H_INCLUDED
