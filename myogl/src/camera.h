#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "../src/vector_types.h"
#include "../src/render.h"

#include <math.h>



namespace MyOGL{

    class CCamera;

    class CCamera{
            Vector3f m_position;
            Vector3f m_view;
            Vector3f m_up;
            Vector3f m_strafe;

            Vector3f direction;

        public:
            CCamera(){
                m_position=Vector3f(0,0,0);
                m_view=Vector3f(0,0,-1);
                m_up=Vector3f(0,1,0);
            };

        void set(float positionX, float positionY, float positionZ,
                 float viewX, float viewY, float viewZ,
                 float upVectorX, float upVectorY, float upVectorZ);

            void Apply();
            void setPosition(float x, float y, float z){
                m_position.set(x,y,z);
                Update();
            }
            void setView(float x, float y, float z){
                m_view.set(x,y,z);
                Update();
            }
            void setUp(float x, float y, float z){
                m_up.set(x,y,z);
                Update();
            }

            void Move(float speed);
            void Strafe(float speed);
            void RotateView(float angle, float x, float y, float z);
            void RotateAroundPoint(Vector3f vCenter, float angle, float x, float y, float z);
            void Update(); // calculate left vector

            void MouseRotate(int dx, int dy);

            Vector3f getPosition(void){ return m_position;}
            Vector3f getView(void){ return m_position;}

            void Debug();
    };

}


#endif // CAMERA_H_INCLUDED
