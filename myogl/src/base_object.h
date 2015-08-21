#ifndef BASE_OBJECT_H_INCLUDED
#define BASE_OBJECT_H_INCLUDED

#include "vector_types.h"
#include "render.h"

namespace MyOGL{

    class CBaseObject{
            MyOGL::Vector3f m_Position;
            MyOGL::Vector3f m_Rotation;
        public:
            CBaseObject();
            void SetPosition(float x, float y, float z);
            void SetRotation(float x, float y, float z);
            void ApplyTransformations();
            virtual void Draw();
            void Render();
        };
}

#endif // BASE_OBJECT_H_INCLUDED
