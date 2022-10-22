#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>

#if ENABLE_MUSIC
#include <ogg/ogg.h>
//#include <vorbis/codec.h>
//#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#endif

#include <iostream>
#include <vector>
#include <string.h>

#define BUFFER_SIZE 32768       // 32 KB buffers

enum ESoundState {
    ESS_NOT_INICIALIZED,
    ESS_STOPPED=0,
    ESS_PAUSED,
    ESS_PLAYING
};

/*
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the meta data information that holds,
 * the ID, size and format of the wave file
 */
struct RIFF_Header {
  char chunkID[4];
  unsigned int chunkSize;//size not including chunkSize or chunkID
  char format[4];
};
/*
 * Struct to hold fmt subchunk data for WAVE files.
 */
struct WAVE_Format {
  char subChunkID[4];
  unsigned int subChunkSize;
  short audioFormat;
  short numChannels;
  unsigned int sampleRate;
  unsigned int byteRate;
  short blockAlign;
  short bitsPerSample;
};

/*
* Struct to hold the data of the wave file
*/
struct WAVE_Data {
  char subChunkID[4]; //should contain the word data
  unsigned int subChunk2Size; //Stores the size of the data block
};


class CSoundBase{
    protected:
        ALCenum m_error=0;

    public:

    bool isError(){
    	m_error = alGetError();
        if (m_error != AL_NO_ERROR) {
            return true;
        }
        return false;
    }

    ALCenum getErrorCode(){
        return m_error;
    }
};

class CSoundBuffer:public CSoundBase{

        ALuint m_buffer;

    public:

        CSoundBuffer(){
            alGenBuffers(1, &m_buffer);
        }

        bool loadWavFile(std::string file_name){

            //ALvoid *data;
            unsigned char* data=NULL;
            ALsizei size, freq;
            ALenum format=0;

            //return loadWavFile(file_name, &format, &m_buffer, &size, &freq);
            if(!loadWavFile(file_name, &format, data, &size, &freq)){
                return false;
            }

            //alBufferData(m_buffer, format, (void *)data,
            //         size, freq);
             //now we put our data into the openAL buffer and
            if(isError()){
                return false;
            }
            return true;
        }

        ~CSoundBuffer(){
            alDeleteBuffers(1, &m_buffer);
            //delete (unsigned char)data[];
        }

        ALuint getBuffer(){
            return m_buffer;
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

    bool loadWavFile(const std::string filename, ALenum* format, unsigned char* data,
                                   ALsizei* size, ALsizei* frequency);
    bool loadOggFile(std::string file_name);
};


class CSoundSource:public CSoundBase{

        ALuint m_source;
        ESoundState m_state=ESS_NOT_INICIALIZED;

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

        void setLooping(ALboolean loop = AL_FALSE){
            alSourcei(m_source, AL_LOOPING, loop);
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
            m_state=ESS_PLAYING;
            return true;
        }

        void stop(){
            alSourceStop(m_source);
            m_state=ESS_STOPPED;
        }

        void pause(){
            alSourcePause(m_source);
            m_state=ESS_PAUSED;
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
        if (m_enumeration == AL_FALSE) {
            fprintf(stderr, "enumeration extension not available\n");
        }

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
