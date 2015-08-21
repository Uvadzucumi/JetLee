#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "locations.h"
#include "hero.h"

enum {
    AI_ACTION_STAY=0,
    AI_ACTION_MOVE_TO_FLOR_POINT,
    AI_MOVE_TO_STAIRS_POINT,
    AI_MOVE_TO_HERO_POINT,
    AI_GO_UP_FLOOR
};

enum {
    FLOOR_GROUND,
    FLOOR_MIDDLE,
    FLOOR_UPPER
};

class CArtificialIntelligence{

        int m_scale_factor;
        int m_grid_size;

        double m_wait_time;
        int m_task;
        bool m_ready_action;
        bool m_enable;

        // tasks data
        MyOGL::Vector2i m_point;

    public:

        void setScaleFactor(int factor){
            m_scale_factor=factor;
        }

        CArtificialIntelligence(){
            m_task=AI_ACTION_STAY;
            m_scale_factor=1;
            m_grid_size=4;
        }
        // x, y - pixel coords
        int getFloorByYCoord(int y){
            int grid_coord=y/m_grid_size;
            if(grid_coord<=12){
                return FLOOR_UPPER;
            }
            if(grid_coord<=21){
                return FLOOR_MIDDLE;
            }
            return FLOOR_GROUND;
        }

        int getXStairCoord(){
            return 0;
        }

        int getXDownWay(int x, int y){
            return 0;
        }

        bool isGoPoint(){
            if(isTaskReady() && m_task==AI_ACTION_MOVE_TO_FLOR_POINT){
                return true;
            }
            return false;
        }

        int getTask(){
            return m_task;
        }

        void setTask(int task){
            m_ready_action=false;
            m_wait_time=1.0;
            switch(task){
                case AI_ACTION_MOVE_TO_FLOR_POINT:
                    // get random point in current floor
                    m_point.x=(rand() % (LOCATION_GRID_WIDTH-4)) * m_grid_size + 2*m_grid_size;
                    break;
                default:
                    std::cout << "Unknown Task for AI::setTask()" << std::endl;

            };
            m_task=task;
        }

        // only new task
        void startSelectTask(){


            setTask(AI_ACTION_MOVE_TO_FLOR_POINT);
        }

        void update(double DeltaTime){
            if(m_enable){
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

        void reset(){
            m_task=AI_ACTION_STAY;
            m_enable=false;
        }

        void setEnable(bool enable){
            m_enable=enable;
        }

        bool isTaskReady(){
            return m_ready_action;
        }

        MyOGL::Vector2i getPoint(){
            return m_point;
        }

};

#endif // AI_H_INCLUDED
