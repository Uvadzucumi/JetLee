#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "../myogl/src/myogl.h"
#include "../myogl/src/3rd/enum.h"

#include <vector>
#include <string>

BETTER_ENUM(ESprites, int,
    // SM (0)
    SM_STAY=0,
    SM_RUN0,
    SM_RUN1,
    SM_PUSH,
    SM_DIE,
    // FM (5)
    FM_STAY,
    FM_RUN0,
    FM_RUN1,
    FM_UP,
    FM_DOWN,
    FM_CLIMB0,
    FM_CLIMB1,
    FM_PUSH,
    FM_KICK,
    FM_DIE,
    // JL (15)
    JL_STAY,
    JL_RUN0,
    JL_RUN1,
    JL_UP,
    JL_DOWN,
    JL_CLIMB0,
    JL_CLIMB1,
    JL_PUSH,
    JL_KICK,
    JL_JUMP,
    JL_DIE,
    JL_HIDE,
    // GAME OBJECTS
    FLARE,
    // LOCATIONS
    LOCATION0,
    LOCATION1,
    LOCATION2,

    DEBUG_BLOCK,
    DEBUG_FLARE,
    DEBUG_STAIRS,
    DEBUG_PORTAL,
    CONGRATULATION,
    ENTER_TO_CONTINUE
)

extern std::vector<MyOGL::CTexture*> textures;
extern std::vector<MyOGL::CHudSprite*> sprites;

extern bool loadGraphics(int sprite_scale);

#endif // GRAPHICS_H_INCLUDED
