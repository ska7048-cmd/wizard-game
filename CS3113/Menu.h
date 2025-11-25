#include "Scene.h"

#ifndef MENU_H
#define MENU_H

class Menu : public Scene {
private:
    Texture2D mBackgroundImage;
    const char* mInstructions[5] = {
        "WASD or Arrow Keys - Move your character",
        "E - Pick up contraband items",
        "SPACE - Cast Lumos (light spell)",
        "Avoid Mr. Flinch and his cat Mrs. Whiskers!",
        "Collect all items to unlock the exit door"
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
