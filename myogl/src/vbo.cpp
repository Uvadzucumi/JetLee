
#include "vbo.h"

using namespace MyOGL;

std::vector<CVBOBufferFloat*> MyOGL::VBOFloatBuffersList;
std::vector<CVBOBufferByte*> MyOGL::VBOByteBuffersList;
std::vector<CVAO*> MyOGL::VAOList;

CVBOBufferVerticles3f::CVBOBufferVerticles3f(GLenum Usage, GLenum Target){
    m_Usage=Usage;
    m_Target=Target;
    VBOFloatBuffersList.push_back(this);
}

CVBOBufferVerticles3f::~CVBOBufferVerticles3f(){
    // remove VBO buffer from list
    for(unsigned int i=0;i<VBOFloatBuffersList.size();i++){
        if(VBOFloatBuffersList[i]==this){
            VBOFloatBuffersList.erase(VBOFloatBuffersList.begin()+i);
            break;
        }
    }
    Log->puts("Deleted VBO object (Verticles3f)\n");
}

// Vertex buffer
void CVBOBufferVerticles3f::Enable(){
    glEnableClientState( GL_VERTEX_ARRAY );
    Bind();
    glVertexPointer( 3, GL_FLOAT, 0, NULL );
}

void CVBOBufferVerticles3f::Disable(){
    glDisableClientState( GL_VERTEX_ARRAY );
}

void CVBOBufferVerticles3f::DrawArray(GLenum mode, int first, int count){
    if(!count){ count=m_Buffer.size(); }
    glDrawArrays(mode , first, count);
}

// Text Coords Buffer
CVBOBufferTextCoords2f::CVBOBufferTextCoords2f(GLenum Usage, GLenum Target){
    m_Usage=Usage;
    m_Target=Target;
    VBOFloatBuffersList.push_back(this);
}

CVBOBufferTextCoords2f::~CVBOBufferTextCoords2f(){
    // remove VBO buffer from list
    for(unsigned int i=0;i<VBOFloatBuffersList.size();i++){
        if(VBOFloatBuffersList[i]==this){
            VBOFloatBuffersList.erase(VBOFloatBuffersList.begin()+i);
            break;
        }
    }
    Log->puts("Deleted VBO object (TextCoords2f)\n");
}

void CVBOBufferTextCoords2f::Enable(){
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    Bind();
    glTexCoordPointer(2, GL_FLOAT, 0, NULL );
}

void CVBOBufferTextCoords2f::Disable(){
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

// Normals Buffer
CVBOBufferNormals3f::CVBOBufferNormals3f(GLenum Usage, GLenum Target){
    m_Usage=Usage;
    m_Target=Target;
    VBOFloatBuffersList.push_back(this);
}

CVBOBufferNormals3f::~CVBOBufferNormals3f(){
    // remove VBO buffer from list
    for(unsigned int i=0;i<VBOFloatBuffersList.size();i++){
        if(VBOFloatBuffersList[i]==this){
            VBOFloatBuffersList.erase(VBOFloatBuffersList.begin()+i);
            break;
        }
    }
    Log->puts("Deleted VBO object (Normals3f)\n");
}

void CVBOBufferNormals3f::Enable(){
    glEnableClientState( GL_NORMAL_ARRAY );
    Bind();
    glNormalPointer(GL_FLOAT, 0, NULL );
}

void CVBOBufferNormals3f::Disable(){
    glDisableClientState( GL_NORMAL_ARRAY );
}

// Index Buffer
CVBOBufferIndices1b::CVBOBufferIndices1b(GLenum Usage, GLenum Target){
    m_Usage=Usage;
    m_Target=Target;
    VBOByteBuffersList.push_back(this);
}

CVBOBufferIndices1b::~CVBOBufferIndices1b(){
    // remove VBO buffer from list
    for(unsigned int i=0;i<VBOByteBuffersList.size();i++){
        if(VBOByteBuffersList[i]==this){
            VBOByteBuffersList.erase(VBOByteBuffersList.begin()+i);
            break;
        }
    }
    Log->puts("Deleted VBO object (Indices1b)\n");
}

void CVBOBufferIndices1b::Draw(GLenum mode, int index_count, const void *start_element){
    if(!index_count) index_count=m_Buffer.size();
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
    glDrawElements(mode, index_count, GL_UNSIGNED_BYTE, start_element);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
}

CVAO::CVAO(){
    m_VAO=0;
    MyOGL::VAOList.push_back(this);
}

CVAO::~CVAO(){
    if(m_VAO){
        glDeleteVertexArrays(1, &m_VAO);
    }
    // remove from VAO list
    for(unsigned int i=0;i<VAOList.size();i++){
        if(VAOList[i]==this){
            VAOList.erase(VAOList.begin()+i);
            break;
        }
    }
    m_VBOBuffers.clear();
    Log->puts("Deleted VAO object\n");
}
