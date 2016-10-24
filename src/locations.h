#ifndef LOCATIONS_H_INCLUDED
#define LOCATIONS_H_INCLUDED

#include "../myogl/src/vector_types.h"
#include "graphics.h"
#include "../myogl/src/3rd/enum.h"


#define LOCATION_GRID_SIZE 4

#define LOCATION_WIDTH 168
#define LOCATION_HEIGHT 124

#define LOCATION_GRID_WIDTH 42
#define LOCATION_GRID_HEIGHT 31
// flare coords in map grid. map grix - 4x4 pixels

typedef struct{
    MyOGL::Vector2i position;
    bool collected;
} SFlare;

typedef struct{
    MyOGL::Vector2i hero_spawn, enemy_spawn;
    int x, y, w, h;
    int location;
} SPortal;


BETTER_ENUM(
        EBlockTypes,
        int,
        TILE_EMPTY=0,
        TILE_FLARE,
        TILE_WALL,
        TILE_PORTAL,
        TILE_STAIRS
)


class CLocation{

        std::vector<SFlare> flares;
        std::vector<SPortal> portals;

        int m_scale_factor;
        int m_location_sprite_index;
        bool display_blocks;

        int map_blocks[LOCATION_HEIGHT][LOCATION_WIDTH];

        MyOGL::Vector2i hero_spawn;
        MyOGL::Vector2i enemy_spawn;

    public:
        CLocation(){
            m_scale_factor=1;
            m_location_sprite_index=-1;
            display_blocks=false;
            for(int y=0; y<LOCATION_GRID_HEIGHT; y++){
                for(int x=0; x<LOCATION_GRID_WIDTH; x++){
                    map_blocks[y][x]=EBlockTypes::TILE_EMPTY;
                }
            }
        }

        void addBlock(int x, int y, int w, int h=1, int block_type=EBlockTypes::TILE_WALL){

            for(int dy=y; dy<(y+h);dy++){
                for(int dx=x; dx<(x+w); dx++){
                    map_blocks[dy][dx]=block_type;
                }
            }

        }

        void setSpriteIndex(int sprite_index){
            m_location_sprite_index=sprite_index;
        }

        void addFlare(int x, int y){
            SFlare flare;
            flare.position.x=x;
            flare.position.y=y;
            flare.collected=false;
            map_blocks[y][x]=EBlockTypes::TILE_FLARE;
            flares.push_back(flare);
        }

        void setScaleFactor(int scale_factor){
            m_scale_factor=scale_factor;
        }

        const int getFlaresCount(){
            return flares.size();
        }

        void setDisplayBlocks(bool show){
            this->display_blocks=show;
        }

        void setHeroSpawn(int x, int y){
            hero_spawn.set(x, y);
        }

        MyOGL::Vector2i getHeroSpawn(){
            return hero_spawn;
        }

        void setEnemySpawn(int x, int y){
            enemy_spawn.set(x, y);
        }

        MyOGL::Vector2i getEnemySpawn(){
            return enemy_spawn;
        }

        bool isTile(int x, int y, int tile_type){
            if(x<0 || y<0 || x >= LOCATION_GRID_WIDTH || y >= LOCATION_GRID_HEIGHT){
                return false;
            }
            return map_blocks[y][x]==tile_type;
        }

        bool collectFlareByCoords(int x, int y){
            if(this->isTile(x, y, EBlockTypes::TILE_FLARE)){
                map_blocks[y][x]=EBlockTypes::TILE_EMPTY;
                for(int i=0; i < (int)flares.size(); i++){
                    if(flares[i].position.x==x && flares[i].position.y==y){
                        flares[i].collected=true;
                    }
                }
                return true;
            }
            return false;
        }

        void addPortal(int x, int y, int w, int h, int location, MyOGL::Vector2i hero_spawn, MyOGL::Vector2i enemy_spawn){
            this->addBlock(x, y, w, h, EBlockTypes::TILE_PORTAL);
            SPortal portal;
            portal.x=x;
            portal.y=y;
            portal.w=w;
            portal.h=h;
            portal.location=location;
            portal.enemy_spawn=enemy_spawn;
            portal.hero_spawn=hero_spawn;
            portals.push_back(portal);
        }

        SPortal getPortalByCoords(int x, int y){
            SPortal tmp;
            tmp.location=-1;
            for(int i=0; i < (int)portals.size(); i++){
                if(x>=portals[i].x && x<portals[i].x+portals[i].w && y>=portals[i].y && y<portals[i].y+portals[i].h){
                    return portals[i];
                }
            }
            return tmp;
        }

        void render(){
            // render level in pos 0, 0
            sprites[m_location_sprite_index]->render();
            // render flare (if not collected)
            for(int i=0; i < (int)flares.size(); i++){
                if(!flares[i].collected){
                // render flare
                    sprites[SPRITE_FLARE]->setPosition(
                        (flares[i].position.x * LOCATION_GRID_SIZE-1) * m_scale_factor,
                        (flares[i].position.y * LOCATION_GRID_SIZE-1) * m_scale_factor);
                    sprites[SPRITE_FLARE]->render();
                }
            }

            if(this->display_blocks){
                for(int y=0; y<LOCATION_GRID_HEIGHT; y++){
                    for(int x=0; x<LOCATION_GRID_WIDTH; x++){
                        if(map_blocks[y][x]!=EBlockTypes::TILE_EMPTY){
                            int sprite_index;
                            if(map_blocks[y][x]==EBlockTypes::TILE_WALL){
                                sprite_index=SPRITE_DEBUG_BLOCK;
                            }else if(map_blocks[y][x]==EBlockTypes::TILE_FLARE){
                                sprite_index=SPRITE_DEBUG_FLARE;
                            }else if(map_blocks[y][x]==EBlockTypes::TILE_STAIRS){
                                sprite_index=SPRITE_DEBUG_STAIRS;
                            }else if(map_blocks[y][x]==EBlockTypes::TILE_PORTAL){
                                sprite_index=SPRITE_DEBUG_PORTAL;
                            }

                            sprites[sprite_index]->setPosition(
                                (x * LOCATION_GRID_SIZE) * m_scale_factor,
                                (y * LOCATION_GRID_SIZE) * m_scale_factor);
                            sprites[sprite_index]->render();
                        }
                    }
                }
            }


        }

};

extern std::vector<CLocation*> locations;
extern int current_location;

extern bool initLocations(int scale_factor);

#endif // LOCATIONS_H_INCLUDED
