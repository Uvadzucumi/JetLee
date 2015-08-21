
#include "log.h"
#include<stdarg.h>

using namespace MyOGL;

CLog *MyOGL::Log=NULL;

void CLog::Init(const char *file_name){
    strcpy(m_file_name,file_name);
#ifdef ENABLE_LOG_FILE
    remove(m_file_name);    // remove old log file
    printf("Log file name: %s\n",m_file_name);
#endif
}

bool CLog::open_file(void){
    fp=stdout;
#ifdef ENABLE_LOG_FILE
    if(!(fp=fopen(m_file_name,"a+"))){
        printf("Error open log file: %s\n",m_file_name);
        return false;
    }
#endif
    return true;
}

void CLog::close_file(void){
#ifdef ENABLE_LOG_FILE
    fclose(fp);
#endif
}

void CLog::puts(const char *str){
    if(open_file()){
        //fprintf(fp,str);
        fputs(str,fp);
        close_file();
    }
}

void CLog::puts(const char *str, Vector4f *color){
    if(open_file()){
        fprintf(fp,str,color->r, color->g, color->b, color->a);
        close_file();
    }
}

void CLog::printf(const char *format, ...){
    if(open_file()){
        va_list vargs;
        va_start(vargs, format);
        vfprintf(fp, format, vargs);
        va_end(vargs);
        close_file();
    }
}

