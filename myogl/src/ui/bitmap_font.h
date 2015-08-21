#ifndef BITMAP_FONT_H_INCLUDED
#define BITMAP_FONT_H_INCLUDED

#include "../vector_types.h"
#include "../texture.h"
#include <vector>

namespace MyOGL{

    struct Glyph{
        unsigned int code;
        Vector2f text_coords[2];
        Vector2i vertex_Coords[4];
        int width, height, orig_width;
        int x_offset, y_offset;
    };

    class CFont{
            CTexture *m_texture;
            std::vector <Glyph> m_chars;
            int m_font_height;
            Vector3i ColorFromCode(const char *code);   // return color created from 6 char symbols
            Vector3i m_last_color;
            Vector2i m_last_sizes;
            bool m_color_changed;
        public:
            CFont(){m_font_height=-1;m_last_sizes.width=0; m_last_sizes.height=0;}
            ~CFont();
            int RenderGlyph(int x, int y, Glyph *glyph);
            bool LoadGlyphData(const char *file_name, CTexture *fexture);
            void debug(void);
            int PrintAt(int x, int y, const char *string, Vector4f color);
            int PrintAt(int x, int y, const char *string);
            int Print(const char *string, int max_width=0);
            int GetGlyphId(unsigned int code);
            int RenderText(const char *string, int max_width=0);
            CTexture *GetTexture(){ return m_texture;};
            bool IsColorChanged(){ return m_color_changed;};  // true if color changed in rendering time (with special color codes in text)
            Vector3i GetLastColor(){ return m_last_color;};   // return last rendered color
            Vector2i GetLastSizes(){ return m_last_sizes;};
            int GetHeight(){ return m_font_height;};
    };

    class CText{
            GLuint m_list_id;
            CFont *m_font;
            char *m_text;
            int m_width;
            // chave color changes for set in GL state cashe
            bool color_changed;
            Vector3i last_color;
            Vector2i m_text_sizes;
            int m_max_width;
        public:
            CText(CFont *font, const char *text=NULL, int max_width=0);
            ~CText();
            void SetText(const char *text, int max_width=0);
            Vector2i GetTextSizes(){
                return m_text_sizes;
            }
            int PrintAt(int x, int y, Vector4f color);
            int PrintAt(int x, int y);
            void Free(void);
            bool CreateDisplayList();   // create display list for text rendering
    };

    extern std::vector <CText*> TextsList;

}

#endif // BITMAP_FONT_H_INCLUDED
