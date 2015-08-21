#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "vector_types.h"

namespace MyOGL{

    class CLight{
            Vector4f m_Ambient;
            Vector4f m_Diffuse;
            Vector4f m_Position;
            unsigned m_LightNumber;
        public:
            CLight(unsigned light_no=GL_LIGHT0){ m_LightNumber=light_no;};
            ~CLight(){};
            void SetAmbient(float r, float g, float b, float a){
                m_Ambient.Set(r,g,b,a);
            }
            void SetAmbient(float* color){
                m_Ambient.Set(color[0],color[1],color[2],color[3]);
            }
            void SetDiffuse(float r, float g, float b, float a){
                m_Diffuse.Set(r,g,b,a);
            }
            void SetDiffuse(float* color){
                m_Diffuse.Set(color[0],color[1],color[2],color[3]);
            }
            void SetPosition(float x, float y, float z, float w){
                m_Position.Set(x,y,z,w);
            }
            void SetPosition(float *pos){
                m_Position.Set(pos[0],pos[1],pos[2],pos[3]);
            }
            void Enable(){ GL.Enable(m_LightNumber); }; // Enable Light
            void Disbale(){ GL.Disable(m_LightNumber); };
            void Apply(){
                glLightfv(m_LightNumber, GL_AMBIENT, m_Ambient.data);             // Setup The Ambient Light
                glLightfv(m_LightNumber, GL_DIFFUSE, m_Diffuse.data);             // Setup The Diffuse Light
                glLightfv(m_LightNumber, GL_POSITION, m_Position.data);            // Position The Light
                Enable();
            }
            void Debug(){
                Log->printf("light: %x\n",m_LightNumber);
                Log->puts("Ambient: %f %f %f %f\n", &m_Ambient);
                Log->puts("Diffuse: %f %f %f %f\n", &m_Diffuse);
                Log->puts("Position: %f %f %f %f\n", &m_Position);
            }
    };

}

#endif // LIGHT_H_INCLUDED
