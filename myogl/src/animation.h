#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

namespace MyOGL{

    class CAnimation {
        private:
            int    CurrentFrame;
            int     FrameInc;

            double    FrameRate; //in seconds
            double    OldTime;

            bool Finished;
            bool FirstLoop;

        public:
            int    FramesCount;
            bool    Oscillate;

            CAnimation(int frames_count=0, float frame_rate=0.1, float oscillate=false);
            bool onAnimate(double DeltaTime);

            void setFramesCout(int FramesCount);
            void setFrameRate(double Rate);
            void setCurrentFrame(int Frame);
            int getCurrentFrame();

            void Start(){
                FirstLoop=true;
                Finished=false;
                CurrentFrame=0;
            }

            void Continue(){
                FirstLoop=false;
                Finished=false;
                CurrentFrame=0;
            }

            const bool isFinished(){
                return Finished;
            }

            const bool isFirstLoop(){
                return FirstLoop;
            }
    };

}
#endif // ANIMATION_H_INCLUDED
