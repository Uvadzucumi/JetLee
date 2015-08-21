#include "sprites.h"

using namespace MyOGL;

void CHudSprite::render(CMaterial *material){
    if(!m_texture->GetID()){
        Log->puts("CHudSprite::Render() Warning: TextureID=0\n");
    }
    MyOGL::Render->Set2D();
    MyOGL::Render->SetBlendMode(this->BlendMode);
    MyOGL::GL.Enable(GL_TEXTURE_2D);
    MyOGL::Render->BindTexture(m_texture->GetID());
    // material (only color material)
    if(material!=NULL){
        material->Apply();
    }else if(m_material!=NULL){
        m_material->Apply();
    }else{ // show only texture - disable material
        GL.Disable(GL_COLOR_MATERIAL);
        MyOGL::Render->SetColor(1.0f, 1.0f, 1.0f);
    }
    if(m_angle){
        float center_x=m_left+(m_right-m_left)/2;
        float center_y=m_top+(m_bottom-m_top)/2;
        glPushMatrix();
        glTranslatef(center_x, center_y, 0);
        glRotatef(m_angle,0,0,1);
        glTranslatef(-center_x, -center_y, 0);
    }
    glBegin( GL_QUADS );
    if(!m_mirroring){
	// Top-left vertex (corner)
        glTexCoord2f( TextCoords[0].u, TextCoords[0].v );
        glVertex3f( m_left, m_top, 0);
	// Top-right vertex (corner)
        glTexCoord2f( TextCoords[1].u, TextCoords[0].v );
        glVertex3f( m_right, m_top, 0 );
	// Bottom-right vertex (corner)
        glTexCoord2f( TextCoords[1].u, TextCoords[1].v );
        glVertex3f( m_right, m_bottom, 0 );
	// Bottom-left vertex (corner)
        glTexCoord2f( TextCoords[0].u, TextCoords[1].v );
        glVertex3f( m_left, m_bottom, 0);
    }else{
	// Top-left vertex (corner)
        glTexCoord2f( TextCoords[1].u, TextCoords[0].v );
        glVertex3f( m_left, m_top, 0);
	// Top-right vertex (corner)
        glTexCoord2f( TextCoords[0].u, TextCoords[0].v );
        glVertex3f( m_right, m_top, 0 );
	// Bottom-right vertex (corner)
        glTexCoord2f( TextCoords[0].u, TextCoords[1].v );
        glVertex3f( m_right, m_bottom, 0 );
	// Bottom-left vertex (corner)
        glTexCoord2f( TextCoords[1].u, TextCoords[1].v );
        glVertex3f( m_left, m_bottom, 0);
    }
    glEnd();

    if(m_angle){
        glPopMatrix();
    }
}

void CHudSprite::setPosition(int left, int top){
    m_right+=left-m_left;
    m_bottom+=top-m_top;
    m_left=left;
    m_top=top;
}

void CHudSprite::setSize(int width, int height){
    m_right=m_left+width;
    m_bottom=m_top+height;
}

