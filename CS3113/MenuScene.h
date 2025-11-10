#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"

class MenuScene : public Scene {
    Texture2D mBackgroundTexture;
public:
    MenuScene();
    MenuScene(Vector2 origin, const char *bgHexCode);
    ~MenuScene();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};


#endif 