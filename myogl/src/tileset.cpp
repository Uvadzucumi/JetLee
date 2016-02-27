#include "tileset.h"

using namespace MyOGL;

CTileset::CTileset(CTexture *texture, int tile_x_size, int tile_y_size){
    CHudSprite *sprite;
    int i,j;
    if(!texture){
        Log->puts("CTiles::CTiles() - empty texture\n");
    }

    m_tileset_texture=texture;
    for(i=0;i<texture->GetHeight();i+=tile_y_size){
        for(j=0;j<texture->GetWidth();j+=tile_x_size){
            sprite=new CHudSprite(texture);
            m_tiles.push_back(sprite);
            sprite->setUVPixelCoords(j,i,tile_x_size,tile_y_size);
            sprite->setSize(tile_x_size,tile_y_size);
        }
    }
    m_tile_width=tile_x_size;
    m_tile_height=tile_y_size;
    m_dec_width=0; m_dec_height=0; // decrease tile=0;
    MyOGL::Log->printf("Created %d tiles tileset\n",m_tiles.size());
};

CTileset::~CTileset(){
    MyOGL::Log->puts("Clear TilesList\n");
    for(unsigned int i=0;i<m_tiles.size();i++){
        delete m_tiles[i];
    }
    m_tiles.clear();
};

// render sprite in special coords
void CTileset::RenderAt(int x, int y, int tile_id, CMaterial *material){
    glTranslatef(x,y,0);
    //TilesList[tile_id]->SetMaterial(material);
    Render(tile_id, material);
    glTranslatef(-x,-y,0);
};

void CTileset::Render(int tile_id, CMaterial *material){
    m_tiles[tile_id]->render(material);
};

void CTileset::setTileDecrease(int dec_width, int dec_height){
    if(m_dec_width!=dec_width || m_dec_height!=dec_height){
        m_dec_width=dec_width;
        m_dec_height=dec_height;
        int new_w=m_tile_width-m_dec_width;
        int new_h=m_tile_height-m_dec_height;
        for(unsigned int i=0; i < m_tiles.size(); i++){
            m_tiles[i]->setSize(new_w, new_h);
        }
    }
}
