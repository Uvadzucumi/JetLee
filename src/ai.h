#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "locations.h"

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

        int m_owner_id;
        int m_target_id;


    public:

        void setOwnerIndex(int index){
            m_owner_id=index;
        }

        void setTargetIndex(int index){
            m_target_id=index;
        }

        void setScaleFactor(int factor){
            m_scale_factor=factor;
        }

        CArtificialIntelligence(){
            m_task=AI_ACTION_STAY;
            //m_scale_factor=1;
            m_grid_size=4;
        }
        // x, y - pixel coords
        int getFloorByYCoord(int y){
            int grid_coord=y/m_grid_size;
            if(grid_coord<=16){  // 12
                return FLOOR_UPPER;
            }
            if(grid_coord<=25){  // 21
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

        void setTask(int task);

        // only new task
        void startSelectTask();

        bool isTargetInCurrentFloor();

        void update(double DeltaTime);

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

extern int difficulty_level;

#endif // AI_H_INCLUDED
