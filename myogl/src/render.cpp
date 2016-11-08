
#include "render.h"

#include "texture.h"
#include "ui/bitmap_font.h"
#include "vbo.h"

#include <math.h>

#ifdef __WIN32__
//PFNGLBLENDEQUATIONPROC              glBlendEquation         = NULL;
// texture units
//PFNGLACTIVETEXTUREPROC           glActiveTexture            = NULL;
#endif // __WIN32__

// FBO functions
PFNGLGENFRAMEBUFFERSEXTPROC         glGenFramebuffersEXT      = NULL;
PFNGLGENRENDERBUFFERSEXTPROC        glGenRenderbuffersEXT     = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC         glBindFramebufferEXT      = NULL;
PFNGLBINDRENDERBUFFEREXTPROC        glBindRenderbufferEXT     = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC     glRenderbufferStorageEXT  = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    glFramebufferTexture2DEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC          glGenerateMipmapEXT       = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC     glDeleteRenderbuffersEXT  = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC      glDeleteFramebuffersEXT   = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatusEXT = NULL;

// VBO functions
PFNGLGENBUFFERSARBPROC       glGenBuffersARB         = NULL;
PFNGLBINDBUFFERARBPROC       glBindBufferARB         = NULL;
PFNGLMAPBUFFERARBPROC        glMapBufferARB          = NULL;
PFNGLUNMAPBUFFERARBPROC      glUnmapBufferARB        = NULL;
PFNGLBUFFERDATAARBPROC       glBufferDataARB         = NULL;
PFNGLBUFFERSUBDATAARBPROC    glBufferSubDataARB      = NULL;
PFNGLDELETEBUFFERSARBPROC    glDeleteBuffersARB      = NULL;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB   = NULL;
// VAO
PFNGLBINDVERTEXARRAYPROC     glBindVertexArray      = NULL;
PFNGLDELETEVERTEXARRAYSPROC  glDeleteVertexArrays   = NULL;
PFNGLGENVERTEXARRAYSPROC     glGenVertexArrays      = NULL;
PFNGLISVERTEXARRAYPROC       glIsVertexArray        = NULL;


// Shaders
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB   = NULL;
PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB          = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB      = NULL;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB    = NULL;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB          = NULL;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB         = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB  = NULL;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB          = NULL;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB            = NULL;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB           = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB    = NULL;
PFNGLUNIFORM1FARBPROC            glUniform1fARB             = NULL;
PFNGLUNIFORM2FARBPROC            glUniform2fARB             = NULL;
PFNGLUNIFORM3FARBPROC            glUniform3fARB             = NULL;
PFNGLUNIFORM4FARBPROC            glUniform4fARB             = NULL;
PFNGLUNIFORM1FVARBPROC           glUniform1fvARB            = NULL;
PFNGLUNIFORM2FVARBPROC           glUniform2fvARB            = NULL;
PFNGLUNIFORM3FVARBPROC           glUniform3fvARB            = NULL;
PFNGLUNIFORM4FVARBPROC           glUniform4fvARB            = NULL;
PFNGLUNIFORM1IARBPROC            glUniform1iARB             = NULL;
PFNGLBINDATTRIBLOCATIONARBPROC   glBindAttribLocationARB    = NULL;
PFNGLGETACTIVEUNIFORMARBPROC     glGetActiveUniformARB      = NULL;
PFNGLGETSHADERIVPROC             glGetShaderiv              = NULL;
PFNGLGETPROGRAMIVPROC            glGetProgramiv             = NULL;

using namespace MyOGL;

// extern pointer to render class
CRender *MyOGL::Render=NULL;
RenderStates MyOGL::GL;

// constructor
CRender::CRender(){
    // Window parameters
    m_width=0;
    m_height=0;
    m_bpp=0;
    m_full_screen=false;
    // render context
    Context=NULL;
    Window=NULL;
    zNearPlane=0.1;
    zFarPlane=500;
}

// destructor
CRender::~CRender(){
    Free();
    SDL_Quit();
}

// clear all data
void CRender::Free(){
    if(!Context){
        SDL_GL_DeleteContext(Context);
        Context=NULL;
    }
    if(!Window){
        SDL_DestroyWindow(Window);
        Window=NULL;
    }
}

// set start OpenGL states
void CRender::InitGL(){

    glColor3f(1,1,1);
    glClearColor(0,0,0,1);
    glClearDepth(1.0f);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_BLEND);

    //glBlendEquation(GL_FUNC_ADD);

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    Set2D(true);    // Set2D projection
    GL.GetCurrentStates();
    GL.Debug();
}

// Render class realisation
bool CRender::Init(int width, int height, int bpp, bool full_screen, const char *title){
    this->m_width=width;
    this->m_height=height;
    this->m_bpp=bpp;
    this->m_full_screen=full_screen;

    if( SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log->printf("CRender::Init SDL_Init(): false \"%s\"\n",SDL_GetError());
        return false;
    }

    // Set OpenGL Context Version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 0);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Color Buffer
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, this->m_bpp);
    // Depth buffer 24bit
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    #ifdef MYOGL_DOUBLE_BUFFER
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    #endif
    window_flags = SDL_WINDOW_OPENGL;
    if(this->m_full_screen){
        window_flags |= SDL_WINDOW_FULLSCREEN;
    }else{
    #ifdef MYOGL_RESIZABLE_WINDOW
        window_flags |= SDL_WINDOW_RESIZABLE;
    #endif
    }
    // create the sdl2 window
    Window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, this->m_width, this->m_height,
            window_flags);
    // create the opengl context
    Context = SDL_GL_CreateContext(Window);
    #ifdef MYOGL_VSYNC
        SDL_GL_SetSwapInterval(1); // Enable VSync
    #else
        SDL_GL_SetSwapInterval(0); // Discable VSync
    #endif

    int tmp,tmp2;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &tmp);
    printf("Double Buffering: %s\n",(tmp)?"Enabled":"Disabled");
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &tmp);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &tmp2);
    printf("OpenGL Version: %d.%d\n",tmp,tmp2);
    // show Opengl Info
    this->gl_vendor = (char *) glGetString(GL_VENDOR);
    Log->printf("OpenGL Vendor: %s\n",gl_vendor);
    gl_version = (char *) glGetString(GL_VERSION);
    Log->printf("OpenGL Version: %s\n",gl_version);

    if(!strcmp(gl_vendor,"Microsoft Corporation")){ // 0 - HACK for Windows :(
       Log->puts("Need install Video Driver!\n");
        return false;
    }else{

        gl_extensions = (char *) glGetString(GL_EXTENSIONS);

        Log->puts("OpenGL Extensions:\n");
        for(int i=0;gl_extensions[i]!=0;i++){
            if(gl_extensions[i]==32) { gl_extensions[i]=10; }
        }
        Log->puts(gl_extensions);

    }
#ifdef __WIN32__
//    glBlendEquation = (PFNGLBLENDEQUATIONPROC)this->GetProcAddress("glBlendEquation");
    // enable another
//    glActiveTexture = (PFNGLACTIVETEXTUREPROC)    GetProcAddress("glActiveTexture");
#endif // __WIN32__
    // if supported
    m_vbo=EnableVBOFunctions();
    m_vao=EnableVAOFunctions();
    m_shaders=EnableShadersFunctions();
    m_fbo=EnableFBOFunctions();

    InitGL();   // start opengl states

    return true;
}

// Set Orthogonal projection
// width, height - need for FBO
void CRender::Set2D(bool force, int width, int height){
    int new_w, new_h;
    if(width!=0 && height!=0){
        force=true;
        new_w=width;
        new_h=height;
    }else{
        new_w=this->m_width;
        new_h=this->m_height;
    }
    if(force || GL.mode3d){
        glViewport (0, 0, new_w, new_h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //void glOrtho(GLdouble  left,  GLdouble  right,  GLdouble  bottom,  GLdouble  top,  GLdouble  nearVal,  GLdouble  farVal);
        glOrtho(0, new_w, new_h, 0, 1, -1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        GL.mode3d=false;
        GL.Disable(GL_DEPTH_TEST);
        //Log->printf("Set 2D projection (%d,%d)\n",this->m_width,this->m_height);
    }
}

// Set Orthogonal projection
void CRender::Set3D(bool force){
    if(force || !GL.mode3d){
        glViewport (0, 0, this->m_width, this->m_height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective( 45.0f, (GLfloat)this->m_width/(GLfloat)this->m_height, 0.1f, 100.0f );
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        GL.mode3d=true;
        GL.Enable(GL_DEPTH_TEST);
        //Log->puts("Set 3D projection\n");
    }
}

// Clear ViewPort
void CRender::ClearScreen(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}
// Swap render buffers
void CRender::SwapBuffers(){
    SDL_GL_SwapWindow(Window);
}

// Window functions
void CRender::SetWinCaption(const char *title){
   SDL_SetWindowTitle(Window, title);
}

bool CRender::SetWinIcon(const char *file_name){
    SDL_Surface* icon;
//Load Bitmap
    if(!(icon = SDL_LoadBMP(file_name))) {
        Log->printf("Error load icon file name: %s\n",file_name);
        return false;
    }
    SDL_SetWindowIcon(Window, icon);
    SDL_FreeSurface(icon);
    return true;
}

void CRender::BindTexture(GLuint TextureID, bool force){
    if(GL.CurrentTexure!=TextureID || force){
	// Bind the texture object
        glBindTexture( GL_TEXTURE_2D, TextureID );
        GL.CurrentTexure=TextureID;
    }
}

void CRender::TextureOperation(MyEnum operation){
    // texture mixing
    // Replace
	// Add
	// MYOGL_TEXTURE_COMBINE_MODULATE - Modulate
	// Interpolate

    switch(operation){
        case MYOGL_TEXTURE_COMBINE_MODULATE:
            glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            break;
        default:
            break;
    }
}

void CRender::SetBlendMode(MyGlBlendMode mode){
    if(GL.CurrentBlendMode==mode) return;   // no need change
    // set new blend mode
    switch(mode){
        case blNone:
            GL.Disable(GL_BLEND);
            break;
        case blSource:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            GL.Enable(GL_BLEND);
            break;
        case blConstColor:
            glBlendFunc(GL_CONSTANT_COLOR, GL_DST_COLOR);
            GL.Enable(GL_BLEND);
            break;
        case blAdditive:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            GL.Enable(GL_BLEND);
            break;
        default:
            //printf("warnind: unknown blend mode %d\n",mode);
            Log->printf("warnind: unknown blend mode %x\n",mode);
    }
    GL.CurrentBlendMode=mode;
}

// Resize Application Windows
bool CRender::OnResize(int window_id, int width, int height){
    this->m_width=width;
    this->m_height=height;
/*
// SDL 1.2 Recreate Context Resources (for Windows)
    Log->puts("for Windows need reinicialize OGL states and recreate texture!\n");
    InitGL();
    // recreate textures
    for(unsigned int i=0;i<TexturesList.size();i++){
        // free old from video memory
        //GLuint texture_id=TexturesList[i]->GetID();
        //glDeleteTextures(1,&texture_id);
        // create new in video memory
        TexturesList[i]->CreateFromMemory();
        Log->printf("texture %d recreated. ", TexturesList[i]->GetID());
        Log->printf("file: %s\n", TexturesList[i]->GetFileName());
    }
    // recreate text display lists
    for(unsigned int i=0;i<TextsList.size();i++){
        TextsList[i]->CreateDisplayList();
        Log->puts("CText object display list recreated\n");
    }
    // recreate vbo buffers
    for(unsigned int i=0;i<VBOFloatBuffersList.size();i++){
        VBOFloatBuffersList[i]->BuildVBO();
        Log->puts("CVBO object recreated\n");
    }
    for(unsigned int i=0;i<VBOByteBuffersList.size();i++){
        VBOByteBuffersList[i]->BuildVBO();
        Log->puts("CVBO object recreated\n");
    }
    for(unsigned int i=0;i<VAOList.size();i++){
        VAOList[i]->BuildVAO();
        Log->puts("CVAO object recreated\n");
    }
    */
    glViewport(0,0,this->m_width,this->m_height);
        // set projection - force reset view port
    if(GL.mode3d){
        Set3D(true);
    }else{
        Set2D(true);
    }
    printf("window resized to %dx%d\n", width, height);
    return true;
}

// Set Inc color
void CRender::SetColor(float r, float g, float b, float a, bool force){
    if(GL.Color.r!=r || GL.Color.g!=g || GL.Color.b != b || GL.Color.a!=a || force){
        GL.Color.r=r; GL.Color.g=g; GL.Color.b=b; GL.Color.a=a;
        glColor4f(r,g,b,a);
    }
}
// Set Inc color
void CRender::SetColor(Vector4f color, bool force){
    if(GL.Color.r!=color.r || GL.Color.g!=color.g || GL.Color.b != color.b || GL.Color.a!=color.a || force){
        GL.Color.r=color.r; GL.Color.g=color.g; GL.Color.b=color.b; GL.Color.a=color.a;
        glColor4f(color.r, color.g, color.b, color.a);
    }
}
// Set Inc color
void CRender::SetColor(Vector3i color, bool force){
    this->SetColor(((float)color.r)/255,((float)color.g)/255,((float)color.b)/255,1.0, force);
}
// Set Inc color
void CRender::SetColor(int r, int g, int b, bool force){
    this->SetColor(((float)r)/255,((float)g)/255,((float)b)/255,1.0, force);
}


// Set Paper color
void CRender::SetClearColor(float r, float g, float b, float a){
    if(GL.ClearColor.r==r && GL.ClearColor.g==g && GL.ClearColor.b == b && GL.ClearColor.a==a){
        // nop
    }else{
        GL.ClearColor.r=r; GL.ClearColor.g=g; GL.ClearColor.b=b; GL.ClearColor.a=a;
        glClearColor(r,g,b,a);
    }
}

// Check OpenGL extension
bool CRender::isExtensionSupported ( const char * ext ){

    const char * extensions = (const char *)glGetString ( GL_EXTENSIONS );
    const char * start      = extensions;
    const char * ptr;

    while ( ( ptr = strstr ( start, ext ) ) != NULL )
    {
        // we've found, ensure name is exactly ext
        const char * end = ptr + strlen ( ext );

        if ( isspace ( *end ) || *end == '\0' )
            return true;
        start = end;
    }
    return false;
}

bool CRender::EnableFBOFunctions(){
    if ( isExtensionSupported ( "GL_ARB_framebuffer_object" ) ){

        glGenFramebuffersEXT  = (PFNGLGENFRAMEBUFFERSEXTPROC)  GetProcAddress("glGenFramebuffersEXT");
        glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) GetProcAddress("glGenRenderbuffersEXT");
        glBindFramebufferEXT  = (PFNGLBINDFRAMEBUFFEREXTPROC)  GetProcAddress("glBindFramebufferEXT");
        glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) GetProcAddress("glBindRenderbufferEXT");
        glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) GetProcAddress("glRenderbufferStorageEXT");
        glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) GetProcAddress("glFramebufferRenderbufferEXT");
        glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) GetProcAddress("glFramebufferTexture2DEXT");
        glGenerateMipmapEXT   = (PFNGLGENERATEMIPMAPEXTPROC)   GetProcAddress("glGenerateMipmapEXT");
        glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)        GetProcAddress("glDeleteRenderbuffersEXT");
        glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)          GetProcAddress("glDeleteFramebuffersEXT");
        glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)  GetProcAddress("glCheckFramebufferStatusEXT");

        // check functions addresses
        if(glGenFramebuffersEXT && glGenRenderbuffersEXT && glBindFramebufferEXT && glBindRenderbufferEXT &&
           glRenderbufferStorageEXT && glFramebufferRenderbufferEXT && glFramebufferTexture2DEXT && glGenerateMipmapEXT &&
           glDeleteRenderbuffersEXT && glDeleteFramebuffersEXT && glCheckFramebufferStatusEXT){

            Log->puts("FBO: OK\n");
            return true;

        }

    }
    Log->puts("FBO: none!\n");
    return false;
}

bool CRender::EnableVAOFunctions(){
    if( isExtensionSupported( "GL_ARB_vertex_array_object" )){
        glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)          GetProcAddress("glBindVertexArray");
        glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)    GetProcAddress("glDeleteVertexArrays");
        glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)          GetProcAddress("glGenVertexArrays");
        glIsVertexArray = (PFNGLISVERTEXARRAYPROC)              GetProcAddress("glIsVertexArray");
        if(glBindVertexArray && glDeleteVertexArrays && glGenVertexArrays && glIsVertexArray){
            Log->puts("VAO: OK\n");
            return true;
        }
    }
    Log->puts("VAO: none!\n");
    return false;
}

bool CRender::EnableVBOFunctions(){
    if ( isExtensionSupported ( "GL_ARB_vertex_buffer_object" ) ){
        glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)          GetProcAddress("glGenBuffersARB");
        glBindBufferARB = (PFNGLBINDBUFFERARBPROC)          GetProcAddress("glBindBufferARB");
        glMapBufferARB = (PFNGLMAPBUFFERARBPROC)            GetProcAddress("glMapBufferARB");
        glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)        GetProcAddress("glUnmapBufferARB");
        glBufferDataARB = (PFNGLBUFFERDATAARBPROC)          GetProcAddress("glBufferDataARB");
        glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)    GetProcAddress("glBufferSubDataARB");
        glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)    GetProcAddress("glDeleteBuffersARB");
        glGetBufferSubDataARB = (PFNGLGETBUFFERSUBDATAARBPROC)  GetProcAddress("glGetBufferSubDataARB");
        if(glGenBuffersARB && glBindBufferARB && glMapBufferARB && glUnmapBufferARB &&
           glBufferDataARB && glBufferSubDataARB && glDeleteBuffersARB && glGetBufferSubDataARB){
                Log->puts("VBO: OK\n");
                return true;
        }
    }
    Log->puts("VBO: none!\n");
    return false;
}

bool CRender::EnableShadersFunctions(){
    if(     isExtensionSupported("GL_ARB_shading_language_100") &&
            isExtensionSupported("GL_ARB_shader_objects") &&
            isExtensionSupported("GL_ARB_fragment_shader") &&
            isExtensionSupported("GL_ARB_vertex_shader")){
        // inicialize functions
        glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)    GetProcAddress("glCreateProgramObjectARB");
        glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)                  GetProcAddress("glDeleteObjectARB");
        glUseProgramObjectARB  = (PFNGLUSEPROGRAMOBJECTARBPROC)         GetProcAddress("glUseProgramObjectARB");
        glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)      GetProcAddress("glCreateShaderObjectARB");

        glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)                  GetProcAddress("glShaderSourceARB");
        glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)                GetProcAddress("glCompileShaderARB");
        glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)  GetProcAddress("glGetObjectParameterivARB");
        glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)                  GetProcAddress("glAttachObjectARB");

        glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)                      GetProcAddress("glGetInfoLogARB");
        glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)                    GetProcAddress("glLinkProgramARB");
        glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)      GetProcAddress("glGetUniformLocationARB");
        glUniform1fARB = (PFNGLUNIFORM1FARBPROC)                        GetProcAddress("glUniform1fARB");

        glUniform2fARB = (PFNGLUNIFORM2FARBPROC)                        GetProcAddress("glUniform2fARB");
        glUniform3fARB = (PFNGLUNIFORM3FARBPROC)                        GetProcAddress("glUniform3fARB");
        glUniform4fARB = (PFNGLUNIFORM4FARBPROC)                        GetProcAddress("glUniform4fARB");
        glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC)                      GetProcAddress("glUniform1fvARB");

        glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC)                      GetProcAddress("glUniform2fvARB");
        glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)                      GetProcAddress("glUniform3fvARB");
        glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)                      GetProcAddress("glUniform4fvARB");
        glUniform1iARB = (PFNGLUNIFORM1IARBPROC)                        GetProcAddress("glUniform1iARB");

        glBindAttribLocationARB = (PFNGLBINDATTRIBLOCATIONARBPROC)      GetProcAddress("glBindAttribLocationARB");
        glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC)          GetProcAddress("glGetActiveUniformARB");
        glGetShaderiv = (PFNGLGETSHADERIVPROC)                          GetProcAddress("glGetShaderiv");
        glGetProgramiv = (PFNGLGETPROGRAMIVPROC)                        GetProcAddress("glGetProgramiv");
        // check functions
        if(glCreateProgramObjectARB && glDeleteObjectARB && glUseProgramObjectARB && glCreateShaderObjectARB &&
           glShaderSourceARB && glCompileShaderARB && glGetObjectParameterivARB && glAttachObjectARB &&
           glGetInfoLogARB && glLinkProgramARB && glGetUniformLocationARB && glUniform1fARB &&
           glUniform2fARB && glUniform3fARB && glUniform4fARB && glUniform1fvARB &&
           glUniform2fvARB && glUniform3fvARB && glUniform4fvARB && glUniform1iARB &&
           glBindAttribLocationARB && glGetActiveUniformARB && glGetShaderiv && glGetProgramiv
        ){
            Log->puts("Shaders: OK\n");
            return true;
        }
    }
    Log->puts("Shaders: none!\n");
    return false;
}

// Check OpenGL Errors and add to Log
bool CRender::CheckError(void){
    GLenum error;
    if((error=glGetError()) != GL_NO_ERROR){
        switch(error){
            case GL_INVALID_ENUM:
                Log->printf("OpenGL ERROR: %s\n","GL_INVALID_ENUM");
                break;
            case GL_INVALID_VALUE:
                Log->printf("OpenGL ERROR: %s\n","GL_INVALID_VALUE");
                break;
            case GL_INVALID_OPERATION:
                Log->printf("OpenGL ERROR: %s\n","GL_INVALID_OPERATION");
                break;
            case GL_STACK_OVERFLOW:
                Log->printf("OpenGL ERROR: %s\n","GL_STACK_OVERFLOW");
                break;
            case GL_STACK_UNDERFLOW:
                Log->printf("OpenGL ERROR: %s\n","GL_STACK_UNDERFLOW");
                break;
            case GL_OUT_OF_MEMORY:
                Log->printf("OpenGL ERROR: %s\n","GL_OUT_OF_MEMORY");
                break;
            default:
                Log->printf("OpenGL ERROR: %x\n",error);
                break;
        }
        return true;
    }else{
        return false;
    }
}

void CRender::RenderScreenQuad(GLuint texture_id){
   // const GLuint target=GL_TEXTURE_2D;
    glMatrixMode   ( GL_MODELVIEW );
    glPushMatrix   ();
    glLoadIdentity ();
    MyOGL::Render->Set2D();
    //glClear ( GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );
    GL.Enable(GL_TEXTURE_2D);
    MyOGL::Render->BindTexture(texture_id);
    glBegin( GL_QUADS );//glOrtho(0, this->m_width, this->m_height, 0, 1, -1);
        glTexCoord2f ( 0, 0 ); glVertex2i( 0, 0 );
        glTexCoord2f ( 1, 0 ); glVertex2i( this->m_width, 0 );
        glTexCoord2f ( 1, 1 ); glVertex2i( this->m_width, this->m_height );
        glTexCoord2f ( 0, 1 ); glVertex2i( 0, this->m_height );
    glEnd();
    glMatrixMode ( GL_MODELVIEW );
    glPopMatrix  ();
/*
    glMatrixMode   ( GL_PROJECTION );
    glPushMatrix   ();
    glLoadIdentity ();

    glOrtho        ( 0, 1, 0, 1, -1, 1 );
    glMatrixMode   ( GL_MODELVIEW );
    glPushMatrix   ();
    glLoadIdentity ();

    glClear ( GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT );
    //GL.Disable(GL_LIGHTING);

    this->BindTexture(texture_id);
    GL.Enable(GL_TEXTURE_2D);
    glBegin       ( GL_QUADS );
        glTexCoord2f ( 0, 0 ); glVertex2f   ( 0, 0 );
        glTexCoord2f ( 1, 0 ); glVertex2f   ( 1, 0 );
        glTexCoord2f ( 1, 1 ); glVertex2f   ( 1, 1 );
        glTexCoord2f ( 0, 1 ); glVertex2f   ( 0, 1 );
    glEnd;
    this->BindTexture(0);
    GL.Disable(GL_TEXTURE_2D);
    glMatrixMode ( GL_PROJECTION );
    glPopMatrix  ();
    glMatrixMode ( GL_MODELVIEW );
    glPopMatrix  ();

    GL.Enable(GL_LIGHTING);
    */
};

// Create ProjectionMatrix
void CRender::Matrix4Perspective(float *M, float fovy, float aspect, float znear, float zfar){
        // convert fovy from degreases to radian
        float f = 1 / tanf(fovy * M_PI / 360),
              A = (zfar + znear) / (znear - zfar),
              B = (2 * zfar * znear) / (znear - zfar);

        M[ 0] = f / aspect; M[ 1] =  0; M[ 2] =  0; M[ 3] =  0;
        M[ 4] = 0;          M[ 5] =  f; M[ 6] =  0; M[ 7] =  0;
        M[ 8] = 0;          M[ 9] =  0; M[10] =  A; M[11] =  B;
        M[12] = 0;          M[13] =  0; M[14] = -1; M[15] =  0;
};

void CRender::LookAt(float eyex, float eyey, float eyez,
                     float centerx, float centery, float centerz,
                     float upx, float upy, float upz){

   GLfloat m[16];

//    Make rotation matrix

//    Z vector
    Vector3f eye=Vector3f(eyex, eyey, eyez);
    Vector3f center=Vector3f(centerx, centery, centerz);

    Vector3f vZ=eye-center;
    vZ=vZ.normalize();

//    Y vector
    Vector3f vY=Vector3f(upx, upy, upz);
//    X vector = Y cross Z
    Vector3f vX=vY.cross(vZ);
//    Recompute Y = Z cross X
    vY=vZ.cross(vX);

    vX=vX.normalize();
    vY=vY.normalize();

#define M(row,col)  m[col*4+row]
   M(0, 0) = vX.x;
   M(0, 1) = vX.y;
   M(0, 2) = vX.z;
   M(0, 3) = 0.0;
   M(1, 0) = vY.x;
   M(1, 1) = vY.y;
   M(1, 2) = vY.z;
   M(1, 3) = 0.0;
   M(2, 0) = vZ.x;
   M(2, 1) = vZ.y;
   M(2, 2) = vZ.z;
   M(2, 3) = 0.0;
   M(3, 0) = 0.0;
   M(3, 1) = 0.0;
   M(3, 2) = 0.0;
   M(3, 3) = 1.0;
#undef M
   glMultMatrixf(m);

//    Translate Eye to Origin
   glTranslatef(-eyex, -eyey, -eyez);

}


// cashed OGL states
void RenderStates::Enable(GLenum cap){
    bool *param;
    param=GetGLParam(cap);
    if(param && !*param){
        *param=true;
        glEnable(cap);
    }
}

// cashed OGL states
void RenderStates::Disable(GLenum cap){
    bool *param;
    param=GetGLParam(cap);
    if(param && *param){
        *param=false;
        glDisable(cap);
    }
}

bool *RenderStates::GetGLParam(GLenum cap){
    switch(cap){
        case GL_COLOR_MATERIAL:
            return &ColorMaterial;
        case GL_TEXTURE_2D:
            return &Texture2D;
        case GL_DEPTH_TEST:
            return &DepthTest;
        case GL_LIGHTING:
            return &Lighting;
        case GL_LIGHT0:
            return &Light0;
        case GL_LIGHT1:
            return &Light1;
        case GL_LIGHT2:
            return &Light2;
        case GL_LIGHT3:
            return &Light3;
        case GL_LIGHT4:
            return &Light4;
        case GL_LIGHT5:
            return &Light5;
        case GL_LIGHT6:
            return &Light6;
        case GL_LIGHT7:
            return &Light7;
        case GL_BLEND:
            return &Blend;
        case GL_CULL_FACE:
            return &CullFace;
        default:
            Log->printf("Wrong GL.Enable parameter: %x\n",cap);
            return NULL;
    }
}

void RenderStates::GetCurrentStates(){
//
   ColorMaterial=glIsEnabled(GL_COLOR_MATERIAL);
   Texture2D=glIsEnabled(GL_TEXTURE_2D);
   DepthTest=glIsEnabled(GL_DEPTH_TEST);
   Lighting=glIsEnabled(GL_LIGHTING);
   Light0=glIsEnabled(GL_LIGHT0);
   Light1=glIsEnabled(GL_LIGHT1);
   Light2=glIsEnabled(GL_LIGHT2);
   Light3=glIsEnabled(GL_LIGHT3);
   Light4=glIsEnabled(GL_LIGHT4);
   Light5=glIsEnabled(GL_LIGHT5);
   Light6=glIsEnabled(GL_LIGHT6);
   Light7=glIsEnabled(GL_LIGHT7);
   Blend=glIsEnabled(GL_BLEND);
   // blend color
   //glGetFloatv(GL_BLEND_COLOR, BlendColor.data);
   // get current color
   glGetFloatv(GL_CURRENT_COLOR, Color.data);
   // -//- clear color
   glGetFloatv(GL_COLOR_CLEAR_VALUE, ClearColor.data);
   // -//- texture
   // get max texture units
   glGetIntegerv(GL_MAX_TEXTURE_UNITS,&MaxTextureUnits);
}

void RenderStates::Debug(void){
    Log->puts("---[ OpenGL states Debug ]---\n");
    Log->printf("GL_COLOR_MATERIAL: %s\n",(ColorMaterial)?"true":"false");
    Log->printf("GL_TEXTURE_2D: %s\n",(Texture2D)?"true":"false");
    Log->printf("GL_DEPTH_TEST: %s\n",(DepthTest)?"true":"false");
    Log->printf("GL_LIGHTING: %s\n",(Lighting)?"true":"false");
    Log->printf("GL_LIGHT0: %s\n",(Light0)?"true":"false");
    Log->printf("GL_LIGHT1: %s\n",(Light1)?"true":"false");
    Log->printf("GL_LIGHT2: %s\n",(Light2)?"true":"false");
    Log->printf("GL_LIGHT3: %s\n",(Light3)?"true":"false");
    Log->printf("GL_LIGHT4: %s\n",(Light4)?"true":"false");
    Log->printf("GL_LIGHT5: %s\n",(Light5)?"true":"false");
    Log->printf("GL_LIGHT6: %s\n",(Light6)?"true":"false");
    Log->printf("GL_LIGHT7: %s\n",(Light7)?"true":"false");
    Log->printf("GL_BLEND: %s\n",(Blend)?"true":"false");
//    Log->puts("GL_BLEND_COLOR: (%f,%f,%f,%f)\n",&BlendColor);
    Log->puts("GL_CURRENT_COLOR: (%f,%f,%f,%f)\n",&Color);
    Log->puts("GL_COLOR_CLEAR_VALUE: (%f,%f,%f,%f)\n",&ClearColor);
    Log->printf("GL_MAX_TEXTURE_UNITS: %d\n",MaxTextureUnits);
    Log->puts("----------------------------\n");
}

// Scissor functions
void CRender::ScissorEnable(int left, int top, int width, int height){
    glEnable(GL_SCISSOR_TEST);
    glScissor(left, this->m_height-(top+height), width, height); // left, bottom, width, height
}

void CRender::ScissorDisable(){
    glDisable(GL_SCISSOR_TEST);
}

// Draw primitives
void CRender::DrawPoint(int x, int y){
    glBegin(GL_POINTS);
        glVertex2i(x,y);
    glEnd();
}

void CRender::DrawLine(int x1, int y1, int x2, int y2){
    glBegin(GL_LINES);
        glVertex2i( x1, y1 );
        glVertex2i( x2, y2 );
    glEnd();
}

void CRender::DrawQuad(int left, int top, int width, int height){
    int right=left+width;
    int bottom=top+height;
    glBegin( GL_QUADS );
	// Top-left vertex (corner)
        glVertex2i( left, top);
	// Top-right vertex (corner)
        glVertex2i( right, top);
	// Bottom-right vertex (corner)
        glVertex2i( right, bottom);
	// Bottom-left vertex (corner)
        glVertex2i( left, bottom);
    glEnd();
}


