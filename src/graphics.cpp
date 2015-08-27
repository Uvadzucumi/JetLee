
#include "graphics.h"

#define CREATE_SPRITE_MACRO(NAME, TEXTURE_NUM, LEFT, TOP, RIGHT, BOTTOM, SCALE) \
  sprite=new MyOGL::CHudSprite(textures[TEXTURE_NUM]); \
  sprite->setUVPixelCoords(LEFT, TOP, RIGHT - LEFT, BOTTOM - TOP); \
  sprite->setSize( (RIGHT - LEFT) * SCALE, (BOTTOM - TOP) * SCALE); \
  sprites.push_back(sprite);

std::vector<MyOGL::CTexture*> textures;
std::vector<MyOGL::CHudSprite*> sprites;

MyOGL::CTexture *actor_texture[3];
MyOGL::CHudSprite *sm_sprites[4];

bool loadGraphics(int sprite_scale){
    std::vector<std::string> texture_files={
        "sprites.png"
    };

    // load textures
    std::string textures_path="data/";

    for(int i=0; i < (int)texture_files.size(); ++i){
        MyOGL::CTexture *tex=new MyOGL::CTexture();
        tex->setMinFilter(GL_NEAREST);
        tex->setMagFilter(GL_NEAREST);
        std::string file_name=textures_path+texture_files[i];
        if(!tex->LoadFromFile(file_name.c_str())){
            std::cout << "Graphics load error! in: " <<  texture_files[i] << std::endl;
            return false;
        }
        textures.push_back(tex);
    }

    MyOGL::CHudSprite *sprite;
    // set sprite by texture coords
    // NAME, TEXTURE, LEFT, TOP, RIGHT, BOTTOM
    // SM (0)
    CREATE_SPRITE_MACRO("stay", 0, 0, 0, 8, 11, sprite_scale);
    CREATE_SPRITE_MACRO("run0", 0, 26, 0, 35, 14, sprite_scale);
    CREATE_SPRITE_MACRO("run1", 0, 8, 0, 16, 14, sprite_scale);
    CREATE_SPRITE_MACRO("push", 0, 35, 0, 47, 11, sprite_scale);
    CREATE_SPRITE_MACRO("die" , 0, 16, 0, 26, 6, sprite_scale);
    // FM (5)
    CREATE_SPRITE_MACRO("stay", 0, 47, 0, 60, 14, sprite_scale);
    CREATE_SPRITE_MACRO("run0", 0, 60, 0, 72, 14, sprite_scale);
    CREATE_SPRITE_MACRO("run1", 0, 72, 0, 83, 14, sprite_scale);
    CREATE_SPRITE_MACRO("up"  , 0, 83, 0, 94, 14, sprite_scale);
    CREATE_SPRITE_MACRO("dwn" , 0, 94, 0, 107, 14, sprite_scale);
    CREATE_SPRITE_MACRO("climb0", 0, 107, 0, 120, 14, sprite_scale);
    CREATE_SPRITE_MACRO("climb1", 0, 120, 0, 133, 14, sprite_scale);
    CREATE_SPRITE_MACRO("push", 0, 133, 0, 148, 14, sprite_scale);
    CREATE_SPRITE_MACRO("kick", 0, 148, 0, 166, 13, sprite_scale);
    CREATE_SPRITE_MACRO("die", 0, 166, 0, 182, 11, sprite_scale);
    // JL (15)
    CREATE_SPRITE_MACRO("stay", 0, 21, 14, 29, 28, sprite_scale);
    CREATE_SPRITE_MACRO("run0", 0, 29, 14, 38, 28, sprite_scale);
    CREATE_SPRITE_MACRO("run1", 0, 38, 14, 44, 28, sprite_scale);
    CREATE_SPRITE_MACRO("up", 0, 44, 14, 53, 28, sprite_scale);
    CREATE_SPRITE_MACRO("dwn", 0, 53, 14, 64, 28, sprite_scale);
    CREATE_SPRITE_MACRO("climb0", 0, 64, 14, 75, 28, sprite_scale);
    CREATE_SPRITE_MACRO("climb1", 0, 75, 14, 86, 28, sprite_scale);
    CREATE_SPRITE_MACRO("push", 0, 86, 14, 98, 28, sprite_scale);
    CREATE_SPRITE_MACRO("kick", 0, 98, 14, 114, 24, sprite_scale);
    CREATE_SPRITE_MACRO("jump", 0, 114, 14, 127, 28, sprite_scale);
    CREATE_SPRITE_MACRO("die", 0, 127, 14, 143, 21, sprite_scale);
    CREATE_SPRITE_MACRO("hide", 0, 143, 14, 160, 21, sprite_scale);
    // game objects
    CREATE_SPRITE_MACRO("flare", 0, 21, 8, 26, 14, sprite_scale);

    CREATE_SPRITE_MACRO("location0", 0, 0, 28, 168, 152, sprite_scale);
    CREATE_SPRITE_MACRO("location1", 0, 0, 152, 168, 276, sprite_scale);
    CREATE_SPRITE_MACRO("location2", 0, 0, 276, 168, 400, sprite_scale);

    CREATE_SPRITE_MACRO("debug_block", 0, 17, 10, 21, 14, sprite_scale);
    CREATE_SPRITE_MACRO("debug_flare", 0, 0, 14, 4, 18, sprite_scale);
    CREATE_SPRITE_MACRO("debug_stairs", 0, 4, 14, 8, 18, sprite_scale);
    CREATE_SPRITE_MACRO("debug_portal", 0, 8, 14, 12, 18, sprite_scale);

    CREATE_SPRITE_MACRO("congratulation", 0, 0, 400, 64, 464, sprite_scale);

    CREATE_SPRITE_MACRO("enter_to_continue", 0, 64, 400, 104, 413, sprite_scale);

    return true;

}

