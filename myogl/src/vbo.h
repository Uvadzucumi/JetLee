#ifndef VBO_H_INCLUDED
#define VBO_H_INCLUDED

#include <vector>
//#include <initializer_list>
#include "vector_types.h"
#include "render.h"

// Vbo class template
namespace MyOGL{

    template <class Type>
        class CVBOBuffer{
        protected:
            GLuint m_BufferId;
            GLenum m_Usage, m_Target;
            std::vector <Type> m_Buffer;
        public:
            CVBOBuffer(GLenum Usage=GL_STATIC_DRAW, GLenum Target=GL_ARRAY_BUFFER){
                m_Usage=Usage;
                m_Target=Target;
            }
            virtual ~CVBOBuffer(){
                Log->puts("Clear Buffer memery\n");
                if(m_BufferId) glDeleteBuffersARB(1,&m_BufferId);
                m_Buffer.clear();
            };
            void Assign(const Type *data, unsigned int size){
                m_Buffer.clear();
                for(unsigned int i=0; i < size; i++){
                    m_Buffer.push_back(data[i]);
                }
                Log->printf("CVBOBuffer::Assign(const *data). size: %d\n",  m_Buffer.size());
            };
            void Assign(const std::vector <Type> &Buffer){
                m_Buffer.clear();
                m_Buffer=Buffer;
                Log->printf("CVBOBuffer::Assign(const std::vector &). size: %d\n",  m_Buffer.size());
            };
            void Bind(){
                glBindBufferARB(m_Target, m_BufferId );
            };
            void UnBind(){
                glBindBufferARB(m_Target,0);
            };
            void BuildVBO(){
                glGenBuffersARB( 1, &m_BufferId );
                this->Bind();
                glBufferDataARB(m_Target, sizeof(Type)*m_Buffer.size(), &m_Buffer[0], m_Usage);
                this->UnBind();
                Log->printf("Created VBO Buffer: %d\n",m_BufferId);
            };
            void SubData(const std::vector <Type> &SubData, int StartPos=0){
                glBufferSubDataARB(m_Target, sizeof(Type)*StartPos, sizeof(Type)*m_Buffer.size(), &SubData[0]);
            };

            void * Map( GLenum access ){
                return glMapBufferARB(m_Target, access);
            };

            bool UnMap(){
                return glUnmapBufferARB(m_Target) == GL_TRUE;
            };
            virtual void Enable(){};
            virtual void Disable(){};
    };

// buffer types
    typedef CVBOBuffer<GLfloat> CVBOBufferFloat;
    typedef CVBOBuffer<GLubyte> CVBOBufferByte;

// buffer classes realization
    class CVBOBufferVerticles3f:public CVBOBufferFloat{
        public:
            CVBOBufferVerticles3f(GLenum Usage=GL_STATIC_DRAW, GLenum Target=GL_ARRAY_BUFFER);
            ~CVBOBufferVerticles3f();
            void Enable();
            void Disable();
            void DrawArray(GLenum mode, int first=0, int count=0);
    };

    class CVBOBufferTextCoords2f:public CVBOBufferFloat{
        public:
            CVBOBufferTextCoords2f(GLenum Usage=GL_STATIC_DRAW, GLenum Target=GL_ARRAY_BUFFER);
            ~CVBOBufferTextCoords2f();
            void Enable();
            void Disable();
    };

    class CVBOBufferNormals3f: public CVBOBufferFloat{
        public:
            CVBOBufferNormals3f(GLenum Usage=GL_STATIC_DRAW, GLenum Target=GL_ARRAY_BUFFER);
            ~CVBOBufferNormals3f();
            void Enable();
            void Disable();
    };

    class CVBOBufferIndices1b:public CVBOBufferByte{
        public:
            CVBOBufferIndices1b(GLenum Usage=GL_STATIC_DRAW, GLenum Target=GL_ARRAY_BUFFER);
            ~CVBOBufferIndices1b();
            void Draw(GLenum mode=GL_TRIANGLES, int index_count=0, const void *start_element=NULL);
    };

    // VertexArrayObject class
    class CVAO{
            std::vector<CVBOBufferFloat*> m_VBOBuffers;
            GLuint m_VAO;
        public:
            void AttachFloatBuffer(CVBOBufferFloat *Buffer){
                m_VBOBuffers.push_back(Buffer);
            }

            void BuildVAO(){
                Create();
                Bind();
                for(unsigned int i=0;i<m_VBOBuffers.size();i++){
                    m_VBOBuffers[i]->Enable();
                }
                UnBind();
            };

            CVAO();
            ~CVAO();
            void Create(){ if(m_VAO){ glDeleteVertexArrays(1, &m_VAO); }; glGenVertexArrays(1, &m_VAO); };
            void Bind(){ glBindVertexArray(m_VAO); };
            void UnBind(){ glBindVertexArray(0); };
    };

    extern std::vector<CVBOBufferFloat*> VBOFloatBuffersList;
    extern std::vector<CVBOBufferByte*> VBOByteBuffersList;
    extern std::vector<CVAO*> VAOList;
}
#endif // VBO_H_INCLUDED
