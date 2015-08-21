#ifndef EVENTS_H_INCLUDED
#define EVENTS_H_INCLUDED

#ifdef __WIN32__
    #include <SDL2/SDL.h>
#else
    #include <SDL2/SDL.h>
#endif


class CEvent {
    public:
        CEvent();

        virtual ~CEvent();

        virtual void OnEvent(SDL_Event* Event);

        virtual void OnInputFocus();

        virtual void OnInputBlur();

        virtual void OnKeyDown(SDL_Scancode code);

        virtual void OnKeyUp(SDL_Scancode code);

        virtual void OnMouseFocus();

        virtual void OnMouseBlur();

        virtual void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);

        virtual void OnMouseWheel(int scrollX, int scrollY);

        virtual void OnLButtonDown(int mX, int mY);

        virtual void OnLButtonUp(int mX, int mY);

        virtual void OnRButtonDown(int mX, int mY);

        virtual void OnRButtonUp(int mX, int mY);

        virtual void OnMButtonDown(int mX, int mY);

        virtual void OnMButtonUp(int mX, int mY);

        virtual void OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);

        virtual void OnJoyButtonDown(Uint8 which, Uint8 button);

        virtual void OnJoyButtonUp(Uint8 which, Uint8 button);

        virtual void OnJoyHat(Uint8 which, Uint8 hat, Uint8 value);

        virtual void OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel);

        virtual void OnMinimize();

        virtual void OnRestore();

        virtual void OnResize(int window_id, int w, int h);

        virtual void OnExpose();

        virtual void OnExit();

        virtual void OnUser(Uint8 type, int code, void* data1, void* data2);
};

#endif // EVENTS_H_INCLUDED
