#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "../myogl/src/myogl.h"
#include "../myogl/src/ui/panel.h"

#define SPAWN_TIME 3
#define SPAWN_TIME_DIE 5

extern MyOGL::CApplication *App;
extern int hatch_flares_count;
extern bool game_over;
extern int hero_index;

#endif // GAME_H_INCLUDED
