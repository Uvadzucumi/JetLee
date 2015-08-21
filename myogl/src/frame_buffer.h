#ifndef FRAME_BUFFER_H_INCLUDED
#define FRAME_BUFFER_H_INCLUDED

#include "render.h"
#include "texture.h"

namespace MyOGL{

    class CFBO{

        GLuint m_FboID, m_RboID;
        int m_width, m_height;
        CTexture *m_texture;

        public:
            CFBO(int width, int height){
                m_width=width;
                m_height=height;
                // create empty texture
                m_texture=new CTexture();
                m_texture->CreateEmpty(m_width, m_height);
                //Create Render Buffer Object for (Deph)
                glGenRenderbuffersEXT(1, &m_RboID);
                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_RboID);
                glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
                glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
                // create FBO buffer
                glGenFramebuffersEXT(1, &m_FboID);
                this->Enable();
                // assign texture to FBO
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_texture->GetID(), 0);
                glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_RboID);

                if(!this->IsOK()){
                    Log->printf("CFBO::CFBO() Init Error! Status: %d\n",this->Status());
                }else{
                    Log->printf("Create ColorAttachment FBO id=%d texture_id=%d\n",m_FboID, m_texture->GetID());
                }
                this->Disable(); // Unbind buffer
            };

            ~CFBO(){
                // delete FBO
                this->Disable();
                glDeleteFramebuffersEXT ( 1, &m_FboID );
                // delete texture
                MyOGL::Render->BindTexture(0);
                delete m_texture;
           }

            const bool IsOK(){
                return Status()==GL_FRAMEBUFFER_COMPLETE_EXT;
            }

            int Status(void){
                return glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
            }

            void Enable(){
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FboID);
            }
            void Disable(){
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
            }
            GLuint GetTextureId(){
                return m_texture->GetID();
            }
            CTexture *GetTexture(){
                return m_texture;
            }
    };


}

#endif // FRAME_BUFFER_H_INCLUDED
