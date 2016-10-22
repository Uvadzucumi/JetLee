#ifndef GAMESOUND_H_INCLUDED
#define GAMESOUND_H_INCLUDED

#include "sound/sound.h"

#include <vector>

#define SOUD_CREATE_MACRO(index, sound_file) \
        source=new CSoundSource(); \
        buffer=new CSoundBuffer(); \
        if(!buffer->loadWavFile(sound_file)){ \
            std::cout << "loading wav file "<< sound_file << " failed." << std::endl; \
            m_initialized=false; \
            return false; \
        } \
        if(!source->setBuffer(buffer->getBuffer())){ \
            std::cout << "buffer binding error!" << std::endl; \
            m_initialized=false; \
            return false; \
        } \
        buffer_collection.push_back(buffer); \
        sources_collection.push_back(source);

#define MUSIC_CREATE_MACRO(index, music_file) \
        source=new CSoundSource(); \
        buffer=new CSoundBuffer(); \
        if(!buffer->loadOggFile(music_file)){ \
            std::cout << "loading ogg file "<< music_file << " failed." << std::endl; \
            m_initialized=false; \
            return false; \
        } \
        if(!source->setBuffer(buffer->getBuffer())){ \
            std::cout << "buffer binding error!" << std::endl; \
            m_initialized=false; \
            return false; \
        } \
        source->setLooping(true); \
        buffer_collection.push_back(buffer); \
        sources_collection.push_back(source);

enum ESOUND{
    SOUND_STEP=0,
    SOUND_FIGHT,
    SOUND_FLARE_COLLECT,
    SOUND_DIE,
    SOUND_BG_MUSIC
};


class CGameSound{

    std::vector<CSoundBuffer*> buffer_collection;
    std::vector<CSoundSource*> sources_collection;

    CSoundSystem *m_sound_system;
    bool m_initialized;
public:
    CGameSound(){
        m_sound_system=NULL;
        m_initialized=false;
    }

    ~CGameSound(){
        // stop play & delete sources
        for(auto i=0; i < sources_collection.size(); ++i){
            sources_collection[i]->stop();
            delete sources_collection[i];
        }
        for(auto i=0; i < buffer_collection.size(); ++i){
            delete buffer_collection[i];
        }

        delete m_sound_system; // close AL context & device
    }

    bool init(){
        m_sound_system=new CSoundSystem();
        if(!m_sound_system->init()){
            m_initialized=false;
            return false;
        }
        return true;
    }

    bool loadSounds(){
        m_initialized=true;

        CSoundSource *source;
        CSoundBuffer *buffer;

        SOUD_CREATE_MACRO(SOUND_STEP, "data/step.wav");
        SOUD_CREATE_MACRO(SOUND_FIGHT, "data/fight.wav");
        SOUD_CREATE_MACRO(SOUND_FLARE_COLLECT, "data/flare.wav");
        SOUD_CREATE_MACRO(SOUND_DIE, "data/die.wav");

        MUSIC_CREATE_MACRO(SOUND_BG_MUSIC, "data/bg.ogg");

        return true;
    }

    const int getSourcesCount(){
        return sources_collection.size();
    }

    void play(ESOUND sound){
        if(m_initialized && isSound(sound)){
            sources_collection[sound]->play();
        }
    }

    void pause(ESOUND sound){
        if(m_initialized && isSound(sound)){
            sources_collection[sound]->pause();
        }
    }

    // check sound number in sound collection
    bool isSound(ESOUND sound){
        if(sound<0 || sound>=(int)sources_collection.size()){
            logW("Wrong sound number "<<sound);
            return false;
        }
        return true;
    }

    void setVolume(ESOUND sound, float volume){
        if(m_initialized && isSound(sound)){
            sources_collection[sound]->setVolume(volume);
        }
    }

};

#endif // GAMESOUND_H_INCLUDED
