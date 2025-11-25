#include "MenuScene.h"

MenuScene* gMenuScene = nullptr;
//LevelA*    gLevelA    = nullptr;


MenuScene::MenuScene()                                      : Scene { {0.0f}, nullptr   } {}
MenuScene::MenuScene(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}
MenuScene::~MenuScene() { shutdown(); }

void MenuScene::initialise()
{
}

void MenuScene::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);
    if (IsKeyPressed(KEY_ENTER)) {
        mGameState.nextSceneID = 1;
    }
}

void MenuScene::render()
{
   // clear background using your helper
    ClearBackground(ColorFromHex(mBGColourHexCode));

    const char* title  = "Hogwarts Library";
    const char* prompt = "Press ENTER to start";

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    int titleSize  = 60;
    int promptSize = 25;

    int titleW  = MeasureText(title, titleSize);
    int promptW = MeasureText(prompt, promptSize);

    DrawText(title, (sw - titleW)/2, sh/3, titleSize, RAYWHITE);
    DrawText(prompt, (sw - promptW)/2, sh/3 + 80, promptSize, GRAY);
}

void MenuScene::shutdown()
{
    UnloadTexture(mBackgroundTexture);
}