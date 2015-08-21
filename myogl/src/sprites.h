#ifndef SPRITES_H_INCLUDED
#define SPRITES_H_INCLUDED

#include "vector_types.h"
#include "render.h"
#include "texture.h"
#include "material.h"

namespace MyOGL{

    class CHudSprite{
            MyGlBlendMode BlendMode;
            //int TextureWidth, TextureHeight;
            CTexture *m_texture;
            CMaterial *m_material;
            Vector2f TextCoords[2];
            // sprite position
            int m_left, m_top;
            //int m_width, m_height;
            int m_right, m_bottom;
            float m_angle;    // rotate angle
            // Matherial
            bool m_mirroring;
        public:
            CHudSprite(CTexture *texture, CMaterial *material=NULL){
                // set inicial values
                m_angle=0;
                m_left=0; m_top=0;  // default sprite position
                // set material
                m_material=material;
                // dafault sprite size from texture
                m_texture=texture;
                setSize(m_texture->GetWidth(),m_texture->GetHeight());
                // default text coords for full texture
                TextCoords[0].u=0.0; TextCoords[0].v=0.0;
                TextCoords[1].u=1.0; TextCoords[1].v=1.0;
                // set color material
                // default blend mode
                // if in texture have alfa - set Source
                if(texture->IsAlpha()){
                    BlendMode=blSource;
                }else{
                    BlendMode=blNone;
                }
                m_mirroring=false;
                //Log->puts("Created HudSprite\n");
            };

            void setMirroring(bool mirroring){
                m_mirroring=mirroring;
            }

            ~CHudSprite(){
                //Log->puts("Deleted HudSprite\n");
            };
            void rotate(float angle){ m_angle=angle; };
            void setPosition(int left, int top);
            void setSize(int width, int height);
            //void SetTextureCoords(int text_left, int text_top, int text_right, int text_bottom);
            void setTransparent(MyGlBlendMode mode){ BlendMode=mode; }
            void render(CMaterial *material=NULL);
            void setUVCoords(float t0_u, float t0_v, float t1_u, float t1_v){
                TextCoords[0].u=t0_u; TextCoords[0].v=t0_v;
                TextCoords[1].u=t1_u; TextCoords[1].v=t1_v;
            };
            void setUVPixelCoords(int left, int top, int width, int height){
                setUVCoords((float)left/m_texture->GetWidth(),(float)top/m_texture->GetHeight(),(float)(left+width)/m_texture->GetWidth(),(float)(top+height)/m_texture->GetHeight());
            }
            int getWidth(){ return m_right-m_left; };
            int getHeight(){ return m_bottom-m_top; };
            int getTextureWidth(){ return m_texture->GetWidth();};
            int getTextureHeight(){ return m_texture->GetHeight();};
            // set color material value
            //void SetColorMaterial(float r, float g, float b){ m_ColorMaterial.r=r; m_ColorMaterial.g=g; m_ColorMaterial.b=b; }
            void setMaterial(CMaterial *material){ m_material=material;};
    };

}
#endif // SPRITES_H_INCLUDED
