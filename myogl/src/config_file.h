#ifndef CONFIG_FILE_H_INCLUDED
#define CONFIG_FILE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <vector>

namespace MyOGL{

typedef struct{
    char *name;
    int value;
}cfParameter;

class CConfigFile{
        char *m_file_name;
        std::vector <cfParameter> ParamsList;
        void sread_param_value(const char *buff, char *param, int *value);
    public:
        CConfigFile(const char *file_name){
            m_file_name=new char[strlen(file_name)+1];
            strcpy(m_file_name,file_name);
            LoadConfig();
        }
        ~CConfigFile(){
            for(unsigned int i=0;i<ParamsList.size();i++){
                // clear param names memory
                delete ParamsList[i].name;
            }
            ParamsList.clear();
            delete m_file_name;
        }
        bool LoadConfig();
        int GetParamValue(const char *param_name, int default_value);
        void AddConfigParam(const char *param_name, int value);
};

} // namespace end;

#endif // CONFIG_FILE_H_INCLUDED
