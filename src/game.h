#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../myogl/src/myogl.h"
#include "../myogl/src/ui/panel.h"
#include "../myogl/src/3rd/enum.h"
#include "gamesound.h"

#define SPAWN_TIME 3
#define SPAWN_TIME_DIE 5

BETTER_ENUM(
        EGameState,
        int,
        GAME_STATE_PLAY=0,
        GAME_STATE_PAUSE,
        GAME_STATE_WIN,
        GAME_STATE_END,
        GAME_STATE_FAIL
)

extern MyOGL::CApplication *App;
extern int hatch_flares_count;
extern bool game_over;
extern int hero_index;
extern CGameSound *sound;
extern int game_state;


#endif // GAME_H_INCLUDED
