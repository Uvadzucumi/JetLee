#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

#include <stdio.h>
#include <string.h>
/*
log(...) {
    char str[100];
    sprintf(str, __VA_ARGS__);
    std::cout  << "[" << __FILE__ << "][" << __PRETTY_FUNCTION__ << "][Line " << __LINE__ << "] " << str << std::endl;
    }
*/

#include "config.h" // enable/disable create log file
#include "vector_types.h"

#ifdef DEBUG

#define log(string) \
    std::cout << string << std::endl;

#define logW(string) \
    std::cout << "[WARNING] " << string << std::endl;


#define logE(string) \
    std::cout << "[ERROR] " << string << std::endl;

#else
#define log(...)
#define logW(...)
#define logE(...)
#endif

namespace MyOGL{


class CLog{
        char m_file_name[100];
        FILE *fp;
    public:
        void Init(const char *file_name);
        void puts(const char *str);
        void puts(const char *str, Vector4f *color);
        void printf(const char *format, ...);
        bool open_file(void);
        void close_file(void);
};

    extern CLog *Log;

}

#endif // LOG_H_INCLUDED
