#include "Menu.h"

Menu::Menu() : Scene { {0.0f}, nullptr } {}

Menu::Menu(Vector2 origin, const char *bgHexCode) 
    : Scene { origin, bgHexCode } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise()
{
    mGameState.nextSceneID = -1;

    mBackgroundImage = LoadTexture("assets/game/hauntedmansion.png");
}

void Menu::update(float deltaTime)
{ 
    if (IsKeyPressed(KEY_ENTER))
    {
        mGameState.nextSceneID = 1; 
    }
}

void Menu::render()
{
    DrawRectangle(100, 100, 200, 200, RED);
    ClearBackground(ColorFromHex(mBGColourHexCode));
    
    
    DrawTexturePro(
        mBackgroundImage,
        {0, 0, (float)mBackgroundImage.width, (float)mBackgroundImage.height}, 
        {0, 0, 1000, 600},
        {0, 0}, 
        0.0f,
        WHITE
    );

    const char* startText = "Press ENTER to start";
    
    int startFontSize = 30;
    
    int startWidth = MeasureText(startText, startFontSize);
    
    
    DrawText(
        startText,
        mOrigin.x - startWidth / 2,
        mOrigin.y + 150,
        startFontSize,
        RAYWHITE
    );
}


void Menu::shutdown()
{
    UnloadTexture(mBackgroundImage); 
}
