#include "animation.h"

using namespace MyOGL;

CAnimation::CAnimation(int frames_count, float frame_rate, float oscillate) {
    CurrentFrame    = 0;
    FramesCount     = frames_count;
    FrameInc        = 1;

    FrameRate       = frame_rate; //seconds
    OldTime         = 0;

    Oscillate       = oscillate;
}

// return true if frame changed
bool CAnimation::onAnimate(double DeltaTime){
    OldTime+=DeltaTime;
    if(OldTime<FrameRate){
        return false;
    }
    OldTime-=FrameRate;
    CurrentFrame += FrameInc;
    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= FramesCount-1) {
                FrameInc = -FrameInc;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
            }
        }
    }else{
        if(CurrentFrame >= FramesCount) {
            CurrentFrame = 0;
            Finished=true;
        }
    }
    return true;
}

void CAnimation::setFrameRate(double rate) {
    FrameRate = rate;
}

void CAnimation::setFramesCout(int count) {
    FramesCount = count;
}

void CAnimation::setCurrentFrame(int frame) {
    if(frame < 0 || frame > FramesCount) return;
    CurrentFrame = frame;
}

int CAnimation::getCurrentFrame() {
    return CurrentFrame;
}
