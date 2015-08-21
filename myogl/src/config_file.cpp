#include "config_file.h"

#include <stdlib.h> // for atoi
using namespace MyOGL;

#ifndef STRRSPN
/*
  A non-standard helper function:
  Find c such that s=".*c[d]*", where [d]=delimiters and c in [d].
  If c is not found, it returns the argument s.
*/
const char* strrspn( const char* s, const char* delimiters )
{
  const char* p = strchr( s, '\0' );
  while (p-- != s)
    if (!strchr( delimiters, *p ))
      break;
  return p + 1;
}
#endif

#ifndef STRNCPY0
/*
  A non-standard helper function:
  A strncpy() that guarantees termination of the result string.
  The result string must be at minimum n+1 characters long.
*/
char* strncpy0( char* result, const char* s, size_t n )
{
  strncpy( result, s, n );
  result[ n ] = '\0';
  return result;
}
#endif

char* trim_right_inplace( char* s, const char* delimiters )
{
  s[ strrspn( s, delimiters ) - s ] = '\0';
  return s;
}

char* trim_left_inplace( char* s, const char* delimiters )
{
  char* p = s + strspn( s, delimiters );
  return (char *)memmove( s, p, strlen( p ) + 1 );
}

char* trim_inplace( char* s, const char* delimiters )
{
  return trim_left_inplace( trim_right_inplace( s, delimiters ), delimiters );
}

void CConfigFile::sread_param_value(const char *buff, char *param, int *value){\
    unsigned int cnt=0,p_cnt=0,v_cnt=0;
    bool read_value=false;
    char tmp[1000];
    while(buff[cnt] && buff[cnt]!='\n'){
        // skip wrong symbols
        if(buff[cnt]==' ' || buff[cnt]=='\t'){
            cnt++;
            continue;
        }
        if(buff[cnt]==':'){
            read_value=true;
            cnt++;
            continue;
        }
        if(!read_value){
            param[p_cnt]=buff[cnt];
            p_cnt++;
        }else{
            tmp[v_cnt]=buff[cnt];
            v_cnt++;
        }
        cnt++;
    }
    param[p_cnt]=0;
    tmp[v_cnt]='\0';
    *value = atoi(tmp);
}

void CConfigFile::AddConfigParam(const char *param_name, int value){
    cfParameter tmp_param;
    tmp_param.name=new char[strlen(param_name)+1];
    strcpy(tmp_param.name,param_name);
    tmp_param.value=value;
    ParamsList.push_back(tmp_param);
}

bool CConfigFile::LoadConfig(){
    FILE *fp;
    char buff[1000], param[1000]/*, param_value[1000]*/;
    int value;

    if(!(fp=fopen(m_file_name,"rt"))){
        printf("Error open config file '%s'",m_file_name);
    }else{
        printf("--- reading config file: \"%s\" ---\n",m_file_name);
        while(!feof(fp)){
            if ( fgets (buff , 1000 , fp) != NULL ){
                // remove comments
                char *pch=strchr(buff,'#');
                if(pch!=NULL){
                    pch[0]=0;
                }
                trim_inplace(buff,"\n\r\t ");
                if(strlen(buff)==0){
                    continue;
                }
                // get parameters
                //sscanf(buff,"%s:%d\n",param,&value);
                sread_param_value(buff,param,&value);
                printf("parameter: \"%s\" value: %d\n", param, value);
                AddConfigParam(param,value);
            }else{
                break;
            }
        }
        fclose(fp);
        printf("--- end read config file ---\n");
        return true;
    }
    return false;
}

// return parameter valye by name
int CConfigFile::GetParamValue(const char *param_name, int default_value){
    for(unsigned int i=0; i<ParamsList.size(); i++){
        //printf("confirm \"%s\" and \"%s\"\n",ParamsList[i].name,param_name);
        if(!strcmp(ParamsList[i].name,param_name)){
            return ParamsList[i].value;
        }
    }
    printf("not found config param named \"%s\". used default value %d\n",param_name,default_value);
    return default_value;
}

