
#include "locations.h"
#include "game.h"

std::vector<CLocation*> locations;
int current_location=0;

void initLocations(int scale_factor){

    CLocation *loc;


    loc = new CLocation();
    loc->setSpriteIndex(SPRITE_LOCATION0);
    loc->setScaleFactor(scale_factor);
    // midle floor
    loc->addFlare(29,15);
    loc->addFlare(40,15);
    // bottom floor
    loc->addFlare( 3,24);
    loc->addFlare(22,24);
    loc->addFlare(40,24);
    // upper floor
    loc->addBlocked(0,12,15);
    loc->addBlocked(18,12,6);
    loc->addBlocked(29,12,13);
    // midle floor
    loc->addBlocked(0,19,4);
    loc->addBlocked(6,21,5);
    loc->addBlocked(22,21,20);
    // ground floor
    loc->addBlocked(0,30,42);
    // walls
    loc->addBlocked(0,15,1,15); // left
    loc->addBlocked(41,22,1,8); // right
    loc->addBlocked(1,8,3,4); // peyote
    // stairs
    loc->addBlocked(15,12,3,14,TILE_STAIRS); // stair

    //loc->addBlocked(41,6,1,6,TILE_PORTAL); // portal
    loc->addPortal(41, 6, 1, 6, 1, MyOGL::Vector2i(8,48), MyOGL::Vector2i(8,48));
    //loc->addBlocked(41,15,1,6,TILE_PORTAL); // portal
    loc->addPortal(41, 15, 1, 6, 1, MyOGL::Vector2i(8,84), MyOGL::Vector2i(8,48));
    // start game spawn points
    loc->setHeroSpawn(52,48);
    loc->setEnemySpawn(156,48);

    std::cout << "location: " << locations.size() << " flares: " << loc->getFlaresCount() << std::endl;
    hatch_flares_count+=loc->getFlaresCount();

    locations.push_back(loc); // END LOCATION0

//////////////////////////////////////// LOCATION 1 //////////////////////////////////////////////////////////////////////
    loc = new CLocation();
    loc->setSpriteIndex(SPRITE_LOCATION1);
    loc->setScaleFactor(scale_factor);

    // upper floor
    loc->addBlocked(0,12,10);
    loc->addBlocked(15,12,4);
    loc->addBlocked(22,12,4);
    loc->addBlocked(31,12,11);

    // middle floor
    loc->addBlocked(0,21,15);
    loc->addBlocked(26,21,16);

    // ground floor
    loc->addBlocked(0,30,42);

    // walls
    loc->addBlocked(0,24,1,6); // left
    loc->addBlocked(41,15,1,15); // right

    //stairs
    loc->addBlocked(19,12,3,5,TILE_STAIRS); // stair
    loc->addBlocked(19,21,3,5,TILE_STAIRS); // stair

    // portals
    loc->addPortal(0, 6, 1, 6, 0, MyOGL::Vector2i(156, 48), MyOGL::Vector2i(156,48));
    loc->addPortal(0, 15, 1, 6, 0, MyOGL::Vector2i(156, 84), MyOGL::Vector2i(156,48));
    loc->addPortal(41, 6, 1, 6, 2, MyOGL::Vector2i(8, 48), MyOGL::Vector2i(8,48));

    loc->addFlare(9,15);
    loc->addFlare(31,15);

    loc->addFlare(3,24);
    loc->addFlare(14,24);
    loc->addFlare(26,24);
    loc->addFlare(38,24);

    std::cout << "location: " << locations.size() << " flares: " << loc->getFlaresCount() << std::endl;
    hatch_flares_count+=loc->getFlaresCount();

    locations.push_back(loc);

////////////////////// location 2 ////////////////////////////////////////////////////////////////////////////////////////////

    loc = new CLocation();
    loc->setSpriteIndex(SPRITE_LOCATION2);
    loc->setScaleFactor(scale_factor);

    // upper floor
    loc->addBlocked(0,12,10);
    loc->addBlocked(14,12,3);
    loc->addBlocked(20,12,7);
    loc->addBlocked(30,12,12);

    // middle floor
    loc->addBlocked(0,21,5);
    loc->addBlocked(9,21,7);
    loc->addBlocked(21,21,7);
    loc->addBlocked(34,21,5);

    // ground floor
    loc->addBlocked(0,30,42);

    // walls
    loc->addBlocked(0,15,1,15);
    loc->addBlocked(41,15,1,15);
    loc->addBlocked(31,15,1,7);
    loc->addBlocked(35,8,3,4); // peyote

    // stairs
    loc->addBlocked(17,12,3,14,TILE_STAIRS); // stair

    // portals
    loc->addPortal(0, 6, 1, 6, 1, MyOGL::Vector2i(156, 48), MyOGL::Vector2i(156,48));

    loc->addFlare(4, 15);
    loc->addFlare(9, 15);
    loc->addFlare(23, 15);
    loc->addFlare(26, 15);
    loc->addFlare(30, 15);

    loc->addFlare(4, 24);
    loc->addFlare(9, 24);
    loc->addFlare(15, 24);
    loc->addFlare(21, 24);
    loc->addFlare(27, 24);

    std::cout << "location: " << locations.size() << " flares: " << loc->getFlaresCount() << std::endl;
    hatch_flares_count+=loc->getFlaresCount();

    locations.push_back(loc);

}
