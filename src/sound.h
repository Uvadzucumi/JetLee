#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <iostream>


class CSoundBase{
    protected:
        ALCenum m_error;

    public:

    bool isError(){
    	m_error = alGetError();
        if (m_error != AL_NO_ERROR) {
            //fprintf(stderr, _msg "\n");
            return true;
        }
        return false;
    }
};

class CSoundBuffer:public CSoundBase{

        ALuint m_buffer;

    public:

        CSoundBuffer(){
            alGenBuffers(1, &m_buffer);
        }

        bool loadWavFile(std::string file_name, ALboolean loop = AL_FALSE){
            ALvoid *data;
            ALsizei size, freq;
            ALenum format;

            //ALboolean loop = AL_FALSE;
            // loading wav file
            alutLoadWAVFile((ALbyte *)file_name.c_str(), &format, &data, &size, &freq, &loop);

            if(!size){
                return false;
            }
            if(isError()){
                return false;
            }

            // buffer copy
            alBufferData(m_buffer, format, data, size, freq);

            if(isError()){
                return false;
            }

            return true;
        }

        ~CSoundBuffer(){
            alDeleteBuffers(1, &m_buffer);
        }

         ALuint getBuffer(){
            return m_buffer;
        }
};


class CSoundSource:public CSoundBase{

        ALuint m_source;

    public:

        CSoundSource(){

            alGenSources((ALuint)1, &m_source);
        //TEST_ERROR("source generation");
            alSourcef(m_source, AL_PITCH, 1);
        //TEST_ERROR("source pitch");
            alSourcef(m_source, AL_GAIN, 1);
        //TEST_ERROR("source gain");
            alSource3f(m_source, AL_POSITION, 0, 0, 0);
        //TEST_ERROR("source position");
            alSource3f(m_source, AL_VELOCITY, 0, 0, 0);
        //TEST_ERROR("source velocity");
            alSourcei(m_source, AL_LOOPING, AL_FALSE);
        //TEST_ERROR("source looping");
        }

        ~CSoundSource(){
            alDeleteSources(1, &m_source);
        }

        bool setVolume(float newVolume){
            alSourcef(m_source, AL_GAIN, newVolume);
            if(isError()){
                return false;
            }
            return true;
        }

        bool setBuffer(ALuint buffer){
            alSourcei(m_source, AL_BUFFER, buffer);
            if(isError()){
                return false;
            }
            return true;
        }

        bool getState(ALint *source_state){
            alGetSourcei(m_source, AL_SOURCE_STATE, source_state);
            if(isError()){
                return false;
            }
            return true;
        }

        bool play(){
            alSourcePlay(m_source);
            if(isError()){
                return false;
            }
            return true;
        }

};

class CSoundSystem:public CSoundBase{

        ALboolean m_enumeration;
        const ALCchar *m_defaultDeviceName;
        ALCdevice *m_device;
        ALCcontext *m_context;

    public:

    CSoundSystem(){
        m_error=AL_NO_ERROR;
        m_enumeration=false;
        m_defaultDeviceName=NULL;
        m_device=NULL;
    }

    ~CSoundSystem(){
    	m_device = alcGetContextsDevice(m_context);
        alcMakeContextCurrent(NULL);
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
    }

    bool init(){
        m_enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        if (m_enumeration == AL_FALSE)
            fprintf(stderr, "enumeration extension not available\n");

        this->listAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

        if (!m_defaultDeviceName)
            m_defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

        m_device = alcOpenDevice(m_defaultDeviceName);

        if (!m_device) {
            fprintf(stderr, "unable to open default device\n");
            return false;
        }

        fprintf(stdout, "Device: %s\n", alcGetString(m_device, ALC_DEVICE_SPECIFIER));

        //alGetError(); // ???

        m_context = alcCreateContext(m_device, NULL);
        if (!alcMakeContextCurrent(m_context)) {
            std::cout << "failed to make default context" << std::endl;
            return false;
        }
        if(this->isError()){
            std::cout << "[" << m_error << "] create context error" << std::endl;
            return false;
        }

        if(!setListenerOrientation()){
            std::cout << "[" << m_error << "] set listener orientation error" << std::endl;
            return false;
        }

        return true;
    }

    // set orientation
    bool setListenerOrientation(){
    	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };


        alListener3f(AL_POSITION, 0, 0, 1.0f);
        if(isError()){
            return false;
        }

        alListener3f(AL_VELOCITY, 0, 0, 0);
        if(isError()){
            return false;
        }

        alListenerfv(AL_ORIENTATION, listenerOri);

        if(isError()){
            return false;
        }
        return true;
    }

    static inline ALenum toAlFormat(short channels, short samples){
        bool stereo = (channels > 1);

        switch (samples) {
        case 16:
            if (stereo)
                return AL_FORMAT_STEREO16;
            else
                return AL_FORMAT_MONO16;
        case 8:
            if (stereo)
                return AL_FORMAT_STEREO8;
            else
                return AL_FORMAT_MONO8;
        default:
            return -1;
        }
    }

    static void listAudioDevices(const ALCchar *devices){

        const ALCchar *device = devices, *next = devices + 1;
        size_t len = 0;

        fprintf(stdout, "Devices list:\n");
        fprintf(stdout, "----------\n");
        while (device && *device != '\0' && next && *next != '\0') {
            fprintf(stdout, "%s\n", device);
            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
        }
        fprintf(stdout, "----------\n");
    }

};

#endif // SOUND_H_INCLUDED
