
#include "entity.h"

using namespace MyOGL;

std::vector<CEntity*> MyOGL::EntityList;

CEntity::CEntity() {
    m_texture = NULL;
    X = Y = 0.0f;
    Width = Height = 0;
    AnimState = 0;
    m_animation=new CAnimation;
    EntityList.push_back(this);
    Log->printf("created entity %d!\n",EntityList.size());
}

CEntity::~CEntity() {
    Log->puts("Starting delete Entity\n");
    free();
    // remove from entity list
    // serach ad remove from entity list
    for(unsigned int i=0;i<EntityList.size();i++){
        if(EntityList[i]==this){
            EntityList.erase(EntityList.begin()+i);
            break;
        }
    }
    Log->puts("Finished delete Entity\n");
}

bool CEntity::onCreate(CTexture* texture, int Width, int Height, int MaxFrames, CMaterial *material) {
    if(texture==NULL){
        Log->puts("CEntity::OnCreate Error: NULL texture\n");
        return false;
    }
    m_material=material;
    m_texture=texture;
    this->Width = Width;
    this->Height = Height;
    m_animation->FramesCount = MaxFrames;
    //
    int dx=0;
    int dy=0;
    for(int i=0;i<MaxFrames;i++){
        // create sprites
        CHudSprite *sprite=new CHudSprite(texture, material);
        SpritesList.push_back(sprite);
        // calculate text coords
        sprite->setUVPixelCoords(dx, dy, Width, Height);
        sprite->setSize(Width, Height);
        dx+=Width;
        if(dx>=texture->GetWidth()){
            dx=0;
            dy+=Height;
            if(dy>=texture->GetHeight() && i!=(MaxFrames-1)){
                Log->puts("CEntity::OnCreate Error: Wrong MaxFrames  or Texture parameter. ");
                Log->printf("(dx: %d dy: %d)\n", dx, dy);
                return false;
            }
        }
    }
    Log->printf("CEntity. Created %d frames\n",SpritesList.size());
    return true;
}

void CEntity::onLoop(double DeltaTime) {
    m_animation->onAnimate(DeltaTime);
}

void CEntity::onRender() {
    if(SpritesList.size()!=0){
        if(m_material!=NULL){
            m_material->Apply();
        }else{
            MyOGL::Render->SetColor(1.0f,1.0f,1.0f);
        }
        if((int)SpritesList.size() > m_animation->getCurrentFrame()){
            SpritesList[m_animation->getCurrentFrame()]->setPosition(X,Y);
            SpritesList[m_animation->getCurrentFrame()]->render();
        }else{
            Log->printf("CEntity::OnRender() Error: Wrong Frame Number %d\n",m_animation->getCurrentFrame());
        }
    }
}

void CEntity::free() {
    // delete all sprites
    for(unsigned int i=0;i<SpritesList.size();i++){
        if(SpritesList[i]){
            delete SpritesList[i];
        }
    }
    if(m_animation!=NULL){
        delete m_animation;
    }
    SpritesList.clear();
    Log->puts("Free entity\n");
}
