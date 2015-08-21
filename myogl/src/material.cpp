
#include "material.h"

using namespace MyOGL;

void CMaterial::Apply(void){
    switch(m_MaterialType){
        case mColorMaterial:
            GL.Enable(GL_COLOR_MATERIAL);
            Render->SetColor(m_Diffuse.r, m_Diffuse.g, m_Diffuse.b); // set diffuse color
            break;
        case mNormal:
            GL.Disable(GL_COLOR_MATERIAL); //Required for the glMaterial calls to work
            //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_Diffuse);
            glMaterialfv(GL_FRONT, GL_AMBIENT, m_Ambient.data);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Diffuse.data);
            glMaterialfv(GL_FRONT, GL_SPECULAR, m_Specular.data);
            glMaterialfv(GL_FRONT, GL_EMISSION, m_Emission .data);
            //The shininess parameter.  A higher number indicates a more concentrated
            //shiny area, while a lower number indicates a larger shiny area.  The
            //shininess must be between 0 and 128.
            glMaterialf(GL_FRONT, GL_SHININESS, m_Shininess);
            break;
        case mShader:
        default:
            printf("CMaterial::Appy() - wrong or not ready material type (%d)\n",m_MaterialType);
    }
    Render->SetBlendMode(m_blend_mode);
    if(m_DiffuseMap!=NULL){
        GL.Enable(GL_TEXTURE_2D);
        m_DiffuseMap->Bind();
        Render->TextureOperation(m_TextureCombine);
    }else{
        GL.Disable(GL_TEXTURE_2D);
    }
};
