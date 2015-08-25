
#include "ai.h"
#include "hero.h"

int difficulty_level;

void CArtificialIntelligence::setTask(int task){
    m_ready_action=false;
    m_wait_time=1.0;
    switch(task){
        case AI_ACTION_MOVE_TO_FLOR_POINT:
                    // get random point in current floor
            m_point.x=(rand() % (LOCATION_GRID_WIDTH-4)) * m_grid_size + 2*m_grid_size;
            break;
        case AI_MOVE_TO_HERO_POINT:
            m_point.x=heroes[m_target_id]->getPosition().x;
            break;
        default:
            logW("Unknown Task for AI::setTask()");

    };
    m_task=task;
}

void  CArtificialIntelligence::startSelectTask(){

    int action_to_fight=rand()%100;

    if(action_to_fight <= difficulty_level){

        if(this->isTargetInCurrentFloor()){
            log("[" << heroes[m_owner_id]->getName() << "] GO TO HERO POINT");
            setTask(AI_MOVE_TO_HERO_POINT);
            return;
        }else{
            log("[" << heroes[m_owner_id]->getName() << "] DIFFERENT FLOOR");
        }

    }else{
        log("[" << heroes[m_owner_id]->getName() << "] RANDOM FOR STYPID MOVEMENT: " << action_to_fight << " > " << difficulty_level);
    }

    setTask(AI_ACTION_MOVE_TO_FLOR_POINT);
}

bool CArtificialIntelligence::isTargetInCurrentFloor(){
    MyOGL::Vector2i enemy_pos = heroes[m_target_id]->getPosition();
    MyOGL::Vector2i owner_pos = heroes[m_owner_id]->getPosition();
    int enemy_floor=this->getFloorByYCoord(enemy_pos.y);
    int owner_floor=this->getFloorByYCoord(owner_pos.y);
    //log("[" << heroes[m_owner_id]->getName() << "] floor=" << owner_floor << " target_floor=" << enemy_floor);
    return enemy_floor==owner_floor;
}

void CArtificialIntelligence::update(double DeltaTime){
    if(m_enable){

        if(m_task==AI_MOVE_TO_HERO_POINT){
            if(this->isTargetInCurrentFloor()){
                m_point=heroes[m_target_id]->getPosition();
            }
        }

        if(m_task==AI_ACTION_STAY){
            this->startSelectTask();
            return;
        }

        if(m_wait_time>0){
            m_wait_time-=DeltaTime;
        }else{
            //
            m_wait_time=0;
            m_ready_action=true;
        }
        //std::cout << "AI: wait: " << m_wait_time << " task:" << m_task << " point: " << m_point.x << " task_ready: " << isTaskReady() << std::endl;
    }
}

