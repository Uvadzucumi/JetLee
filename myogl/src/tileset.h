#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include "sprites.h"
#include "material.h"

namespace MyOGL{

// tileset class,
class CTileset{
        CTexture *m_tileset_texture;
        std::vector<CHudSprite*> m_tiles;
        int m_tile_width, m_tile_height;
        int m_dec_width, m_dec_height; // tile decrease in pixels
    public:
        CTileset(CTexture *texture, int tile_x_size=32, int tile_y_size=32);
        ~CTileset();
        // get current tile sprite
        CHudSprite *Tile(unsigned int tile_num){
            if(tile_num<m_tiles.size()){
                return m_tiles[tile_num];
            }else{
                MyOGL::Log->printf("CTileset::Tile(num) Warning! wrong tile number %d\n",tile_num);
                return m_tiles[0];
            }
        };
        void RenderAt(int x, int y, int tile_id, CMaterial *material=NULL);
        void Render(int tile_id, CMaterial *material=NULL);
        void setTileDecrease(int dec_width, int dec_height);
        const int getTileDecreaseW(){ return m_dec_width; }
        const int getTileDecreaseH(){ return m_dec_height; }
};


}

#endif // TILES_LANDSCAPE_H_INCLUDED

