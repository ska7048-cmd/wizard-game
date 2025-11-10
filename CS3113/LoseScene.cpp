#include "LoseScene.h"

LoseScene::LoseScene() : Scene { {0.0f, 0.0f}, nullptr } {}
LoseScene::LoseScene(Vector2 origin, const char *bgHexCode) 
    : Scene { origin, bgHexCode } {}
LoseScene::~LoseScene() { shutdown(); }

void LoseScene::initialise()
{
    mGameState.nextSceneID = -1;
    
    mBackgroundTexture = LoadTexture("assets/game/lose.png");
}

void LoseScene::update(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER)) {
        mGameState.nextSceneID = 0;
    }
}

void LoseScene::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    

    if (mBackgroundTexture.id > 0) {
        DrawTexturePro(
            mBackgroundTexture,
            {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},
            {0, 0, 1000, 600}, 
            {0, 0},
            0.0f,
            WHITE
        );
    }
    const char* promptText = "Press ENTER to return to main menu";
    int promptSize = 30;
    int promptWidth = MeasureText(promptText, promptSize);
    DrawText(promptText, (GetScreenWidth() - promptWidth) / 2, GetScreenHeight() / 2 + 200.f, promptSize, RAYWHITE);

}
void LoseScene::shutdown() 
{
    if (mBackgroundTexture.id > 0) {
        UnloadTexture(mBackgroundTexture);
    }
}
        