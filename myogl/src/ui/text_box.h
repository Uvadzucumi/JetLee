#ifndef TEXT_BOX_H_INCLUDED
#define TEXT_BOX_H_INCLUDED

#include "../render.h"
#include "bitmap_font.h"
#include <vector>

namespace MyOGL{

typedef struct{
    Vector2i str_size;
    CText* string;
} sTextBoxString;

class CTextBox{
        std::vector <sTextBoxString> TextStrings;
        Vector4i m_viewport;
        CFont *m_font;
        unsigned int m_max_strings;
        int m_text_height;
        bool m_vAlignTop;
    public:
        CTextBox(CFont *font);
        ~CTextBox();
        void AddString(const char *text_string);
        void SetViewPort(int left, int top, int width, int height);
        void Render(Vector4f color);
};

}   // MyOGL namespace

#endif // TEXT_BOX_H_INCLUDED
