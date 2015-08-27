#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED

#include "game.h"
#include "graphics.h"
#include "locations.h"
#include "ai.h"

#define MUL_COORDS 4

enum {
    HERO_SM=0,
    HERO_FM,
    HERO_JL
};

enum {
    HERO_ACTION_STAY=0,
    HERO_ACTION_RUN,
    HERO_ACTION_JUMP,
    HERO_ACTION_UP,
    HERO_ACTION_DOWN,
    HERO_ACTION_PUSH,   // 5
    HERO_ACTION_KICK,   // 6
    HERO_ACTION_HIDE,
    HERO_ACTION_CLIMB0,
    HERO_ACTION_CLIMB1,
    HERO_ACTION_DIE
};

class BBox {
    public:
    MyOGL::Vector2i left_top;
    MyOGL::Vector2i right_bottom;
    bool intersect(BBox box){
        return (this->left_top.x < box.right_bottom.x &&
        this->right_bottom.x > box.left_top.x &&
        this->left_top.y < box.right_bottom.y &&
        this->right_bottom.y > box.left_top.y);
    }

    void debug(){
        std::cout << this->left_top.x << "," << this->left_top.y  <<
        " - " << this->right_bottom.x << "," << this->right_bottom.y << std::endl;
    }
};

struct SHeroStateSprite{
    unsigned int sprite_id; // index in sprites vector
    // sprite dx, dy from hero position
    int sprite_dx;
    int sprite_dy;
    // hero bbox, by hero_position
    int width; // state width by player position
    int height; // state height by player position
    bool orientation_depended;
};

typedef enum {
    ORIENTATION_LEFT,
    ORIENTATION_RIGHT
} eHeroOrientation;

struct SHeroAction{
    int type; // HERO_ACTION_STAY, RUN, etc...
    MyOGL::CAnimation animation;
    std::vector<MyOGL::Vector2i> delta_position;
    std::vector<int> state_sprite_indexes;
};

class CBaseHero{
protected:
    // hero position
    MyOGL::Vector2i m_position;
    //int m_state;
    int m_state_sprite;

    bool m_enabled;

    eHeroOrientation m_orientation;

    int m_scale_factor;

    std::vector<SHeroStateSprite> state_sprites;
    std::vector<SHeroAction> actions;

    MyOGL::Vector2i m_last_found_block;

    std::string m_name;

    bool m_visible;

    int m_action;

    int m_health;
    int m_max_health;

    BBox kick_position, push_position;

    int m_owner_index;

public:
    CBaseHero(int x=0, int y=0){
        m_position={x,y};
        m_state_sprite=-1;
        m_enabled=false;
        m_orientation=ORIENTATION_RIGHT;
        m_scale_factor=1;
        m_action=0;
        m_name="";
        m_visible=false;
        m_owner_index=hero_index;
        m_health=m_max_health;
    }

    void decreaseHealth(int dh){
        m_health-=dh;
    }

    int getHealth(){
        //log("health: " << m_health);
        return m_health;
    }

    std::string getName(){
        return m_name;
    }

    void setIndex(int owner_index){
        m_owner_index=owner_index;
    }

    void setKickPosition(int x, int y, int w, int h){
        kick_position.left_top={x,y};
        kick_position.right_bottom={w,h};
    }
    void setPushPosition(int x, int y, int w, int h){
        push_position.left_top={x,y};
        push_position.right_bottom={w,h};
    }

    void setMaxHealt(int max_health){
        m_max_health=max_health;
        m_health=m_max_health;
    }

    const bool isEnabled(){
        return m_enabled;
    }

    const bool isVisible(){
        return m_visible;
    }

    void setVisible(bool visible){
        m_visible=visible;
    }

    void setName(std::string name){
        m_name=name;
    }

    void setEnable(bool enable_state){
        m_enabled=enable_state;
    }

    void setScaleFactor(int scale_factor){
        m_scale_factor=scale_factor;
    }

    void addStateSprite(int sprite_index, int sprite_dx, int sprite_dy, int bb_width, int bb_height, bool orientation_depended){
        SHeroStateSprite state_sprite;
        state_sprite.sprite_id=sprite_index;
        state_sprite.sprite_dx=sprite_dx;
        state_sprite.sprite_dy=sprite_dy;
        state_sprite.width=bb_width;
        state_sprite.height=bb_height;
        state_sprite.orientation_depended=orientation_depended;
        state_sprites.push_back(state_sprite);
    }

    void addAction(SHeroAction action){
        actions.push_back(action);
    }

    void setAction(int action_type){
        this->m_action=getActionIndexByType(action_type);
        this->actions[this->m_action].animation.Start();
        int keyframe=this->actions[this->m_action].animation.getCurrentFrame();
        //this->m_state_sprite=this->actions[this->m_action].state_sprite_indexes[keyframe];
        setStateSprite(this->actions[this->m_action].state_sprite_indexes[keyframe]);
    }

    void continueAction(int action_type){
        this->m_action=getActionIndexByType(action_type);
        this->actions[this->m_action].animation.Continue();
        int keyframe=this->actions[this->m_action].animation.getCurrentFrame();
        setStateSprite(this->actions[this->m_action].state_sprite_indexes[keyframe]);
    }
    // change state sprite
    void setStateSprite(int state_sprite_index){
        if(state_sprite_index >= 0 && (unsigned int) state_sprite_index < state_sprites.size()){
            m_state_sprite=state_sprite_index;
        }else{
            int keyframe=this->actions[this->m_action].animation.getCurrentFrame();
            std::cout << "CHero::setStateSprite() - wrong state_sprite_index number " << state_sprite_index << std::endl;
            std::cout << "m_action_index=" << this->m_action << "keyframe=" << keyframe
            << "state_sprite_index: " << this->actions[this->m_action].state_sprite_indexes[keyframe] << " indexes count " << this->actions[this->m_action].state_sprite_indexes.size() << std::endl;
            std::cout << "OK" << std::endl;
        }
    }

    // change position
    void setPosition(int x, int y){
        m_position={x, y};
    }

    // set orientation
    void setOrientation(eHeroOrientation orientation){
        this->m_orientation=orientation;
    }

    // render current state sprite
    void render();

    const int getStateSpritesCount(){
        return (int) state_sprites.size();
    }

    const MyOGL::Vector2i getPosition(){
        return m_position;
    }

    // left, top, right, bottom
    BBox getBBox();

    virtual void Update(double DeltaTime);

    bool isAction(int action_type){
        // search index
        int action_index=this->getActionIndexByType(action_type);
        return m_action==action_index;
    }

    int getActionIndexByType(int action_type){
        for(int i=0; i<(int)actions.size(); i++){
            if(action_type==actions[i].type){
                return i;
            }
        }
        return 0;   // return default stay action
    }

    // check floor in hero bottom
    bool isDownFloor(){
        BBox bbox=this->getBBox();
        // display down tiles by hero bbox
        MyOGL::Vector2i down={bbox.left_top.x/LOCATION_GRID_SIZE, bbox.right_bottom.y/LOCATION_GRID_SIZE};
        //std::cout << "calculate down " << down.x << "," << down.y << std::endl;
        int width=bbox.right_bottom.x/LOCATION_GRID_SIZE-down.x+1;

        for(int i=down.x; i<(down.x+width); i++){
            if(
                locations[current_location]->isTile(i, down.y, TILE_WALL) ||
                locations[current_location]->isTile(i, down.y, TILE_STAIRS)
            ){
                return true;
            }
           // std::cout << "check: " << i << "," << down.y << std::endl;
        }
        //std::cout << "---false---" << std::endl;
        return false;
    }

    MyOGL::Vector2i getFoundInsideBlock(){
        return m_last_found_block;
    }

    bool isInsideBlock(int bloockType){

        BBox bbox=this->getBBox();

        MyOGL::Vector2i left_top={bbox.left_top.x/LOCATION_GRID_SIZE, bbox.left_top.y/LOCATION_GRID_SIZE};
        MyOGL::Vector2i right_bottom={bbox.right_bottom.x/LOCATION_GRID_SIZE+1, bbox.right_bottom.y/LOCATION_GRID_SIZE};
        // check inside block
        for(int y=left_top.y; y < right_bottom.y; y++){
            for(int x=left_top.x; x<right_bottom.x; x++){
                if(locations[current_location]->isTile(x, y, bloockType)){
                    m_last_found_block={x, y};
                    return true;
                }
            }
        }

        return false;
    }

    void hitToHero(int hero_index);

};

class CHero:public CBaseHero{
    public:
        void Update(double DeltaTime);

        void debugActionAndButtons(){
            std::cout << "orient=" << m_orientation <<
            " action " << this->m_action <<
            " right: " << App->IsKeyPressed(SDLK_RIGHT) <<
            " left: " << App->IsKeyPressed(SDLK_LEFT) <<
            " up: " << App->IsKeyPressed(SDLK_UP) <<
            " down: " << App->IsKeyPressed(SDLK_DOWN) <<
            " ctrl: " << App->IsKeyPressed(SDLK_LCTRL) <<
            std::endl;
        }

        void goSpawnPoint(){
            this->m_enabled=true;
            m_position=locations[current_location]->getHeroSpawn();
            this->setAction(HERO_ACTION_STAY);
            this->m_health=this->m_max_health;
        }
};

class CEnemy:public CBaseHero{

        double m_spawn_time;
        bool m_wait_spawn;
        CArtificialIntelligence *ai;
        int health;

    public:

        CEnemy(int owner_index=0):CBaseHero(){
            ai=new CArtificialIntelligence();
            m_spawn_time=0;
            m_wait_spawn=false;
            m_owner_index=owner_index;
            ai->setTargetIndex(hero_index);
        }

        void setIndex(int owner_index){
            m_owner_index=owner_index;
            ai->setOwnerIndex(owner_index);
        }

        void setScaleFactor(int scale_factor){
            m_scale_factor=scale_factor;
            ai->setScaleFactor(m_scale_factor);
        }

        void setSpawnTime(double time){
            m_spawn_time=time;
            if(time > 0){
                m_wait_spawn=true;
            }
        }

        void goSpawnPoint(double spawn_time=SPAWN_TIME_DIE){
            this->m_enabled=true;
            this->m_visible=false;
            this->setSpawnTime(spawn_time);
            m_position=locations[current_location]->getEnemySpawn();
            this->setAction(HERO_ACTION_STAY);
            this->health=this->m_max_health;
            ai->reset();
        }

        void Update(double DeltaTime);

        void debugActionAndButtons(){
            std::cout << m_name << " orient=" << m_orientation <<
            " action " << this->m_action <<
            " spawn_time " << this->m_spawn_time <<
            " position " << this->m_position.x << "," << m_position.y <<
            std::endl;
        }
};

extern std::vector<CBaseHero*> heroes;
extern void initHeroObjects(int scale_factor, int hero_index);

#endif // HERO_H_INCLUDED
