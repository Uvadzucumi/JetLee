#include "events.h"

CEvent::CEvent() {
}

CEvent::~CEvent() {
    //Do nothing
}

void CEvent::OnEvent(SDL_Event* Event) {
    switch(Event->type) {

        case SDL_KEYDOWN: {
            OnKeyDown(Event->key.keysym.scancode);
            break;
        }

        case SDL_KEYUP: {
            OnKeyUp(Event->key.keysym.scancode);
            break;
        }

        case SDL_MOUSEMOTION: {
            OnMouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    OnLButtonDown(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    OnRButtonDown(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    OnMButtonDown(Event->button.x,Event->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:    {
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    OnLButtonUp(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    OnRButtonUp(Event->button.x,Event->button.y);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    OnMButtonUp(Event->button.x,Event->button.y);
                    break;
                }
            }
            break;
        }

        case SDL_MOUSEWHEEL: {
            OnMouseWheel(Event->wheel.x, Event->wheel.y);
            break;
        }

        case SDL_JOYAXISMOTION: {
            OnJoyAxis(Event->jaxis.which,Event->jaxis.axis,Event->jaxis.value);
            break;
        }

        case SDL_JOYBALLMOTION: {
            OnJoyBall(Event->jball.which,Event->jball.ball,Event->jball.xrel,Event->jball.yrel);
            break;
        }

        case SDL_JOYHATMOTION: {
            OnJoyHat(Event->jhat.which,Event->jhat.hat,Event->jhat.value);
            break;
        }
        case SDL_JOYBUTTONDOWN: {
            OnJoyButtonDown(Event->jbutton.which,Event->jbutton.button);
            break;
        }

        case SDL_JOYBUTTONUP: {
            OnJoyButtonUp(Event->jbutton.which,Event->jbutton.button);
            break;
        }

        case SDL_QUIT: {
            OnExit();
            break;
        }

        case SDL_SYSWMEVENT: {
            //Ignore
            break;
        }

        case SDL_WINDOWEVENT:     // window events
            switch (Event->window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    OnResize(Event->window.windowID, Event->window.data1, Event->window.data2);
                    break;
                case SDL_WINDOWEVENT_EXPOSED:   // required redraw
                    OnExpose();
                    break;
                case SDL_WINDOWEVENT_ENTER: // mouse gained focus
                    OnMouseFocus();
                    break;
                case SDL_WINDOWEVENT_LEAVE: // mouse lost focus
                    OnMouseBlur();
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED: // gained kyobord focus
                    OnInputFocus();
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST: // lost keyboard focus
                    OnInputBlur();
                    break;
                case SDL_WINDOWEVENT_MINIMIZED:
                    OnMinimize();
                    break;
                case SDL_WINDOWEVENT_RESTORED:
                    OnRestore();
                    break;
                default:
                    break;
            }
            break;

        default: {
            OnUser(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);
            break;
        }
    }
}

void CEvent::OnInputFocus() {
    //Pure virtual, do nothing
}

void CEvent::OnInputBlur() {
    //Pure virtual, do nothing
}

void CEvent::OnKeyDown(SDL_Scancode code) {
    //Pure virtual, do nothing
}

void CEvent::OnKeyUp(SDL_Scancode code) {
    //Pure virtual, do nothing
}

void CEvent::OnMouseFocus() {
    //Pure virtual, do nothing
}

void CEvent::OnMouseBlur() {
    //Pure virtual, do nothing
}

void CEvent::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle) {
    //Pure virtual, do nothing
}

void CEvent::OnMouseWheel(int mX, int mY){
    //Pure virtual, do nothing
}

void CEvent::OnLButtonDown(int scrollX, int scrollY) {
    //Pure virtual, do nothing
}

void CEvent::OnLButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void CEvent::OnRButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void CEvent::OnRButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void CEvent::OnMButtonDown(int mX, int mY) {
    //Pure virtual, do nothing
}

void CEvent::OnMButtonUp(int mX, int mY) {
    //Pure virtual, do nothing
}

void CEvent::OnJoyAxis(Uint8 which,Uint8 axis,Sint16 value) {
    //Pure virtual, do nothing
}

void CEvent::OnJoyButtonDown(Uint8 which,Uint8 button) {
    //Pure virtual, do nothing
}

void CEvent::OnJoyButtonUp(Uint8 which,Uint8 button) {
    //Pure virtual, do nothing
}

void CEvent::OnJoyHat(Uint8 which,Uint8 hat,Uint8 value) {
    //Pure virtual, do nothing
}

void CEvent::OnJoyBall(Uint8 which,Uint8 ball,Sint16 xrel,Sint16 yrel) {
    //Pure virtual, do nothing
}

void CEvent::OnMinimize() {
    //Pure virtual, do nothing
}

void CEvent::OnRestore() {
    //Pure virtual, do nothing
}

void CEvent::OnResize(int window_id, int w,int h) {
    //Pure virtual, do nothing
}

void CEvent::OnExpose() {
    //Pure virtual, do nothing
}

void CEvent::OnExit() {
    //Pure virtual, do nothing
}

void CEvent::OnUser(Uint8 type, int code, void* data1, void* data2) {
    //Pure virtual, do nothing
}
