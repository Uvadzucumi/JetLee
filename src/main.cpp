
#include <ctime>

#include "game.h"

//#define SCALE_FACTOR 4

#define WIN_WIDTH  64
#define WIN_HEIGHT 64
#define WIN_TITLE  "JetLee (Pixel Craft 64)"

#define FREE_CAMERA_SPEED 30

#include "graphics.h"
#include "hero.h"
#include "camera2d.h"

#include<string>

CGameSound *sound;

bool show_bboxes=false;
bool free_cam=false;
bool play_bg_music=true;

int hero_index;
//int hero_index=HERO_FM;
//int hero_index=HERO_SM;
int hatch_flares_count=0;

CCamera2d *camera;

MyOGL::CApplication *App;
bool game_over=false;
int scale_factor;
int game_state;
// Render
void OnRender(double DeltaTime){

    MyOGL::Render->Set2D();
    MyOGL::Render->ClearScreen();

    if(game_over){
        sprites[SPRITE_CONGRATULATION]->setPosition(0,0);
        sprites[SPRITE_CONGRATULATION]->render();
    }else{
        if(!free_cam){
            camera->moveBetterViewPositionFor(heroes[hero_index]->getPosition().x, heroes[hero_index]->getPosition().y);
        }
        MyOGL::Vector2f cam_pos=camera->getPosion();

        glTranslatef(-(int)cam_pos.x * scale_factor, -(int)cam_pos.y * scale_factor,0);

        locations[current_location]->render();

        if(!hatch_flares_count && current_location==1){
            // remove hatch
            MyOGL::CUIPanel panel;
            MyOGL::Vector4f color;
            color.set(0.0,0.0,0.0,1.0);
            panel.SetBorderColor(color);
            panel.SetBackgroundColor(color);
            //locations[1]->addBlocked(19,30,3,1,TILE_EMPTY);
            panel.SetPosition(19 * LOCATION_GRID_SIZE * scale_factor, 30 * LOCATION_GRID_SIZE * scale_factor);
            panel.SetDimension(3 * LOCATION_GRID_SIZE * scale_factor, LOCATION_GRID_SIZE * scale_factor);
            panel.Render();
        }

        for(int i=0; i<3; i++){
            heroes[i]->render();
        }

        if(show_bboxes){
            BBox bbox=heroes[hero_index]->getBBox();
            MyOGL::CUIPanel panel;
            panel.SetPosition(bbox.left_top.x*scale_factor, bbox.left_top.y*scale_factor);
            panel.SetDimension(bbox.right_bottom.x*scale_factor-bbox.left_top.x*scale_factor, bbox.right_bottom.y*scale_factor-bbox.left_top.y*scale_factor);
            panel.Render();
            MyOGL::Vector2i pos=heroes[hero_index]->getPosition();
            panel.SetPosition(pos.x*scale_factor, pos.y*scale_factor);
            panel.SetDimension(1*scale_factor,1*scale_factor);
            MyOGL::Vector4f color;
            color.set(1.0,0.0,0.0,0.0);
            panel.SetBorderColor(color);
            panel.Render();

            // display down tiles by hero bbox
            /*
            MyOGL::Vector2i down={bbox.left_top.x/LOCATION_GRID_SIZE, bbox.right_bottom.y/LOCATION_GRID_SIZE};
            // std::cout << "render down " << down.x << "," << down.y << std::endl;
            int width=bbox.right_bottom.x/LOCATION_GRID_SIZE-down.x+1;

            //std::cout << "RENDER: point [" << down.x << "," << down.y << "] " << width << std::endl;

            for(int i=down.x; i<(down.x+width); i++){
                if(
                    locations[current_location]->isTile(i, down.y, TILE_WALL) ||
                    locations[current_location]->isTile(i, down.y, TILE_STAIRS)
                ){
                    sprites[SPRITE_DEBUG_PORTAL]->setPosition(
                        (i * LOCATION_GRID_SIZE) * SCALE_FACTOR,
                        (down.y * LOCATION_GRID_SIZE) * SCALE_FACTOR);
                    sprites[SPRITE_DEBUG_PORTAL]->render();
                }

            }
            */

            MyOGL::Vector2i left_top={bbox.left_top.x/LOCATION_GRID_SIZE, bbox.left_top.y/LOCATION_GRID_SIZE};
            MyOGL::Vector2i right_bottom={bbox.right_bottom.x/LOCATION_GRID_SIZE+1, bbox.right_bottom.y/LOCATION_GRID_SIZE};
            // check outside block
            for(int y=left_top.y; y < right_bottom.y; y++){
                for(int x=left_top.x; x<right_bottom.x; x++){

                    sprites[SPRITE_DEBUG_PORTAL]->setPosition(
                        (x * LOCATION_GRID_SIZE) * scale_factor,
                        (y * LOCATION_GRID_SIZE) * scale_factor);
                    sprites[SPRITE_DEBUG_PORTAL]->render();

                }
            }

        }

        if(game_state==EGameState::GAME_STATE_FAIL){
            glLoadIdentity();
            sprites[SPRITE_ENTER_TO_CONTINUE]->setPosition(
                (WIN_WIDTH*scale_factor-sprites[SPRITE_ENTER_TO_CONTINUE]->getWidth())/2,
                (WIN_HEIGHT*scale_factor-sprites[SPRITE_ENTER_TO_CONTINUE]->getHeight())/2
            );
            sprites[SPRITE_ENTER_TO_CONTINUE]->render();
        }
    }

}

void OnUpdate(double DeltaTime){
    for(int i=0; i<3; i++){
        heroes[i]->Update(DeltaTime);
    }
}

// Check KeyPressed
void OnLoop(double DeltaTime){

    if(App->IsKeyPressed(SDLK_ESCAPE)){
        App->OnExit();
    }

    if(!game_over){ // not WIN

        if(game_state==EGameState::GAME_STATE_FAIL && App->IsKeyDown(SDLK_RETURN)){
            //
            game_state=EGameState::GAME_STATE_PLAY;
            ((CHero *)heroes[hero_index])->goSpawnPoint();
        }

        if(App->IsKeyDown(SDLK_b)){
            show_bboxes=!show_bboxes;
            locations[current_location]->setDisplayBlocks(show_bboxes);
        }

        if(App->IsKeyDown(SDLK_c)){
            free_cam=!free_cam;
            if(free_cam){
                log("Free camera: On");
            }else{
                log("Free camera: Off");
            }
        }

        if(App->IsKeyDown(SDLK_s)){
            play_bg_music=!play_bg_music;
            if(play_bg_music){
                sound->play(SOUND_BG_MUSIC);
                log("Ppay BG music: On");
            }else{
                sound->pause(SOUND_BG_MUSIC);
                log("Ppay BG music: Off");
            }
        }

        if(!free_cam){
            if(App->IsKeyPressed(SDLK_LEFT) && heroes[hero_index]->isAction(HERO_ACTION_STAY)){
                heroes[hero_index]->setOrientation(ORIENTATION_LEFT);
                heroes[hero_index]->setAction(HERO_ACTION_RUN);
            }
            if(App->IsKeyPressed(SDLK_RIGHT) && heroes[hero_index]->isAction(HERO_ACTION_STAY)){
                heroes[hero_index]->setOrientation(ORIENTATION_RIGHT);
                heroes[hero_index]->setAction(HERO_ACTION_RUN);
            }

            if(App->IsKeyPressed(SDLK_UP)){
                if(
                    heroes[hero_index]->isAction(HERO_ACTION_RUN) ||
                    (heroes[hero_index]->isAction(HERO_ACTION_STAY) && (App->IsKeyPressed(SDLK_LEFT) || App->IsKeyPressed(SDLK_RIGHT)))
                ){
                    heroes[hero_index]->setAction(HERO_ACTION_JUMP);
                }else{
                    if(heroes[hero_index]->isAction(HERO_ACTION_STAY)){
                        heroes[hero_index]->setAction(HERO_ACTION_UP);
                    }
                }
            }

            if(App->IsKeyPressed(SDLK_DOWN)){
                if(heroes[hero_index]->isAction(HERO_ACTION_STAY)){
                    heroes[hero_index]->setAction(HERO_ACTION_HIDE);
                }
            }

            if(App->IsKeyPressed(SDLK_LCTRL)){

                if(heroes[hero_index]->isAction(HERO_ACTION_STAY)){
                    //std::cout << "PUSH!!!" << std::endl;
                    heroes[hero_index]->setAction(HERO_ACTION_PUSH);
                }
                else if(heroes[hero_index]->isAction(HERO_ACTION_RUN)){
                    heroes[hero_index]->setAction(HERO_ACTION_KICK);
                }
            }

            OnUpdate(DeltaTime);

        }else{ // free camera movement

                if(App->IsKeyPressed(SDLK_LEFT)){
                    camera->moveX(-10 * FREE_CAMERA_SPEED * DeltaTime);
                }
                if(App->IsKeyPressed(SDLK_RIGHT)){
                    camera->moveX(10 * FREE_CAMERA_SPEED * DeltaTime);
                }
                if(App->IsKeyPressed(SDLK_UP)){
                    camera->moveY(-10 * FREE_CAMERA_SPEED * DeltaTime);
                }
                if(App->IsKeyPressed(SDLK_DOWN)){
                    camera->moveY(10 * FREE_CAMERA_SPEED * DeltaTime);
                }

        }

    }
}

#ifdef __WIN32__
int WINAPI WinMain(HINSTANCE hInstance,
                  HINSTANCE hPrevInstance,
                  LPSTR lpCmdLine,
                  int nCmdShow){
    char* argv;
    std::vector<std::string> args;
    char* begin;
    char* current;
    argv = (char*) malloc(strlen(lpCmdLine) + 1);
    current = lpCmdLine;

    while (*current)
    {
        while (*current == ' ' || *current == '\t')
            current++;

        if (*current == '\0')
            break;
        begin = current;

        while (*current != ' ' && *current != '\t' && *current != '\0')
            current++;

        strncpy(argv, begin, current - begin);
        argv[current - begin] = '\0';

        std::string str = std::string(argv);

        args.push_back(str);

    }

#else
int main(int argc, char **argv){
    std::vector<std::string> args;
    for(int i=1; i < argc; i++){ // skip zerro parameter - application name with path
        std::string str = std::string(argv[i]);
        args.push_back(str);
    }
#endif
    srand(time(NULL));

    hero_index=2;
    scale_factor=4;
    difficulty_level=50;

    game_state=EGameState::GAME_STATE_PLAY;

    if(args.size()){

        for(int i=0; i<(int)args.size(); ++i){
            if(args[i]=="--hero" && (i+1)<(int)args.size()){
                std::cout << "hero=" << args[++i] << std::endl;
                int hero=atoi(args[i].c_str());
                switch(hero){
                    case 1:
                        std::cout << "Select Ninja for Hero!" << std::endl;
                        hero_index=0;
                        break;
                    case 2:
                        std::cout << "Select Yamo for Hero!" << std::endl;
                        hero_index=1;
                        break;
                    case 3:
                        std::cout << "Select Jet Lee for Hero!" << std::endl;
                        hero_index=2;
                        break;
                    default:
                        std::cout << "wrong hero value! (" << hero << ")" << std::endl;
                        break;
                }
            }else if(args[i]=="--scale" && (i+1) < (int)args.size()){
                int scale=atoi(args[++i].c_str());
                if(scale >= 1 && scale <= 20){
                    scale_factor=scale;
                }else{
                    std::cout << "wrong scale factor value! (" << scale << ")" << std::endl;
                }
            }else if(args[i]=="--level"){
                int level=atoi(args[++i].c_str());
                switch(level){
                    case 1:
                        difficulty_level=30;
                        std::cout << "difficulty level=Karate Kid" << std::endl;
                    break;
                    case 2:
                        difficulty_level=50;
                        std::cout << "difficulty level=Bully" << std::endl;
                    break;
                    case 3:
                        difficulty_level=100;
                        std::cout << "difficulty level=Shaolin master" << std::endl;
                    break;
                    default:
                        std::cout << "wrong difficulty level (" << level << ")" << std::endl;
                }

            }else{
                if(args[i] != "--help"){
                    std::cout << "unknown parameter: \"" << args[i] << "\"" << std::endl;
                }
                #ifdef __WIN32__
                    std::cout << "JetLee.exe [parameter [value]] game. " << std::endl;
                #else
                    std::cout << "JetLee [parameter [value]] game. " << std::endl;
                #endif
                std::cout << "--help - this help screen." << std::endl;
                std::cout << "--hero VALUE - select hero. VALUE=number for 1 to 3." << std::endl <<
                        "\t1 - Ninja.\n\t2 - Yamo.\n\t3 - JetLee (default)."  << std::endl;
                std::cout << "--scale VALUE - screen scale. VALUE=number for 1 to 20. default: 4" << std::endl;
                std::cout << "--level VALUE - difficulty level. VALUE=number for 1 to 3." << std::endl <<
                        "\t1 - Karate Kid.\n\t2 - Bully (default).\n\t3 - Shaolin master." << std::endl;
                return 0;
            }

        }

    }

    log("hero: " << hero_index);
    log("Scale factor: " << scale_factor);
    log("difficulty_level: " << difficulty_level);

    App = new MyOGL::CApplication();
    if(!App->Init(WIN_WIDTH * scale_factor, WIN_HEIGHT * scale_factor, 32, false, WIN_TITLE)){
        delete App;
        logE("Application Init Error!");
        return -1;
    };

    // init sound system
    sound=new CGameSound();
    if(!sound->init()){
        log("Sound system not initialized!");
    }else{

        if(sound->loadSounds()){
            log("loading "<<sound->getSourcesCount()<<"sounds");
        }else{
            log("load sounds error. sound disabled");
        }

    }
    // play BG music
    sound->setVolume(SOUND_BG_MUSIC,0.5);
    sound->play(SOUND_BG_MUSIC);
    // Load textures & create sprites
    if(!loadGraphics(scale_factor)){
        logE("Load graphics error!");
        delete sound;   // colose sound device
        delete App;
        return -1;
    }
    log("loaded " << textures.size() << " textures" << std::endl << "initialized " << sprites.size() << " sprites.");// << std::endl;

    initHeroObjects(scale_factor, hero_index);
    log( "initialized " << heroes.size() << " heroes." );

    initLocations(scale_factor);

    log( "initialised " << locations.size() << " locations." );

    heroes[HERO_SM]->setName("SM");
    heroes[HERO_FM]->setName("FM");
    heroes[HERO_JL]->setName("JL");

    double spawn_time=SPAWN_TIME;

    for(int i=0; i<HERO_COUNT; i++){
        if(i==hero_index){
            heroes[i]->setPosition(locations[current_location]->getHeroSpawn().x, locations[current_location]->getHeroSpawn().y);
            heroes[i]->setEnable(true);
            heroes[i]->setVisible(true);
            heroes[i]->setAction(HERO_ACTION_STAY);
        }else{
            ((CEnemy *)heroes[i])->setIndex(i);
            ((CEnemy *)heroes[i])->goSpawnPoint(spawn_time);
            spawn_time+=SPAWN_TIME;
        }
    }

    // Set Render function
    App->OnRender=OnRender;
    App->OnLoop=OnLoop;

    camera=new CCamera2d();

    // run application
    App->Run();

    delete sound;
    delete App;

    return 0;
}
