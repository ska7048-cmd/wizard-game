#ifndef WINSCENE_H
#define WINSCENE_H

#include "Scene.h"

class WinScene : public Scene {
private:
    Texture2D mBackgroundTexture;  // Add this!
    
public:
    WinScene();
    WinScene(Vector2 origin, const char *bgHexCode);
    ~WinScene();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
