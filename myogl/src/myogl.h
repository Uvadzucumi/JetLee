#ifndef MYOGL_H_INCLUDED
#define MYOGL_H_INCLUDED

#ifdef __WIN32__
    #include <windows.h>
#endif

#include <stdio.h>

#include "config.h"
#include "types.h"
#include "render.h"
#include "events.h"
#include "entity.h"

#include "singleton.h"

namespace MyOGL{
    // SDLK_DOWN events
    #define MYOGL_BUTTONS_COUNT 322

    struct MouseState{
        bool button_l;
        bool button_m;
        bool button_r;
        Vector2i coords;
        Vector2i coords_lb_down;
        Vector2i coords_lb_up;
        Vector2i coords_rb_down;
        Vector2i coords_rb_up;
        Vector2i coords_mb_down;
        Vector2i coords_mb_up;
    };

    // application class
    class CApplication : public CEvent{

            friend class Singleton<CApplication>;

            float FPS;
            double DeltaTime;
            void Events(SDL_Event *Event,  double DeltaTime);  // parse application events
            bool Running;
            bool KEYS[SDL_NUM_SCANCODES];  // keys pressed
            bool KEYS_DOWN[SDL_NUM_SCANCODES];  // keys down events
            MouseState MOUSE;
            char *user_home_dir;
        public:
            static CApplication* getInstance();
            CApplication(){
                FPS=-1;
                Running=false;
                OnRender=NULL;
                OnLoop=NULL;
                OnEvent=NULL;
                OnWindowResize=NULL;
                // mouse events
                OnLButtonDown=NULL;
                OnRButtonDown=NULL;
                OnMButtonDown=NULL;
                OnLButtonUp=NULL;
                OnRButtonUp=NULL;
                OnMButtonUp=NULL;
                OnMouseMove=NULL;
                // user home directory path string
                user_home_dir=NULL;
                // set button states
                for(int i=0;i<MYOGL_BUTTONS_COUNT;i++){
                    KEYS[i]=false;
                    KEYS_DOWN[i]=false;
                }
                // set mouse state
                MOUSE.button_l=false;
                MOUSE.button_r=false;
                MOUSE.button_m=false;
            };
            ~CApplication(){
            }
            // create window and render context
            bool Init(int width=800, int height=600, int bpp=32, bool full_screen=false, const char *title=NULL);
            int Run(void); // Start Application
            void Free();   // clear all engine data
            void SetWinCaption(const char *title){ Render->SetWinCaption(title);};
            void SetWinIcon(const char *file_name){ Render->SetWinIcon(file_name);};
            float GetFPS(){ return FPS; }
            Vector2i GetMousePos(){ return MOUSE.coords; };
            // user functions
            void( *OnRender)(double dt);
            void( *OnLoop)(double dt);
            //void( *OnEvent)(SDL_Event *OnEvent);
            void( *OnEvent)(SDL_Event *Event, double DeltaTime);
            void( *OnWindowResize)(unsigned int width, unsigned int height);
            // mouse events
            void( *OnLButtonDown)(int x, int y);
            void( *OnRButtonDown)(int x, int y);
            void( *OnMButtonDown)(int x, int y);
            void( *OnLButtonUp)(int x, int y);
            void( *OnRButtonUp)(int x, int y);
            void( *OnMButtonUp)(int x, int y);
            void( *OnMouseWheel)(int scrollX, int scrollY);
            void( *OnMouseMove)(int x, int y, int xrel, int yrel, bool l_button, bool r_button, bool m_button);

            //void OnEvent(SDL_Event* Event);
            void OnExit() { Running = false; }
            void OnResize(int window_id, int w,int h);
            // key pressed state
            bool IsKeyPressed(SDL_Keycode key_code){
                SDL_Scancode scan_code=SDL_GetScancodeFromKey(key_code);
                if(scan_code>=SDL_NUM_SCANCODES) return false;
                return KEYS[scan_code];
            }
            bool IsKeyDown(SDL_Keycode key_code){
                SDL_Scancode scan_code=SDL_GetScancodeFromKey(key_code);
                if(scan_code>=SDL_NUM_SCANCODES) return false;
                bool ret_value=KEYS_DOWN[scan_code];
                KEYS_DOWN[scan_code]=false;
                return ret_value;
            }

    };



}

#endif // MYOGL_H_INCLUDED
