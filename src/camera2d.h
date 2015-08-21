#ifndef CAMERA2D_H_INCLUDED
#define CAMERA2D_H_INCLUDED

#include "../myogl/src/vector_types.h"

class CCamera2d{

    MyOGL::Vector2i map_left_top, map_right_bottom;

    MyOGL::Vector2f position;
    MyOGL::Vector2f viewport;

    public:
        CCamera2d(){
            position={0,0};
            viewport={64,64};
            map_left_top={0,0};
            map_right_bottom={168, 124};
        }

        // move camera to better position for display x, y map point
        // 2/3 by y, 1/2 by x of Viewport
        void moveTo(float x, float y){
            position.x=x;
            position.y=y;
            // check map dimension
            if(map_left_top.x > position.x){
                position.x=map_left_top.x;
            }
            if(map_left_top.y > position.y){
                position.y=map_left_top.y;
            }
            if((position.x+viewport.x) > (float)map_right_bottom.x){
                position.x=map_right_bottom.x-viewport.x;
            }
            if((position.y+viewport.y) > (float)map_right_bottom.y){
                position.y=map_right_bottom.y-viewport.y;
            }
        }

        MyOGL::Vector2f getPosion(){
            return position;
        }

        void moveBetterViewPositionFor(float x, float y){
            float dy=viewport.y/3*2;
            float dx=viewport.x/2;
            moveTo(x-dx, y-dy);
        }

        void moveX(float dx){
            moveTo(position.x+dx, position.y);
        }

        void moveY(float dy){
            moveTo(position.x, position.y+dy);
        }

};

#endif // CAMERA2D_H_INCLUDED
