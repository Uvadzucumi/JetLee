#include "myogl.h"
#include "log.h"

#include <unistd.h> // for sleep

/* For detect user directory */
//#ifdef __LINUX__
//    #include <unistd.h> // getuid()
//    #include <pwd.h>    // getpwuid()
//#endif

using namespace MyOGL;

bool CApplication::Init(int width, int height, int bpp, bool full_screen, const char *title){

// set user home folder
#ifdef __LINUX__
//    printf("User Directory: %s\n", getenv("HOME"));
//    struct passwd *user = NULL;
//    uid_t user_id = getuid();
//    user = getpwuid(user_id);
//    printf("home dir: %s\n", user->pw_dir);
    user_home_dir=getenv("HOME");
#else
    // Win: %HOMEPATH%, %USERPROFILE%
    user_home_dir=getenv("USERPROFILE");
#endif
    Log=new CLog();
    Log->Init("log.txt");
    Log->printf("user home dir: %s\n",user_home_dir);
    // init keys state array
    for(int i = 0; i < SDL_NUM_SCANCODES; i++) { // init them all to false
        KEYS[i] = false;
        KEYS_DOWN[i] = false;
    }
    // create window
    if(Render!=NULL){
        Log->puts("CApp::Init - Render not NULL!\n");
        return false;
    }
    Render=new CRender();
    // init render object
    if(!Render->Init(width,height,bpp,full_screen,title)){
        return false;
    }
    return true;
};

int CApplication::Run(void){
    unsigned int currTime, lastTime, savedTime=0; // curren ticks count, previos framw tick count, saved tick count for frames calculator
    unsigned int frames=0;
    SDL_Event Event;
    // Check inicialized render class
    if(!Render){
        Log->puts("CApplication::Run() - Render class not Inicialized!\n");
        return -1;
    }
    this->Running=true;
    // inicialize fpc counter and dt calculation
    lastTime=SDL_GetTicks()-1;
    while(Running){
        currTime=SDL_GetTicks();
        DeltaTime=(float)(currTime-lastTime)/1000.0;
        if((currTime-savedTime)>100){
            FPS=frames*1000/(currTime-savedTime);
            frames=0;
            savedTime=currTime;
        }
        lastTime=currTime;
        // check events
        while(SDL_PollEvent(&Event)) {
            if(Event.type == SDL_QUIT) {
                Running = false;
            }else{
                this->Events(&Event, DeltaTime);
            }
        }
        // render scene

        if(OnRender!=NULL){
            OnRender(DeltaTime);
        }

        // update all entity
        for(unsigned int i = 0;i < MyOGL::EntityList.size();i++) {
            if(!MyOGL::EntityList[i]) continue;
            MyOGL::EntityList[i]->onLoop(DeltaTime);
        }

        // timer actions... animation and etc...
        if(OnLoop!=NULL){
            OnLoop(DeltaTime);
        }

        Render->SwapBuffers();
        usleep(10);
        frames++;
    }
    Free();    // FreeMemory
    return 0;
}

// parse all SDL events
void CApplication::Events(SDL_Event *Event, double DeltaTime){
    switch(Event->type) {

        case SDL_KEYDOWN:
            KEYS[Event->key.keysym.scancode] = true;
            KEYS_DOWN[Event->key.keysym.scancode] = true;
                //Log->printf("Key %d pressed\n",Event->key.keysym.sym);
            break;
        case SDL_KEYUP:
            KEYS[Event->key.keysym.scancode] = false;
                //Log->printf("Key %d unpressed\n",Event->key.keysym.sym);
            break;
        case SDL_MOUSEMOTION:
            MOUSE.coords.x=Event->motion.x;
            MOUSE.coords.y=Event->motion.y;
            if(OnMouseMove){
                OnMouseMove( Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, \
                            (Event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT))!=0, \
                            (Event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT))!=0, \
                            (Event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    MOUSE.button_l=true;
                    MOUSE.coords_lb_down.x=Event->button.x;
                    MOUSE.coords_lb_down.y=Event->button.y;
                    if(OnLButtonDown){
                        OnLButtonDown(Event->button.x,Event->button.y);
                    }
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    MOUSE.button_r=true;
                    MOUSE.coords_rb_down.x=Event->button.x;
                    MOUSE.coords_rb_down.y=Event->button.y;
                    if(OnRButtonDown){
                        OnRButtonDown(Event->button.x,Event->button.y);
                    }
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    MOUSE.button_m=true;
                    MOUSE.coords_mb_down.x=Event->button.x;
                    MOUSE.coords_mb_down.y=Event->button.y;
                    if(OnMButtonDown){
                        OnMButtonDown(Event->button.x,Event->button.y);
                    }
                    break;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    MOUSE.button_l=false;
                    MOUSE.coords_lb_up.x=Event->button.x;
                    MOUSE.coords_lb_up.y=Event->button.y;
                    if(OnLButtonUp){
                        OnLButtonUp(Event->button.x,Event->button.y);
                    }
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    MOUSE.button_r=false;
                    MOUSE.coords_rb_up.x=Event->button.x;
                    MOUSE.coords_rb_up.y=Event->button.y;
                    if(OnRButtonUp){
                        OnRButtonUp(Event->button.x,Event->button.y);
                    }
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    MOUSE.button_m=false;
                    MOUSE.coords_mb_up.x=Event->button.x;
                    MOUSE.coords_mb_up.y=Event->button.y;
                    if(OnMButtonUp){
                        OnMButtonUp(Event->button.x,Event->button.y);
                    }
                    break;
                }
            }
            break;
            case SDL_MOUSEWHEEL:{
                if(OnMouseWheel){
                    OnMouseWheel(Event->wheel.x, Event->wheel.y);
                }
                break;
            }
    }
    // mouse events
    // check gui
    //if(Event->)
    // user events function
    if(OnEvent!=NULL) OnEvent(Event, DeltaTime);
    // send event to event class
    CEvent::OnEvent(Event);
}

// Clear All Allpication Data
void CApplication::Free(){
    // clear textures list
    Log->puts("Application::Free()\n");
    Log->printf("textures count: %d\n",TexturesList.size());
    for(int i = TexturesList.size()-1; i >=0;i--){
        if(!TexturesList[i]) continue;
        delete TexturesList[i];
    }
    TexturesList.clear();

    // clear Entity list
    Log->printf("entity count: %d\n",EntityList.size());
    for(int i = EntityList.size()-1;i >=0 ;i--) {
        if(!EntityList[i]) continue;
        delete EntityList[i];
    }
    EntityList.clear();
    if(Render!=NULL) delete Render;
}

void CApplication::OnResize(int window_id, int width, int height){
    Render->OnResize(window_id, width, height);
    if(this->OnWindowResize){ // User defined event
        OnWindowResize(width, height);
    }
}

CApplication* CApplication::getInstance()
{
    return Singleton<CApplication>::get();
}
