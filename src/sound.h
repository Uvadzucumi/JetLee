#ifndef SOUND_H_INCLUDED
#define SOUND_H_INCLUDED

#include <AL/al.h>
#include <AL/alc.h>
//#include <AL/alut.h>

#include <iostream>


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

            //ALvoid *data;
            unsigned char* data=NULL;
            ALsizei size, freq;
            ALenum format;
        /*
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
             //now we put our data into the openAL buffer and
        //check for success
        alBufferData(*buffer, *format, (void*)data,
                     *size, *frequency);
        if(isError()){
            return false;
        }
        //clean up and return true if successful
        fclose(soundFile);

            if(isError()){
                return false;
            }

            return true;
            */

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

     /*
     * Load wave file function. No need for ALUT with this
     */
    //bool loadWavFile(const std::string filename, ALenum* format, ALuint* buffer,
    bool loadWavFile(const std::string filename, ALenum* format, unsigned char* data,
                     ALsizei* size, ALsizei* frequency) {
      //Local Declarations
      FILE* soundFile = NULL;
      WAVE_Format wave_format;
      RIFF_Header riff_header;
      WAVE_Data wave_data;
      //unsigned char* data;

      //try {
        soundFile = fopen(filename.c_str(), "rb");
        if (!soundFile){
            return false;
        }


        // Read in the first chunk into the struct
        fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

        //check for RIFF and WAVE tag in memeory
        if ((riff_header.chunkID[0] != 'R' ||
             riff_header.chunkID[1] != 'I' ||
             riff_header.chunkID[2] != 'F' ||
             riff_header.chunkID[3] != 'F') ||
            (riff_header.format[0] != 'W' ||
             riff_header.format[1] != 'A' ||
             riff_header.format[2] != 'V' ||
             riff_header.format[3] != 'E')){
             //    throw ("Invalid RIFF or WAVE Header");
                fclose(soundFile);
                return false;
             }


        //Read in the 2nd chunk for the wave info
        fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
        //check for fmt tag in memory
        if (wave_format.subChunkID[0] != 'f' ||
            wave_format.subChunkID[1] != 'm' ||
            wave_format.subChunkID[2] != 't' ||
            wave_format.subChunkID[3] != ' '){
                //     throw ("Invalid Wave Format");
                fclose(soundFile);
                return false;
            }


        //check for extra parameters;
        if (wave_format.subChunkSize > 16){
            fseek(soundFile, sizeof(short), SEEK_CUR);
        }

        //Read in the the last byte of data before the sound file
        fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        //check for data tag in memory
        if (wave_data.subChunkID[0] != 'd' ||
            wave_data.subChunkID[1] != 'a' ||
            wave_data.subChunkID[2] != 't' ||
            wave_data.subChunkID[3] != 'a'){
                //      throw ("Invalid data header");
                fclose(soundFile);
                return false;
            }


        //Allocate memory for data
        data = new unsigned char[wave_data.subChunk2Size];

        // Read in the sound data into the soundData variable
        if (!fread(data, wave_data.subChunk2Size, 1, soundFile)){
            fclose(soundFile);
            //throw ("error loading WAVE data into struct!");
            return false;
        }
        fclose(soundFile);


        //Now we set the variables that we passed in with the
        //data from the structs
        *size = wave_data.subChunk2Size;
        *frequency = wave_format.sampleRate;
        //The format is worked out by looking at the number of
        //channels and the bits per sample.
        if (wave_format.numChannels == 1) {
            if (wave_format.bitsPerSample == 8 )
                *format = AL_FORMAT_MONO8;
            else if (wave_format.bitsPerSample == 16)
                *format = AL_FORMAT_MONO16;
        } else if (wave_format.numChannels == 2) {
            if (wave_format.bitsPerSample == 8 )
                *format = AL_FORMAT_STEREO8;
            else if (wave_format.bitsPerSample == 16)
                *format = AL_FORMAT_STEREO16;
        }
        //create our openAL buffer and check for success
        //alGenBuffers(1, buffer);
        //if(isError()){
        //    return false;
        //}
        //now we put our data into the openAL buffer and
        //check for success
        alBufferData(m_buffer, *format, (void*)data,
                     *size, *frequency);
        //if(isError()){
        //    return false;
        //}
        //clean up and return true if successful
        return true;
      //} catch(std::string error) {
        //our catch statement for if we throw a string
      //  std::cerr << error << " : trying to load "
      //            << filename << std::endl;
        //clean up memory if wave loading fails
      //  if (soundFile != NULL)
      //      fclose(soundFile);
        //return false to indicate the failure to load wave
      //  return false;
      //}
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
