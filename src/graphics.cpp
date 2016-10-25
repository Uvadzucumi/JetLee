
#include "graphics.h"
#include "json/json.h"

std::vector<MyOGL::CTexture*> textures;
std::vector<MyOGL::CHudSprite*> sprites;

bool loadGraphics(int sprite_scale){
    std::string textures_path="data/";
    //std::vector<std::string> texture_files;

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;

    // read json file
    // read JSON file to string
    std::ifstream t("data/graphics.json");
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    bool parsingSuccessful = reader.parse( buffer, root );
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse graphics.json\n"
        << reader.getFormattedErrorMessages();
        return false;
    }

    // create textures
    const Json::Value texture_files = root["textures"];
    std::cout << "need load " << texture_files.size() << " textures.\n";
    for(int i=0; i<texture_files.size(); ++i){
        MyOGL::CTexture *tex=new MyOGL::CTexture();
        tex->setMinFilter(GL_NEAREST);
        tex->setMagFilter(GL_NEAREST);
        std::string file_name=textures_path+texture_files[i].asString();
        if(!tex->LoadFromFile(file_name.c_str())){
            std::cout << "Graphics load error! in: " <<  texture_files[i] << std::endl;
            return false;
        }
        textures.push_back(tex);
    }

    MyOGL::CHudSprite *sprite;
    const Json::Value sprites_info = root["sprites"];
    for(int i=0; i<sprites_info.size(); ++i){
        std::string name=sprites_info[i].get("name","").asString();
        int texture_index = sprites_info[i].get("texture",0).asInt();
        int left=sprites_info[i].get("l",0).asInt();
        int top=sprites_info[i].get("t",0).asInt();
        int right=sprites_info[i].get("r",0).asInt();
        int bottom=sprites_info[i].get("b",0).asInt();

        sprite=new MyOGL::CHudSprite(textures[texture_index]);
        sprite->setUVPixelCoords(left, top, right - left, bottom - top);
        sprite->setSize( (right - left) * sprite_scale, (bottom - top) * sprite_scale);
        sprites.push_back(sprite);
    }

    return true;

}