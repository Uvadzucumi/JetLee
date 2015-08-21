#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>

#include "render.h"


namespace MyOGL{

    enum eTextureFileFormat{
        tffUnknown=0,
        tffBMP,
        tffPNG
    };

    class CTexture{
            GLuint TextureID;

            std::vector<unsigned char> m_image_data;
            // texture parameters
            GLuint minFilter;
            GLuint magFilter;
            unsigned long int m_width, m_height; // Texture width & height
            GLuint m_bytes_pp;   // Texture bytes per pixel
            GLuint m_texture_format; // GL_RGBA, etc...
            bool m_alpha;
            char m_file_name[100];
            void loadFile(std::vector<unsigned char>& buffer, const std::string& file_name); //designed for loading files from hard disk in an std::vector
            int LoadPNGImage(const char *file_name);
            int LoadBitmapImage(const char *file_name);
        public:
            CTexture(){
                TextureID = 0;
                minFilter=GL_LINEAR; magFilter=GL_LINEAR;
                m_alpha=false;
            }
            ~CTexture();
            void Free();    // clear video memory and memory
            void Bind();
            bool LoadFromFile(const char *file_name);
            bool CreateFromMemory(void);
            bool CreateEmpty(int width, int height);
            int GetWidth(){ if (m_image_data.size()){return m_width;}else{return 0;}};
            int GetHeight(){ if(m_image_data.size()){return m_height;}else{ return 0;}};
            bool IsAlpha(){ return m_alpha; }
            GLuint GetID(){ return TextureID; }
            char *GetFileName(){ return m_file_name;}
            eTextureFileFormat TextureFileFormat(std::string file_name);
            unsigned char* imageData(){ return &m_image_data[0];}
            void setMinFilter(GLuint minFilter){
                this->minFilter=minFilter;
            }
            void setMagFilter(GLuint magFilter){
                this->magFilter=magFilter;
            }
    };

    extern std::vector<CTexture*>  TexturesList;

}

#endif // TEXTURE_H_INCLUDED
