#include "Scene.h"

#ifndef MENU_H
#define MENU_H

class Menu : public Scene {
private:
    Texture2D mBackgroundImage;
    const char* mInstructions[5] = {
        "WASD or Arrow Keys - Move your character",
        "Walk over contraband items to pick them up",
        "You cast Lumos (light spell) in order to see!",
        "Avoid Mr. Filch and his cat Mrs. Norris!",
        "Collect all items to unlock the exit door by pressing - E"
    };
    int mCurrentInstructionIndex = 0;
    int mTotalInstructions = 5;
    bool mInstructionsComplete = false;
    
public:
    Menu();
    Menu(Vector2 origin, const char *bgHexCode);
    ~Menu();
    
    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif // MENU_H
