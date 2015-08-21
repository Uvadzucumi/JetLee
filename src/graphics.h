#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "../myogl/src/myogl.h"

#include <vector>
#include <string>

enum {
    // SM (0)
    SPRITE_SM_STAY=0,
    SPRITE_SM_RUN0,
    SPRITE_SM_RUN1,
    SPRITE_SM_PUSH,
    SPRITE_SM_DIE,
    // FM (5)
    SPRITE_FM_STAY,
    SPRITE_FM_RUN0,
    SPRITE_FM_RUN1,
    SPRITE_FM_UP,
    SPRITE_FM_DOWN,
    SPRITE_FM_CLIMB0,
    SPRITE_FM_CLIMB1,
    SPRITE_FM_PUSH,
    SPRITE_FM_KICK,
    SPRITE_FM_DIE,
    // JL (15)
    SPRITE_JL_STAY,
    SPRITE_JL_RUN0,
    SPRITE_JL_RUN1,
    SPRITE_JL_UP,
    SPRITE_JL_DOWN,
    SPRITE_JL_CLIMB0,
    SPRITE_JL_CLIMB1,
    SPRITE_JL_PUSH,
    SPRITE_JL_KICK,
    SPRITE_JL_JUMP,
    SPRITE_JL_DIE,
    SPRITE_JL_HIDE,
    // GAME OBJECTS
    SPRITE_FLARE,
    // LOCATIONS
    SPRITE_LOCATION0,
    SPRITE_LOCATION1,
    SPRITE_LOCATION2,

    SPRITE_DEBUG_BLOCK,
    SPRITE_DEBUG_FLARE,
    SPRITE_DEBUG_STAIRS,
    SPRITE_DEBUG_PORTAL,
    SPRITE_CONGRATULATION,
};

extern std::vector<MyOGL::CTexture*> textures;
extern std::vector<MyOGL::CHudSprite*> sprites;

extern bool loadGraphics(int sprite_scale);

#endif // GRAPHICS_H_INCLUDED
