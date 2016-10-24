
#include "locations.h"
#include "game.h"

#include "json/json.h"

std::vector<CLocation*> locations;
int current_location=0;

bool initLocations(int scale_factor){

    CLocation *loc;

    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;

    // read JSON file to string
    std::ifstream t("data/locations.json");
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    std::cout << buffer << std::endl;


    bool parsingSuccessful = reader.parse( buffer, root );
    if ( !parsingSuccessful )
    {
        // report to the user the failure and their locations in the document.
        std::cout  << "Failed to parse locations.json\n"
                   << reader.getFormattedErrorMessages();
        return false;
    }

    const Json::Value locations_list = root["locations"];
    for(int i=0; i<locations_list.size(); ++i){
        const Json::Value location = locations_list[i];
        // get location

        std::cout << "parse location [" << i << "]\n";

        int location_id  =  location["location"].asInt();
        int sprite_index = location["sprite_index"].asInt();

        loc = new CLocation();
        loc->setSpriteIndex(sprite_index);
        loc->setScaleFactor(scale_factor);


        const Json::Value blocks = location["blocks"];
        std::cout << "found " << blocks.size() << " blocks\n";
        for(int j=0; j<blocks.size(); ++j){
            int x = blocks[j].get("x",0).asInt();
            int y = blocks[j].get("y",0).asInt();
            int w = blocks[j].get("w",0).asInt();
            int h = blocks[j].get("h",0).asInt();
            std::string type=blocks[j].get("type","").asString();
            int block_type = EBlockTypes::_from_string(type.c_str());
            std::cout << "block [" << j<< "] x: " << x << " y: " << y << " w: " << w << " h: " << h << " type: " << block_type << " (" << type << ") " << std::endl;
            loc->addBlock(x, y, w, h, block_type);
        }

        const Json::Value flares = location["flares"];
        std::cout << "found " << flares.size() << " flares\n";

        for(int j=0; j<flares.size(); ++j){
            int x = flares[j].get("x",0).asInt();
            int y = flares[j].get("y",0).asInt();
            std::cout << "flare [" << j<< "] x: " << x << " y: " << y << std::endl;
            loc->addFlare(x,y);
        }

        hatch_flares_count+=flares.size();

        const Json::Value portals = location["portals"];
        std::cout << "found " << portals.size() << " portals\n";

        for(int j=0; j<portals.size(); ++j){
            int x = portals[j].get("x",0).asInt();
            int y = portals[j].get("y",0).asInt();
            int w = portals[j].get("w",0).asInt();
            int h = portals[j].get("h",0).asInt();
            int warp_location = portals[j].get("warp_location",0).asInt();
            MyOGL::Vector2i hero_spawn(portals[j]["hero_spawn"].get("x",0).asInt(),portals[j]["hero_spawn"].get("y",0).asInt());
            MyOGL::Vector2i enemy_spawn(portals[j]["enemy_spawn"].get("x",0).asInt(),portals[j]["enemy_spawn"].get("y",0).asInt());
            std::string type=blocks[j].get("type","").asString();
            std::cout << "portal [" << j<< "] x: " << x << " y: " << y << " w: " << w << " h: " << h << " warp to: " << \
               warp_location << " h spawn: " << hero_spawn << "e spawn: " << enemy_spawn << std::endl;

            loc->addPortal(x, y, w, h, warp_location, hero_spawn, enemy_spawn);
        }

        // set default spawns if founded

        MyOGL::Vector2i hero_spawn(location["hero_spawn"].get("x",0).asInt(),location["hero_spawn"].get("y",0).asInt());
        MyOGL::Vector2i enemy_spawn(location["enemy_spawn"].get("x",0).asInt(),location["enemy_spawn"].get("y",0).asInt());

        loc->setHeroSpawn(hero_spawn.x, hero_spawn.y);
        loc->setEnemySpawn(enemy_spawn.x, enemy_spawn.y);

        std::cout << "defaul spawn h: " << hero_spawn << " default spawn e: " << enemy_spawn << std::endl;

        locations.push_back(loc);

    }

    return true;

}