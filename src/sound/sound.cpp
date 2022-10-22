
#include "sound.h"

/*
* Load wave file function. No need for ALUT with this
*/
//bool loadWavFile(const std::string filename, ALenum* format, ALuint* buffer,
bool CSoundBuffer::loadWavFile(const std::string filename, ALenum* format, unsigned char* data,
                 ALsizei* size, ALsizei* frequency) {
    //Local Declarations
    FILE* soundFile = NULL;
    WAVE_Format wave_format;
    RIFF_Header riff_header;
    WAVE_Data wave_data;

    soundFile = fopen(filename.c_str(), "rb");
    if (!soundFile){
        std::cerr << "Error opening " << filename.c_str() << std::endl;
        return false;
    }

    // Read in the first chunk into the struct
    if(!fread(&riff_header, sizeof(RIFF_Header), 1, soundFile)){
        std::cerr << "Error read first chunk" << std::endl;
        fclose(soundFile);
        return false;
    }

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
        std::cerr << "Wrong file header" << std::endl;
        fclose(soundFile);
        return false;
    }


    //Read in the 2nd chunk for the wave info
    if(!fread(&wave_format, sizeof(WAVE_Format), 1, soundFile)){
        std::cerr << "Error read second chunk!" << std::endl;
        fclose(soundFile);
        return false;
    }
    //check for fmt tag in memory
    if (wave_format.subChunkID[0] != 'f' ||
        wave_format.subChunkID[1] != 'm' ||
        wave_format.subChunkID[2] != 't' ||
        wave_format.subChunkID[3] != ' '){
        std::cerr << "Invalid Wave Format" << std::endl;
        fclose(soundFile);
        return false;
    }

    //check for extra parameters;
    if (wave_format.subChunkSize > 16){
        fseek(soundFile, sizeof(short), SEEK_CUR);
    }

    //Read in the the last byte of data before the sound file
    if(!fread(&wave_data, sizeof(WAVE_Data), 1, soundFile)){
        std::cerr << "Error read data header" << std::endl;
        fclose(soundFile);
        return false;
    }
    //check for data tag in memory
    if (wave_data.subChunkID[0] != 'd' ||
        wave_data.subChunkID[1] != 'a' ||
        wave_data.subChunkID[2] != 't' ||
        wave_data.subChunkID[3] != 'a'){
        std::cerr << "Invalid data header" << std::endl;
        fclose(soundFile);
        return false;
    }


    //Allocate memory for data
    data = new unsigned char[wave_data.subChunk2Size];

    // Read in the sound data into the soundData variable
    if (!fread(data, wave_data.subChunk2Size, 1, soundFile)){
        fclose(soundFile);
        std::cerr << "error loading WAVE data into struct!" << std::endl;
        return false;
    }
    fclose(soundFile);


    //Now we set the variables that we passed in with the
    //data from the structs
    *size = wave_data.subChunk2Size;
    *frequency = wave_format.sampleRate;
    //The format is worked out by looking at the number of
    //channels and the bits per sample.
    *format=this->toAlFormat(wave_format.numChannels, wave_format.bitsPerSample);
    /*
    if (wave_format.numChannels == 1) {
        if (wave_format.bitsPerSample == 8 )
            *format = AL_FORMAT_MONO8;
        else if (wave_format.bitsPerSample == 16){
            *format = AL_FORMAT_MONO16;
        }else{
            // wrong format
            return false;
        }
    } else if (wave_format.numChannels == 2) {
        if (wave_format.bitsPerSample == 8 )
            *format = AL_FORMAT_STEREO8;
        else if (wave_format.bitsPerSample == 16){
            *format = AL_FORMAT_STEREO16;
        }else{
            // wrong format
            return false;
        }
    }*/

    alBufferData(m_buffer, *format, (void*)data,
                 *size, *frequency);
    return true;
}
#if ENABLE_MUSIC
bool CSoundBuffer::loadOggFile(std::string file_name){
// This function loads a .ogg file into a memory buffer and returns
// the format and frequency.
//        bool LoadOggFile(std::string file_name, vector<char> &buffer, ALenum &format, ALsizei &freq){
    //ALvoid *data;
    std::vector<char> buffer;
    ALsizei freq;
    ALenum format;

    int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];                // Local fixed size array
    FILE *f;

    // Open for binary reading
    f = fopen(file_name.c_str(), "rb");

    if (f == NULL){
        std::cerr << "Cannot open " << file_name << " for reading..." << std::endl;
        return false;
    }
    // end if

    vorbis_info *pInfo;
    OggVorbis_File oggFile;

    // Try opening the given file
    if (ov_open(f, &oggFile, NULL, 0) != 0){
        std::cerr << "Error opening " << file_name << " for decoding..." << std::endl;
        return false;
    }

    // Get some information about the OGG file
    pInfo = ov_info(&oggFile, -1);

    // Check the number of channels... always use 16-bit samples
    format = this->toAlFormat(pInfo->channels, 16);
    /*
    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
    // end if
     */

    // The frequency of the sampling rate
    freq = pInfo->rate;

    // Keep reading until all is read
    do
    {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

        if (bytes < 0){
            ov_clear(&oggFile);
            std::cerr << "Error decoding " << file_name << "..." << std::endl;
            return false;
        }

        // Append to end of buffer
        buffer.insert(buffer.end(), array, array + bytes);
    }
    while (bytes > 0);

    // Clean up!
    ov_clear(&oggFile);
    // buffer copy
    alBufferData(m_buffer, format, &buffer[0], buffer.size(), freq);

    if(isError()){
        return false;
    }

    return true;

}
#endif
