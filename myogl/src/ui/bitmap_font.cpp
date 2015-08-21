
#include "bitmap_font.h"
#include <wchar.h>

using namespace MyOGL;

std::vector <CText*> MyOGL::TextsList;

// Clear font class data
CFont::~CFont(){
    // clear glyphs
    m_chars.clear();
    Log->puts("Clear Font glyphs data\n");
}

// loading font glyphs data from file file_name and assign font texture *texture
bool CFont::LoadGlyphData(const char *file_name, CTexture *texture){
    char font_name[100], font_size[20];
    char tmp[100];
    FILE *fp;
    int i;
    bool font_start=false;
    Glyph glyph;

    if(!texture){
        Log->puts("CFont::LoadGlyphData() - Error: Not set font texture\n");
        return false;
    }
    m_texture=texture;

    if(!(fp=fopen(file_name,"rt"))){
        Log->printf("CFont::LoadGlyphData() - Error: Not opened font description file '%s'\n",file_name);
        return false;
    };
    int char_count=0;
    int CharCode, Xpos, Ypos, Width, Height, Xoffset, Yoffset, OrigW, OrigH;
    while(!feof(fp)){
        // read string
        //fscanf(fp,"%s\n",tmp);
        if(fgets(tmp,100,fp)!=NULL){;
            if(!font_start){
                sscanf(tmp,"%s %s\n",font_name,font_size);
                font_start=true;
            }else{ // font founded - read glyph
                i=sscanf(tmp,"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",&CharCode, &Xpos, &Ypos, &Width, &Height, &Xoffset, &Yoffset, &OrigW, &OrigH);
                if(i!=9){   // wrong gliph string
                    Log->printf("CFont::LoadGlyphData() - Warning: Wrong glyph string '%s'",tmp);
                    break;
                }
                glyph.code=CharCode;
                glyph.width=Width;
                glyph.height=Height;
                glyph.x_offset=Xoffset;
                glyph.y_offset=Yoffset;
                glyph.orig_width=OrigW;
                if(m_font_height==-1){ m_font_height=OrigH;}
            // left top corner
                glyph.text_coords[0].u=(float)Xpos/texture->GetWidth();
                glyph.text_coords[0].v=(float)Ypos/texture->GetHeight();
            // top right
                glyph.text_coords[1].u=(float)(Xpos+Width)/(float)texture->GetWidth();
                glyph.text_coords[1].v=(float)(Ypos+Height)/(float)texture->GetWidth();

                m_chars.push_back(glyph);

                char_count++;
            } // READ GLYPH
        } // IF READED
    } // while EOF
    Log->printf("CFont::LoadGlyphData() - loaded %d glyphs\n",char_count);
    fclose(fp);
    return true;
}

// show font glyphs data
void CFont::debug(void){
// show all glyph
    unsigned int i;
    Log->printf("chars: %d\n",m_chars.size());
    for(i=0;i<m_chars.size();i++){
        Log->printf("code: %d x_off: %d ",m_chars[i].code, m_chars[i].x_offset);
        Log->printf("y_offs: %d width: %d ",m_chars[i].y_offset, m_chars[i].width);
        Log->printf("height: %d orig_width: %d ", m_chars[i].height, m_chars[i].orig_width);
        Log->printf("(%c%c)",(m_chars[i].code>>8 & 0xff),m_chars[i].code & 0xff);
    }
}

// get utf8 char code and return glyph ID from font glyphs list
int CFont::GetGlyphId(unsigned int code){
    unsigned int index;
    for(index=0;index<m_chars.size();index++){
        if(m_chars[index].code==code){
            break;
        }
    }
    if(index==m_chars.size()){
        Log->printf("CFont::GetGlyphId - Warning: symbol %d not found!\n",code);
    }
    return index;
}

// render glyph *glyph at coordinates x,y
int CFont::RenderGlyph(int x, int y, Glyph *glyph){

    float left_top=x+glyph->x_offset;
    float right_bottom=y+glyph->y_offset;

    glTexCoord2f(glyph->text_coords[0].u,glyph->text_coords[0].v);
    glVertex2d(left_top,right_bottom);

    glTexCoord2f(glyph->text_coords[1].u,glyph->text_coords[0].v);
    glVertex2d(left_top+glyph->width,right_bottom);

    glTexCoord2f(glyph->text_coords[1].u,glyph->text_coords[1].v);
    glVertex2d(left_top+glyph->width,right_bottom+glyph->height);

    glTexCoord2f(glyph->text_coords[0].u,glyph->text_coords[1].v);
    glVertex2d(left_top,right_bottom+glyph->height);

    return glyph->orig_width;
}

Vector3i CFont::ColorFromCode(const char *code){
    Vector3i color;
    //char out[50];
    //sprintf(out,"code: %c%c%c%c%c%c\n",code[0],code[1],code[2],code[3],code[4],code[5]);
    //Log->puts(out);
    unsigned char h_c, l_c;
    for(int i=0;i<6;i+=2){
        if(code[i]>47 && code [i]<58){
            h_c=code[i]-48;
        }else if(code[i]>96 && code [i]<103){
            h_c=code[i]-87; //(-97+10);
        }else{ // wrong color code
            Log->printf("Wrong color code[%d]: %c",i*2,code[i*2]);
            color.r=255;
            color.g=255;
            color.b=255;
            break;
        }
        h_c <<= 4;
        if(code[i+1]>47 && code [i+1]<58){
            l_c=code[i+1]-48;
        }else if(code[i+1]>96 && code [i+1]<103){
            l_c=code[i+1]-87; //(-97+10);
        }else{ // wrong color code
            Log->printf("Wrong color code[%d]: %c",i*2+1,code[i*2+1]);
            color.r=255;
            color.g=255;
            color.b=255;
            break;
        }
        color.data[i/2]=h_c+l_c;
    }
    //sprintf(out,"get color [%d,%d,%d]\n",color.r, color.g, color.b);
    //Log->puts(out);
    return color;
}

// render text string
int CFont::RenderText(const char *string, int max_width){
    m_color_changed=false;    // true - if changed color from stecial codes in text
    unsigned int code=0;
    unsigned int glyph_id;
    int dx=0, dy=0;//, max_dx=0;
    int str_len=strlen(string);
    m_last_sizes.width=0; m_last_sizes.height=0;
    //Vector3i last_color;
    glBegin(GL_QUADS);
    for(int i = 0; i < str_len; i++){
        if(string[i] == 10){  // new text line
            dy += this->m_font_height;
            m_last_sizes.height+=this->m_font_height;
            if(dx > m_last_sizes.width) m_last_sizes.width = dx;
            dx = 0;
            continue;
        }
        if(string[i]=='^'){ // detect special char code
            if(i<str_len-1){ // not last
                if(string[i+1]=='^'){
                    i++;
                }else{  // check for color
                    if(i<(str_len-6)){
                        m_last_color=ColorFromCode(&string[i+1]);
                        MyOGL::Render->SetColor(m_last_color,true); // force change color (special for use in display list)
                        m_color_changed=true;
                        i+=7;
                    }else{
                        Log->puts("Warning in CFont::RenderText. Not have text size for color code! Ignore '^'\n");
                    }
                }
            }
        }
        code=string[i];
        if(code > 127){ // 2 byte symbol
            i++;
            code <<= 8;
            code += (string[i] & 0xFF);
            code &= 0xFFFF;
        }
        // get glyph id
        glyph_id=GetGlyphId(code);
        // check max width - need or not move to next line
        if(max_width){
            if((dx+m_chars[glyph_id].orig_width)>max_width){ // new line
                dy += this->m_font_height;
                m_last_sizes.height+=this->m_font_height;
                if(dx > m_last_sizes.width) m_last_sizes.width = dx;
                dx = 0;
            }
        }
        dx += RenderGlyph(dx, dy, &m_chars[glyph_id]);
    }
    glEnd();
    if(dx > m_last_sizes.width) m_last_sizes.width = dx;
    m_last_sizes.height+=this->m_font_height;
    return m_last_sizes.width;
}

// render text *string at coordinates x,y. using color color
int CFont::PrintAt(int x, int y, const char *string, Vector4f color){
    int width;
    //unsigned int time_start;
    //time_start=SDL_GetTicks();
    Render->Set2D();
    GL.Disable(GL_LIGHTING);
    Render->SetColor(color.r, color.g, color.b, color.a);
    Render->SetBlendMode(blSource);
    Render->BindTexture(m_texture->GetID());
    // move to coords
    glTranslatef(x,y,0);
    width=RenderText(string);
    // return
    glTranslatef(-x,-y,0);
    //printf("BeginEnd time: %d\n",SDL_GetTicks()-time_start);
    return width;
}

// render text *string at coordinates x,y. using color color
int CFont::PrintAt(int x, int y, const char *string){
    int width;
    // move to coords
    glTranslatef(x,y,0);
    width=Print(string);
    // return coords
    glTranslatef(-x,-y,0);
    return width;
}

// render text *string at coordinates x,y. using color color
int CFont::Print(const char *string, int max_width){
    int width;
    Render->Set2D();
    GL.Disable(GL_LIGHTING);
    Render->SetBlendMode(blSource);
    Render->BindTexture(m_texture->GetID());
    // move to coords
    width=RenderText(string, max_width);
    // return
    return width;
}

// create text onject
CText::CText(CFont *font, const char *text, int max_width){
    m_list_id=0;
    m_font=font;
    m_text=NULL;
    m_max_width=max_width;
    if(text){
        SetText(text, max_width);
    }
    TextsList.push_back(this);  // add to list
}

void CText::Free(void){
   // Log->puts("Delete display list %d",m_list_id);
   // Log->puts(" and text string (%d bytes)\n",sizeof(m_text));
    if(m_list_id){
        Log->printf("delete Display list %d\n",m_list_id);
        glDeleteLists(m_list_id,  1);
        m_list_id=0;
    }
    if(m_text!=NULL){
        Log->printf("Free text string memory. %d bytes\n",strlen(m_text));
        free(m_text);
        m_text=NULL;
    }
}

void CText::SetText(const char *text, int max_width){
    Free(); // Free previos str and display list (if needed)
    color_changed=false;
    m_text=(char *)malloc(strlen(text)+1);
    strcpy(m_text,text);
    // generate display list
    m_max_width=max_width;
    CreateDisplayList();
}

// create display list for text rendering
bool CText::CreateDisplayList(){
    m_list_id=glGenLists(1);
    Log->printf("Generate display list %d\n",m_list_id);
    glNewList(m_list_id,GL_COMPILE);
    m_width=m_font->RenderText(m_text, m_max_width);
    // render text
    glEndList();
    if(m_font->IsColorChanged()){
        color_changed=true;
        last_color=m_font->GetLastColor();
    }
    m_text_sizes=m_font->GetLastSizes();
    printf("text size w:%d h:%d\n",m_text_sizes.width, m_text_sizes.height);
    return true;
}

int CText::PrintAt(int x, int y, Vector4f color){
    Render->SetColor(color.r, color.g, color.b, color.a);
    return PrintAt(x,y);
}

int CText::PrintAt(int x, int y){
    Render->Set2D();
    glTranslatef(x,y,0);
    GL.Disable(GL_LIGHTING);
    Render->SetBlendMode(blSource);
    m_font->GetTexture()->Bind();
    // move to coords
    glCallList(m_list_id);
    //m_font->RenderText(m_text);
    // return
    glTranslatef(-x,-y,0);
    if(color_changed){
        MyOGL::Render->SetColor(last_color,true);   // force change color - for save to GL cashe
    }
    //printf("DL time: %d\n",SDL_GetTicks()-time_start);
    return m_width;
}

CText::~CText(){
    Free();
    for(unsigned int i=0;i<TextsList.size();i++){
        if(TextsList[i]==this){
            TextsList.erase(TextsList.begin()+i);
        }
    }
    Log->puts("deleted CText object\n");
}
