#include "MenuScene.h"

#ifndef LEVELA_H
#define LEVELA_H

constexpr int LEVEL_WIDTH = 20,
              LEVEL_HEIGHT = 15;



class LevelA : public Scene {
private:
    Texture2D mBackgroundTexture;

    Entity* mMrFlinch;
    Entity* mCat;   

    Entity* mItems[5];
    bool mItemCollected[5] = {false, false, false, false, false};
    int mItemsCollectedCount = 0;
    bool mShowExitPrompt = false;

    
    Sound mPickupSound;
    Sound mSpottedSound;
    Sound mLevelCompleteSound;

    static constexpr int TOTAL_ITEMS = 5;
    
    unsigned int mLevelData[LEVEL_WIDTH * LEVEL_HEIGHT] = {
        3, 3, 3, 3, 3, 3, 3, 3, 3, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 5 is door
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, // 4 is table
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 2, 2, 3, // 3 is bookshelf
        3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 3, // 2 is tile
        3, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 2, 2, 3, 2, 2, 4, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 3,
        3, 2, 2, 4, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 2, 3, 2, 4, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 3, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 2, 2, 2, 3, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    };
public:
    static constexpr float TILE_DIMENSION       = 50.0f;
                        //ACCELERATION_OF_GRAVITY = 981.0f,
                        //END_GAME_THRESHOLD      = 800.0f;

    LevelA();
    LevelA(Vector2 origin, const char *bgHexCode);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

    void checkItems();
    void checkDoorExit();
    void drawScreen();
};

#endif