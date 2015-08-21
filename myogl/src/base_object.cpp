
#include "base_object.h"

using namespace MyOGL;

CBaseObject::CBaseObject(){
    m_Position.set(0,0,0);
    m_Rotation.set(0,0,0);
}

void CBaseObject::SetPosition(float x, float y, float z){
    m_Position.set(x,y,z);
}

void CBaseObject::SetRotation(float x, float y, float z){
    m_Rotation.set(x,y,z);
}

void CBaseObject::ApplyTransformations(){
    glTranslatef(-m_Position.x,-m_Position.y,-m_Position.z);

    glRotatef(m_Rotation.x,1.0f,0.0f,0.0f);                     // Rotate On The X Axis
    glRotatef(m_Rotation.y,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis
    glRotatef(m_Rotation.z,0.0f,0.0f,1.0f);                     // Rotate On The Z Axis
}

void CBaseObject::Draw(){};

void CBaseObject::Render(){
    glPushMatrix();
    ApplyTransformations();
    Draw();
    glPopMatrix();
};
