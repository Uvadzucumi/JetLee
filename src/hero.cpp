
/*#include<cmath>*/

#include "hero.h"

#define NEW_HERO_ACTION_MACRO(ACTION_TYPE, FRAMES) \
    action.delta_position.clear();  \
    action.state_sprite_indexes.clear(); \
    anim.setFramesCout(FRAMES); \
    action.animation=anim; \
    action.type=ACTION_TYPE;

#define ADD_ACTION_KEYFRAME_MACRO(STATE_SPRITE_INDEX, DX, DY) \
    action.delta_position.push_back(MyOGL::Vector2i(DX, DY)); \
    action.state_sprite_indexes.push_back(STATE_SPRITE_INDEX);

std::vector<CBaseHero*> heroes;
/*
double distance(int x1, int x2, int y1, int y2){
    return sqrt(pow((x2 - x1),2) - pow((y2 - y1),2));
}
*/

void initHeroObjects(int scale_factor, int hero_index){
    CBaseHero *hero;
    // add animations and actions
    MyOGL::CAnimation anim;
    SHeroAction action;

    anim.setCurrentFrame(0);
    anim.setFrameRate(0.1);

    // Init SmallMan
    if(hero_index!=0){
        hero=new CEnemy();
    }else{
        hero=new CHero();
    }
    hero->setScaleFactor(scale_factor);
    // sprites by hero position
    hero->addStateSprite(ESprites::SM_STAY, -3, -11, 5, 11, true);
    hero->addStateSprite(ESprites::SM_RUN0, -5, -14, 6, 11, true);
    hero->addStateSprite(ESprites::SM_RUN1, -6, -14, 4, 11, true);
    hero->addStateSprite(ESprites::SM_PUSH, -3, -11, 5, 11, true);
    hero->addStateSprite(ESprites::SM_DIE,  -5, -6, 8, 6, true);

    //action.delta_position.clear();
    //action.state_sprite_indexes.clear();

    //anim.setFramesCout(1);
    //action.animation=anim;
    //action.delta_position.push_back(MyOGL::Vector2i(0,0));
    //action.state_sprite_indexes.push_back(0);
    NEW_HERO_ACTION_MACRO(HERO_ACTION_STAY, 1);
    ADD_ACTION_KEYFRAME_MACRO(0,0,0);
    hero->addAction(action);

    NEW_HERO_ACTION_MACRO(HERO_ACTION_RUN, 2);
    ADD_ACTION_KEYFRAME_MACRO(1,1,0);
    ADD_ACTION_KEYFRAME_MACRO(2,1,0);
    hero->addAction(action);

    NEW_HERO_ACTION_MACRO(HERO_ACTION_DOWN, 1);
    ADD_ACTION_KEYFRAME_MACRO(0,0,0);               // used stay sprite
    hero->addAction(action);

    NEW_HERO_ACTION_MACRO(HERO_ACTION_PUSH, 1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,0);
    hero->addAction(action);    // 3 - push

    NEW_HERO_ACTION_MACRO(HERO_ACTION_DIE, 6);
    ADD_ACTION_KEYFRAME_MACRO(4,0,0);
    ADD_ACTION_KEYFRAME_MACRO(4,2,0);
    ADD_ACTION_KEYFRAME_MACRO(4,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(4,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(4,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(4,-1,0);
    hero->addAction(action); // 4 - die
    hero->setMaxHealt(2);
    hero->setPushPosition(3,-7,6,4);
    hero->setKickPosition(0,0,0,0);
    heroes.push_back(hero); // index: 0 (HERO_SM)

    // Init FatMan
    if(hero_index!=1){
        hero=new CEnemy();
    }else{
        hero=new CHero();
    }
    hero->setScaleFactor(scale_factor);
    hero->addStateSprite(ESprites::FM_STAY, -7, -14, 8, 14, true);    // 0
    hero->addStateSprite(ESprites::FM_RUN0, -7, -14, 8, 14, true);    // 1
    hero->addStateSprite(ESprites::FM_RUN1, -7, -14, 8, 14, true);    // 2
    hero->addStateSprite(ESprites::FM_UP, -6, -14, 8, 14, false);     // 3
    hero->addStateSprite(ESprites::FM_DOWN, -7, -14, 8, 14, false);   // 4
    hero->addStateSprite(ESprites::FM_CLIMB0, -7, -14, 8, 14, false); // 5
    hero->addStateSprite(ESprites::FM_CLIMB1, -6, -14, 8, 14, false); // 6
    hero->addStateSprite(ESprites::FM_PUSH, -6, -14, 8, 14, true);    // 7
    hero->addStateSprite(ESprites::FM_KICK, -7, -12, 10, 12, true);   // 8
    hero->addStateSprite(ESprites::FM_DIE, -8, -11, 14, 11, true);    // 9

    //action.delta_position.clear();
    //action.state_sprite_indexes.clear();

    //anim.setFramesCout(1);
    //action.animation=anim;
    //action.delta_position.push_back(MyOGL::Vector2i(0,0));
    //action.state_sprite_indexes.push_back(0);
    NEW_HERO_ACTION_MACRO(HERO_ACTION_STAY, 1);
    ADD_ACTION_KEYFRAME_MACRO(0,0,0);
    hero->addAction(action);    // 0 - stay

    NEW_HERO_ACTION_MACRO(HERO_ACTION_RUN, 2);
    ADD_ACTION_KEYFRAME_MACRO(1,1,0);
    ADD_ACTION_KEYFRAME_MACRO(2,1,0);
    hero->addAction(action);    // 1 - run

    NEW_HERO_ACTION_MACRO(HERO_ACTION_UP, 7);
    ADD_ACTION_KEYFRAME_MACRO(3,0,0);
    ADD_ACTION_KEYFRAME_MACRO(3,0,-1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,-1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,-1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,1);
    hero->addAction(action);    // 3 - up

    NEW_HERO_ACTION_MACRO(HERO_ACTION_DOWN, 1);
    ADD_ACTION_KEYFRAME_MACRO(4,0,0);
    hero->addAction(action);    // 4 - down

    NEW_HERO_ACTION_MACRO(HERO_ACTION_CLIMB0, 1);
    ADD_ACTION_KEYFRAME_MACRO(5,0,0);
    hero->addAction(action);    // 5 - climkb

    NEW_HERO_ACTION_MACRO(HERO_ACTION_CLIMB1, 1);
    ADD_ACTION_KEYFRAME_MACRO(6,0,0);
    hero->addAction(action);    // 6 - climb

    NEW_HERO_ACTION_MACRO(HERO_ACTION_PUSH, 1);
    ADD_ACTION_KEYFRAME_MACRO(7,0,0);
    hero->addAction(action);    // 7 - push

    NEW_HERO_ACTION_MACRO(HERO_ACTION_KICK, 7);
    ADD_ACTION_KEYFRAME_MACRO(8,0,0);
    ADD_ACTION_KEYFRAME_MACRO(8,1,-2);
    ADD_ACTION_KEYFRAME_MACRO(8,1,-1);
    ADD_ACTION_KEYFRAME_MACRO(8,1,0);
    ADD_ACTION_KEYFRAME_MACRO(8,2,0);
    ADD_ACTION_KEYFRAME_MACRO(8,1,2);
    ADD_ACTION_KEYFRAME_MACRO(8,1,1);
    hero->addAction(action); // 8 - kick

    NEW_HERO_ACTION_MACRO(HERO_ACTION_DIE, 6);
    ADD_ACTION_KEYFRAME_MACRO(9,0,0);
    ADD_ACTION_KEYFRAME_MACRO(9,2,0);
    ADD_ACTION_KEYFRAME_MACRO(9,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(9,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(9,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(9,-1,0);
    hero->addAction(action); // 10 - die

    hero->setPushPosition(4,-8,5,2);
    hero->setKickPosition(5,-5,5,5);

    hero->setMaxHealt(3);
    heroes.push_back(hero); // index: 1 (HERO_FM)

    // Init JetLee
    if(hero_index!=2){
        hero=new CEnemy();
    }else{
        hero=new CHero();
    }
    hero->setScaleFactor(scale_factor);

    hero->addStateSprite(ESprites::JL_STAY, -4, -14, 6, 14, true);     //0
    hero->addStateSprite(ESprites::JL_RUN0, -5, -14, 7, 14, true);     //1
    hero->addStateSprite(ESprites::JL_RUN1, -3, -14, 6, 14, true);     //2
    hero->addStateSprite(ESprites::JL_UP, -4, -14, 5, 14, false);      //3
    hero->addStateSprite(ESprites::JL_DOWN, -5, -14, 5, 14, false);    //4
    hero->addStateSprite(ESprites::JL_CLIMB0, -5, -14, 7, 14, false);  //5
    hero->addStateSprite(ESprites::JL_CLIMB1, -5, -14, 7, 14, false);  //6
    hero->addStateSprite(ESprites::JL_PUSH, -5, -14, 8, 14, true);     //7
    hero->addStateSprite(ESprites::JL_KICK, -5, -8, 8, 8, true);       //8
    hero->addStateSprite(ESprites::JL_JUMP, -5, -14, 9, 14, true);     //9
    hero->addStateSprite(ESprites::JL_DIE, -8, -7, 12, 7, true);       //10
    hero->addStateSprite(ESprites::JL_HIDE, -9, -7, 14, 7, true);      //11

    NEW_HERO_ACTION_MACRO(HERO_ACTION_STAY, 1);
    ADD_ACTION_KEYFRAME_MACRO(0,0,0);
    hero->addAction(action);    // 0 - stay

    NEW_HERO_ACTION_MACRO(HERO_ACTION_RUN, 2);
    ADD_ACTION_KEYFRAME_MACRO(1,1,0);
    ADD_ACTION_KEYFRAME_MACRO(2,1,0);
    hero->addAction(action);    // 1 - run

    NEW_HERO_ACTION_MACRO(HERO_ACTION_UP, 7);
    ADD_ACTION_KEYFRAME_MACRO(3,0,0);
    ADD_ACTION_KEYFRAME_MACRO(3,0,-1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,-1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,-1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,1);
    ADD_ACTION_KEYFRAME_MACRO(3,0,1);
    hero->addAction(action);    // 3 - up

    NEW_HERO_ACTION_MACRO(HERO_ACTION_DOWN, 1);
    ADD_ACTION_KEYFRAME_MACRO(4, 0, 0);
    hero->addAction(action);    // 4 - down

    NEW_HERO_ACTION_MACRO(HERO_ACTION_CLIMB0, 1);
    ADD_ACTION_KEYFRAME_MACRO(5, 0, 0);
    hero->addAction(action);    // 5 - climkb

    NEW_HERO_ACTION_MACRO(HERO_ACTION_CLIMB1, 1);
    ADD_ACTION_KEYFRAME_MACRO(6, 0, 0);
    hero->addAction(action);    // 6 - climb

    NEW_HERO_ACTION_MACRO(HERO_ACTION_PUSH, 1);
    ADD_ACTION_KEYFRAME_MACRO(7, 0, 0);
    hero->addAction(action);    // 7 - push

    NEW_HERO_ACTION_MACRO(HERO_ACTION_KICK, 7);
    ADD_ACTION_KEYFRAME_MACRO(8,0,0);
    ADD_ACTION_KEYFRAME_MACRO(8,1,-2);
    ADD_ACTION_KEYFRAME_MACRO(8,1,-1);
    ADD_ACTION_KEYFRAME_MACRO(8,1,0);
    ADD_ACTION_KEYFRAME_MACRO(8,2,0);
    ADD_ACTION_KEYFRAME_MACRO(8,1,2);
    ADD_ACTION_KEYFRAME_MACRO(8,1,1);
    hero->addAction(action); // 8 - kick

    NEW_HERO_ACTION_MACRO(HERO_ACTION_JUMP, 8);
    ADD_ACTION_KEYFRAME_MACRO(9,0,0);
    ADD_ACTION_KEYFRAME_MACRO(9,1,-1);
    ADD_ACTION_KEYFRAME_MACRO(9,1,-1);
    ADD_ACTION_KEYFRAME_MACRO(9,1,-1);
    ADD_ACTION_KEYFRAME_MACRO(9,2,0);
    ADD_ACTION_KEYFRAME_MACRO(9,1,1);
    ADD_ACTION_KEYFRAME_MACRO(9,1,1);
    ADD_ACTION_KEYFRAME_MACRO(9,1,1);
    hero->addAction(action); // 9 - jump

    NEW_HERO_ACTION_MACRO(HERO_ACTION_HIDE, 1);
    ADD_ACTION_KEYFRAME_MACRO(11,0,0);
    hero->addAction(action); // 11 - hide

    NEW_HERO_ACTION_MACRO(HERO_ACTION_DIE, 6);
    ADD_ACTION_KEYFRAME_MACRO(10,0,0);
    ADD_ACTION_KEYFRAME_MACRO(10,2,0);
    ADD_ACTION_KEYFRAME_MACRO(10,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(10,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(10,-1,0);
    ADD_ACTION_KEYFRAME_MACRO(10,-1,0);
    hero->addAction(action); // 10 - die
    hero->setMaxHealt(3);

    hero->setPushPosition(4, -9, 3, 2);
    hero->setKickPosition(5, -3, 7, 2);

    heroes.push_back(hero); // index: 2 (HERO_JL)

}


// render current state sprite
void CBaseHero::render(){

    if(this->m_enabled && this->m_visible){
        //std::cout << "render: " << m_name << std::endl;
        if(state_sprites[m_state_sprite].sprite_id < sprites.size()){
            int dx=state_sprites[m_state_sprite].sprite_dx;
            if(state_sprites[m_state_sprite].orientation_depended){
                if(m_orientation == ORIENTATION_RIGHT){
                    sprites[state_sprites[m_state_sprite].sprite_id]->setMirroring(false);
                }else{
                    dx=-(sprites[state_sprites[m_state_sprite].sprite_id]->getWidth()/m_scale_factor)-state_sprites[m_state_sprite].sprite_dx;
                    sprites[state_sprites[m_state_sprite].sprite_id]->setMirroring(true);
                }
            }
            sprites[state_sprites[m_state_sprite].sprite_id]->setPosition(
                (m_position.x + dx) * m_scale_factor,
                (m_position.y + state_sprites[m_state_sprite].sprite_dy) * m_scale_factor
            );
            //std::cout << "hero x: " << m_x << " sprite pos x: " << (m_x + state_sprites[m_state_sprite].sprite_dx) * m_scale_factor << " factor: " << m_scale_factor << std::endl;
            sprites[state_sprites[m_state_sprite].sprite_id]->render();
            //std::cout << "hero state sprite: " << m_state_sprite << " hero_action: " << m_action << std::endl;

        }

    }
}

// return hero bbox
BBox CBaseHero::getBBox(){
    BBox b;
    b={{0,0},{0,0}};
    if(m_state_sprite >= 0){
        b.left_top.x=m_position.x-state_sprites[m_state_sprite].width/2;
        b.left_top.y=m_position.y-state_sprites[m_state_sprite].height;
        b.right_bottom.x=b.left_top.x+state_sprites[m_state_sprite].width;
        b.right_bottom.y=m_position.y;
    }
    return b;
}

void CBaseHero::Update(double DeltaTime){
}

void CBaseHero::hitToHero(int h_index){
    sound->play(SOUND_FIGHT);
    // set orientation
    if(m_position.x<heroes[h_index]->getPosition().x){
        heroes[h_index]->setOrientation(ORIENTATION_LEFT);
    }else{
        heroes[h_index]->setOrientation(ORIENTATION_RIGHT);
    }
    heroes[h_index]->setAction(HERO_ACTION_DIE);
    heroes[h_index]->decreaseHealth(1);
    if(h_index==hero_index && heroes[h_index]->getHealth()<=0){

        sound->play(SOUND_DIE);
        game_state=EGameState::GAME_STATE_END;

    }
}


void CHero::Update(double DeltaTime){
    if(this->m_enabled && (game_state==EGameState::GAME_STATE_PLAY || game_state==EGameState::GAME_STATE_END)){

        //log("game state: "<<game_state);

        if(this->actions[this->m_action].animation.onAnimate(DeltaTime)){

            if(
                isAction(HERO_ACTION_RUN)
            ){
                sound->play(SOUND_STEP);
            }

            MyOGL::Vector2i climb_movement={0,0};

            if(this->actions[this->m_action].animation.isFinished()){
                if(
                    (
                    isAction(HERO_ACTION_CLIMB0) ||
                    isAction(HERO_ACTION_CLIMB1)
                    ) && (App->IsKeyPressed(SDLK_UP) || App->IsKeyPressed(SDLK_DOWN) || App->IsKeyPressed(SDLK_LEFT) || App->IsKeyPressed(SDLK_RIGHT))
                ){
                    sound->play(SOUND_STEP);
                }
                // if player continue runnig
                if(this->isAction(HERO_ACTION_RUN) &&
                    (
                        //true
                        (App->IsKeyPressed(SDLK_LEFT) && m_orientation==ORIENTATION_LEFT) ||
                        (App->IsKeyPressed(SDLK_RIGHT) && m_orientation==ORIENTATION_RIGHT)
                    )
                ){

                    this->setAction(HERO_ACTION_RUN);

                }else{

                    if(this->isAction(HERO_ACTION_PUSH) && this->actions[this->m_action].animation.isFirstLoop()){
                        BBox hitbox;
                        if(m_orientation==ORIENTATION_RIGHT){
                            hitbox.left_top={m_position.x+push_position.left_top.x, m_position.y + push_position.left_top.y};
                        }else{
                            hitbox.left_top={m_position.x-(push_position.left_top.x+push_position.right_bottom.x), m_position.y + push_position.left_top.y};
                        }
                        hitbox.right_bottom={hitbox.left_top.x+push_position.right_bottom.x, hitbox.left_top.y+push_position.right_bottom.y};
                        //std::cout << "pos " << m_position.x << "," << m_position.y << std::endl;
                        //hitbox.debug();
                        for(int i=0; i<3; i++){ // only 2 enemy
                            if(i!=hero_index){
                                if(!heroes[i]->isAction(HERO_ACTION_DIE) && heroes[i]->isEnabled() && heroes[i]->isVisible()){ // active enemy
                                    // check bbox crossover
                                    BBox enemy_bbox=heroes[i]->getBBox();
                                    if(hitbox.intersect(enemy_bbox)){
                                        //std::cout << "HIT TO HERO " << i << std::endl;
                                        hitToHero(i);
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if(this->isAction(HERO_ACTION_PUSH) && App->IsKeyPressed(SDLK_LCTRL)){
                       this->continueAction(HERO_ACTION_PUSH);
                    }else{

                        if(this->isAction(HERO_ACTION_HIDE) &&  App->IsKeyPressed(SDLK_DOWN)){
                            this->setAction(HERO_ACTION_HIDE);
                        }else{

                            if(this->isAction(HERO_ACTION_CLIMB0) || this->isAction(HERO_ACTION_CLIMB1)){
                                // check climb move
                                if(App->IsKeyPressed(SDLK_LEFT)){
                                    climb_movement={-1,0};
                                }else if(App->IsKeyPressed(SDLK_RIGHT)){
                                    climb_movement={1,0};
                                }else if(App->IsKeyPressed(SDLK_UP)){
                                    climb_movement={0,-1};
                                }else if(App->IsKeyPressed(SDLK_DOWN)){
                                    climb_movement={0,1};
                                }

                            }else{

                                // if another action finished - hero stay - if not die
                                if(this->getHealth()<=0){
                                    log("game state set to FAIL");
                                    game_state=EGameState::GAME_STATE_FAIL;
                                    return;
                                }

                                this->setAction(HERO_ACTION_STAY);
                            }
                        }
                    }
                }
            }

            // need change coords and sprite
            int keyframe=this->actions[this->m_action].animation.getCurrentFrame();
            MyOGL::Vector2i delta_pos=this->actions[this->m_action].delta_position[keyframe];
            if(this->m_orientation==ORIENTATION_LEFT){
                delta_pos.x*=-1;
            }
            this->m_position+=delta_pos*MUL_COORDS;
            if( this->isAction(HERO_ACTION_KICK) ||
                this->isAction(HERO_ACTION_RUN) ||
                this->isAction(HERO_ACTION_JUMP) ||
                this->isAction(HERO_ACTION_DIE)){
                // check for wall
                    if(this->isInsideBlock(EBlockTypes::TILE_WALL)){
                        //std::cout << "WALL!!! need BACK!!! " << delta_pos.x << std::endl;
                        this->m_position.x-=delta_pos.x*MUL_COORDS;
                    }
            }

            if(climb_movement.x!=0 || climb_movement.y!=0){
                //std::cout << "need climb!" << std::endl;
                this->m_position+=climb_movement*4;
                if(isAction(HERO_ACTION_CLIMB0)){
                    setAction(HERO_ACTION_CLIMB1);
                }else{
                    setAction(HERO_ACTION_CLIMB0);
                }
                // check for finish climb
                if(!isInsideBlock(EBlockTypes::TILE_STAIRS)){
                    //this->m_position.y-=2;    // HACK FOR Y COORD FIX
                    setAction(HERO_ACTION_STAY);
                }

            }

            // check floor
            if(this->isAction(HERO_ACTION_STAY) || this->isAction(HERO_ACTION_RUN)){
                if(!this->isDownFloor()){
                    this->setAction(HERO_ACTION_DOWN);
                }
            }

            if(this->isAction(HERO_ACTION_DOWN) || (this->isAction(HERO_ACTION_DIE) && !this->isDownFloor())){
                this->m_position.y+=1*MUL_COORDS;
            }

            //this->m_state_sprite=

            //setStateSprite(this->actions[this->m_action].state_sprite_indexes[keyframe]);
            keyframe=this->actions[this->m_action].animation.getCurrentFrame();
            setStateSprite(this->actions[this->m_action].state_sprite_indexes[keyframe]);

            // kick ememies
            if(isAction(HERO_ACTION_KICK)){
                BBox hitbox;

                if(m_orientation==ORIENTATION_RIGHT){
                    hitbox.left_top={m_position.x+kick_position.left_top.x, m_position.y + kick_position.left_top.y};
                }else{
                    hitbox.left_top={m_position.x-kick_position.left_top.x-kick_position.right_bottom.x, m_position.y + kick_position.left_top.y};
                }
                hitbox.right_bottom={hitbox.left_top.x+kick_position.right_bottom.x, hitbox.left_top.y+kick_position.right_bottom.y};

                for(int i=0; i<3; i++){ // only 2 enemy
                    if(i!=hero_index){
                        if(!heroes[i]->isAction(HERO_ACTION_DIE) && heroes[i]->isEnabled() && heroes[i]->isVisible()){ // active enemy
                            // check bbox crossover
                            BBox enemy_bbox=heroes[i]->getBBox();
                            if(hitbox.intersect(enemy_bbox)){
                                //std::cout << "HIT TO HERO " << i << std::endl;
                                hitToHero(i);
                                break;
                            }
                        }
                    }
                }

            }

            // check flares
            if(isInsideBlock(EBlockTypes::TILE_FLARE)){
                MyOGL::Vector2i coords=this->getFoundInsideBlock();
                if(locations[current_location]->collectFlareByCoords(coords.x, coords.y)){
                    sound->play(SOUND_FLARE_COLLECT);
                    hatch_flares_count-=1;
                    if(!hatch_flares_count){
                        // open hatch
                        locations[1]->addBlock(19,30,3,1,EBlockTypes::TILE_EMPTY);
                        locations[1]->addPortal(19,30,3,1,3,MyOGL::Vector2i(0,0),MyOGL::Vector2i(0,0));
                        //std::cout << "Hatch unlocked!" << std::endl;
                    }
                }
            }

            if(this->isAction(HERO_ACTION_UP) && isInsideBlock(EBlockTypes::TILE_STAIRS)){
                this->setAction(HERO_ACTION_CLIMB0);
            }

            if(this->isInsideBlock(EBlockTypes::TILE_PORTAL)){
                // get portal coords & move to next level
                //std::cout << "PORTAL " << this->getFoundInsideBlock().x << "," << this->getFoundInsideBlock().y << std::endl;
                SPortal portal=locations[current_location]->getPortalByCoords(this->getFoundInsideBlock().x, this->getFoundInsideBlock().y);;
                if(portal.location==3){
                    game_over=true;
                }else{
                    if(portal.location==-1){
                        logW("Wrong PORTAL!!!");
                    }else{ // goto secon location

                        current_location=portal.location;
                        locations[current_location]->setHeroSpawn(portal.hero_spawn.x, portal.hero_spawn.y);
                        locations[current_location]->setEnemySpawn(portal.enemy_spawn.x, portal.enemy_spawn.y);
                        this->setPosition(
                            locations[current_location]->getHeroSpawn().x,
                            locations[current_location]->getHeroSpawn().y
                        );
                        // set enemy to spawn positions
                        double spawn_time=SPAWN_TIME;
                        for(int i=0; i<3; i++){
                            if(i!=hero_index){
                                ((CEnemy *)heroes[i])->goSpawnPoint(spawn_time);
                                spawn_time+=SPAWN_TIME;
                            }
                        }

                        this->setAction(HERO_ACTION_STAY);

                    }
                }
            }
/*
            std::cout << "new keyframe: " << keyframe << " action: " << this->m_action <<
                " state_sprite:" << this->actions[this->m_action].state_sprite_indexes[keyframe]
                << "dx, dy vector: " << delta_pos.x << "," << delta_pos.y
                << std::endl;
                */
        }


    }


}

void CEnemy::Update(double DeltaTime){
    if(this->m_enabled && (game_state==EGameState::GAME_STATE_PLAY || game_state==EGameState::GAME_STATE_END)){
        if(m_spawn_time>0){
            m_spawn_time-=DeltaTime;
        }else{
            m_spawn_time=0;

            if(m_wait_spawn){
                m_wait_spawn=false;
                //std::cout << m_name + " enters the world." << std::endl;
                ai->setEnable(true);
            }

            m_visible=true;

            ai->update(DeltaTime);

            if(this->actions[this->m_action].animation.onAnimate(DeltaTime)){

                MyOGL::Vector2i climb_movement={0,0};

                if(this->actions[this->m_action].animation.isFinished()){

                    // if player continue running
                    if(this->isAction(HERO_ACTION_RUN) && ai->isGoPoint()){
                        // check distance
                        MyOGL::Vector2i point=ai->getPoint();
                        int distance;
                        if(m_position.x>=point.x){
                            m_orientation=ORIENTATION_LEFT;
                            distance=m_position.x-point.x;
                            setAction(HERO_ACTION_RUN);
                        }else if(m_position.x<point.x){
                            m_orientation=ORIENTATION_RIGHT;
                            distance=point.x-m_position.x;
                            setAction(HERO_ACTION_RUN);
                        }
                        if(distance<=4){
                            setAction(HERO_ACTION_STAY);
                            ai->startSelectTask();
                        }else{
                            this->setAction(HERO_ACTION_RUN);
                        }
                    }else{

                        if(isAction(HERO_ACTION_DIE)){
                            health--;
                            if(health<=0){
                                this->goSpawnPoint();
                                return;
                            }
                        }

                        this->setAction(HERO_ACTION_STAY);

                    }

                }

// need change coords and sprite
                int keyframe=this->actions[this->m_action].animation.getCurrentFrame();
                MyOGL::Vector2i delta_pos=this->actions[this->m_action].delta_position[keyframe];
                if(this->m_orientation==ORIENTATION_LEFT){
                    delta_pos.x*=-1;
                }
                this->m_position+=delta_pos*MUL_COORDS;
                if( this->isAction(HERO_ACTION_KICK) ||
                    this->isAction(HERO_ACTION_RUN) ||
                    this->isAction(HERO_ACTION_JUMP) ||
                    this->isAction(HERO_ACTION_DIE)){
                    // check for wall
                        if(this->isInsideBlock(EBlockTypes::TILE_WALL)){
                            //std::cout << "WALL!!! need BACK!!! " << delta_pos.x << std::endl;
                            this->m_position.x-=delta_pos.x*MUL_COORDS;
                        }
                }

                // check floor
                if(this->isAction(HERO_ACTION_STAY) || this->isAction(HERO_ACTION_RUN)){
                    if(!this->isDownFloor()){
                        this->setAction(HERO_ACTION_DOWN);
                    }
                }

                if(this->isAction(HERO_ACTION_DOWN) || (this->isAction(HERO_ACTION_DIE) && !this->isDownFloor())){
                    this->m_position.y+=1*MUL_COORDS;
                }

            //this->m_state_sprite=

                //setStateSprite(this->actions[this->m_action].state_sprite_indexes[keyframe]);
                keyframe=this->actions[this->m_action].animation.getCurrentFrame();
                setStateSprite(this->actions[this->m_action].state_sprite_indexes[keyframe]);

                if(isAction(HERO_ACTION_STAY) && ai->isTaskReady()){
                    //std::cout << m_name << ": TASK READY & ENEMY STAY - NEED START ";
                    int task=ai->getTask();
                    MyOGL::Vector2i point=ai->getPoint();
                    int distance;
                    if(m_position.x>=point.x){
                        m_orientation=ORIENTATION_LEFT;
                        distance=m_position.x-point.x;
                        setAction(HERO_ACTION_RUN);
                    }else if(m_position.x<point.x){
                        m_orientation=ORIENTATION_RIGHT;
                        distance=point.x-m_position.x;
                        setAction(HERO_ACTION_RUN);
                    }
                    switch(task){
                        case AI_ACTION_MOVE_TO_FLOR_POINT:
                            if(distance<=4){
                                setAction(HERO_ACTION_STAY);
                                ai->startSelectTask();
                            }
                        break;
                        case AI_MOVE_TO_HERO_POINT:
                            if(distance <= 20 && this->getActionIndexByType(HERO_ACTION_KICK)){
                                // kick
                                    this->setAction(HERO_ACTION_KICK);
                                    this->ai->reset();
                                    this->ai->setEnable(true);
                                }else{
                                    if(distance<=4){
                                        setAction(HERO_ACTION_PUSH);
                                        ai->startSelectTask();
                                        this->ai->reset();
                                        this->ai->setEnable(true);
                                    }
                                }
                            break;
                        default:
                            logW("unknown task: " << task );
                    }

                }

                // FIGHT ACTIONS
                if(this->isAction(HERO_ACTION_PUSH)){
                    log(m_name << " : PUSH ACTION")
                    BBox hitbox;
                    if(m_orientation==ORIENTATION_RIGHT){
                        hitbox.left_top={m_position.x+push_position.left_top.x, m_position.y + push_position.left_top.y};
                    }else{
                        hitbox.left_top={m_position.x-(push_position.left_top.x+push_position.right_bottom.x), m_position.y + push_position.left_top.y};
                    }
                    hitbox.right_bottom={hitbox.left_top.x+push_position.right_bottom.x, hitbox.left_top.y+push_position.right_bottom.y};
                    //std::cout << "pos " << m_position.x << "," << m_position.y << std::endl;
                    //hitbox.debug();
                    for(int i=0; i<3; i++){ // only 2 enemy
                        if(i!=m_owner_index){
                            //log("chack hero id="<<i);
                            if( !heroes[i]->isAction(HERO_ACTION_DIE) &&
                                !heroes[i]->isAction(HERO_ACTION_KICK) &&
                                !heroes[i]->isAction(HERO_ACTION_HIDE) &&
                                 heroes[i]->isEnabled() &&
                                 heroes[i]->isVisible()){ // active enemy
                                // check bbox crossover
                                BBox enemy_bbox=heroes[i]->getBBox();
                                if(hitbox.intersect(enemy_bbox)){
                                    //std::cout << "HIT TO HERO " << i << std::endl;
                                    hitToHero(i);
                                    break;
                                }else{
                                    //log("MISS");
                                }
                            }else{
                                //log("FALSE");
                            }
                        }
                    }
                }

                // kick ememies
            if(isAction(HERO_ACTION_KICK)){
                BBox hitbox;

                if(m_orientation==ORIENTATION_RIGHT){
                    hitbox.left_top={m_position.x+kick_position.left_top.x, m_position.y + kick_position.left_top.y};
                }else{
                    hitbox.left_top={m_position.x-kick_position.left_top.x-kick_position.right_bottom.x, m_position.y + kick_position.left_top.y};
                }
                hitbox.right_bottom={hitbox.left_top.x+kick_position.right_bottom.x, hitbox.left_top.y+kick_position.right_bottom.y};

                for(int i=0; i<3; i++){ // only 2 enemy
                    if(i!=m_owner_index){
                            if( !heroes[i]->isAction(HERO_ACTION_DIE) &&
                                !heroes[i]->isAction(HERO_ACTION_KICK) &&
                                !heroes[i]->isAction(HERO_ACTION_HIDE) &&
                                 heroes[i]->isEnabled() &&
                                 heroes[i]->isVisible()){ // active enemy
                            // check bbox crossover
                            //log(getName()<<" KICK to " << heroes[i]->getName());
                            BBox enemy_bbox=heroes[i]->getBBox();
                            if(hitbox.intersect(enemy_bbox)){
                                hitToHero(i);
                                break;
                            }else{
                                //log("MISS");
                            }
                        }
                    }
                }

            }



        } // if animate
    } // if spawned

    } // if enable
}
