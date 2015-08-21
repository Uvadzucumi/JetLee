#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include "render.h"

namespace MyOGL{

    class CShader{
            GLhandleARB m_vs, m_fs, m_program;
            std::vector<char> m_vs_src, m_fs_src;

// load file to vector buffer
            bool loadFile(std::vector<char>& buffer, const std::string& filename){
                std::ifstream file(filename.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

            //get filesize
                std::streamsize size = 0;
                if(file.seekg(0, std::ios::end).good()) size = file.tellg();
                if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

            //read contents of the file into the vector
                if(size > 0){
                    buffer.resize((size_t)size);
                    file.read((char*)(&buffer[0]), size);
                    return true;
                }
                else buffer.clear();
                return false;
            }

        public:

            bool LoadShaders(const char *VertexShaderFileName, const char *FragmentShaderFileName){
                // load program sources
                bool result=true;
                if(!loadFile(m_vs_src, VertexShaderFileName)){
                    Log->printf("Not loaded VertexShader: %s\n", VertexShaderFileName);
                    result=false;
                };
                if(!loadFile(m_fs_src, FragmentShaderFileName)){
                    Log->printf("Not loaded FragmentShader: %s\n", FragmentShaderFileName);
                    result=false;
                }
                return result;
            };

            ~CShader(){
                // clear programm sources
                m_vs_src.clear();
                m_fs_src.clear();
            };

            // check trie for operayin Param, for handle Handle
            const bool IsGLSLError(GLhandleARB Handle, GLenum Param){
                GLint glsl_ok, length;
                char infoLog[1000];

                MyOGL::Render->CheckError();
                glGetObjectParameterivARB(Handle, Param, &glsl_ok); // Check operation result
                // get last operation log
                glGetInfoLogARB(Handle, sizeof(infoLog), &length, (GLcharARB *)infoLog);
                if(glsl_ok!=GL_TRUE){
                    Log->printf("glSLang error: %s\n", infoLog);
                    return true;
                }
                return false;
            };

            void Create(){
                GLint len[1];
                const GLcharARB *src[1];

                // Create Shader programm
                m_program = glCreateProgramObjectARB();
                if(!m_program){
                    MyOGL::Log->puts("Error create ShaderProgram\n");
                }else{
                    MyOGL::Log->printf("Create Shader program: %d\n",m_program);
                }
                // Create Vertex shader
                m_vs = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
                if(MyOGL::Render->CheckError()){
                    MyOGL::Log->puts("Error create VertexShader\n");
                }
                // Create Fragment Shader
                m_fs = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
                if(MyOGL::Render->CheckError()){
                    MyOGL::Log->puts("Error create FragmentShader\n");
                };
                // add vs, fs sources
                len[0]=m_vs_src.size();
                src[0]=&m_vs_src[0];
                glShaderSourceARB(m_vs, 1, src, len);
                if(MyOGL::Render->CheckError()){
                    MyOGL::Log->puts("Error: set source for VertexShader\n");
                }

                len[0]=m_fs_src.size();
                src[0]=&m_fs_src[0];
                glShaderSourceARB(m_fs, 1, src, len);
                if(MyOGL::Render->CheckError()){
                    MyOGL::Log->puts("Error: set source for FragmentShader\n");
                }
                // compile shaders
                glCompileShaderARB(m_vs);
                if(IsGLSLError(m_vs, GL_OBJECT_COMPILE_STATUS_ARB)){
                    MyOGL::Log->puts("Vertex Shader Compilation error\n");
                }
                glCompileShaderARB(m_fs);
                if(IsGLSLError(m_fs, GL_OBJECT_COMPILE_STATUS_ARB)){
                    MyOGL::Log->puts("Fragment Shader Compilation error\n");
                }
                // attach shaders to shader program
                glAttachObjectARB(m_program, m_vs);
                glAttachObjectARB(m_program, m_fs);
                // link programs (varying from vs link to varying fs)
                glLinkProgramARB(m_program);
                if(IsGLSLError(m_program, GL_OBJECT_LINK_STATUS_ARB)){
                    Log->puts("Shaders linking Error\n");
                }
                //glValidateProgramARB(m_program);
                //IsGLSLError(m_program, GL_OBJECT_VALIDATE_STATUS_ARB);
            };

            void Bind(){
                glUseProgramObjectARB(m_program);
            };
            void UnBind(){
                glUseProgramObjectARB(0);
            };
// Get Uniform variable ID
            GLint GetUniformId(const char *VarName){
                return glGetUniformLocationARB (m_program, (const GLcharARB *)VarName);
            };
// Set uniform variable value by id
            void SetUniform1f(GLuint UniformId, GLfloat Value){
                glUniform1fARB(UniformId, Value);
            };
            void SetUniform1i(GLuint UniformId, GLint Value){
                glUniform1iARB(UniformId, Value);
            };

    };

}

#endif // SHADERS_H_INCLUDED
