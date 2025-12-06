#include "Menu.h"

Menu::Menu() : Scene { {0.0f}, nullptr } {}

Menu::Menu(Vector2 origin, const char *bgHexCode) 
    : Scene { origin, bgHexCode } {}

Menu::~Menu() { shutdown(); }

void Menu::initialise()
{
    mBackgroundImage = LoadTexture("assets/game/library.png");
    mGameState.nextSceneID = -1;
    mCurrentInstructionIndex = 0;
    mInstructionsComplete = false;
}

void Menu::update(float deltaTime)
{ 
    if (IsKeyPressed(KEY_SPACE) && !mInstructionsComplete)
    {
        mCurrentInstructionIndex++;
        if (mCurrentInstructionIndex >= mTotalInstructions){ mInstructionsComplete = true;}
    }
    if (IsKeyPressed(KEY_ENTER) && mInstructionsComplete){ mGameState.nextSceneID = 1;}
}


void Menu::render()
{
    DrawTexturePro(
        mBackgroundImage,
        {0, 0, (float)mBackgroundImage.width, (float)mBackgroundImage.height}, 
        {0, 0, 1000, 600},
        {0, 0}, 
        0.0f,
        WHITE
    );

    int boxWidth = 720;
    int boxHeight = 160;
    int screenCenterX = 500;
    int boxX = screenCenterX - boxWidth / 2;
    int boxY = 350;


    DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.70f));
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, RAYWHITE);

    if (!mInstructionsComplete)
    {
        const char* currentInstruction = mInstructions[mCurrentInstructionIndex];
        int textWidth = MeasureText(currentInstruction, 26);
        
        DrawText(
            currentInstruction,
            screenCenterX - textWidth / 2,
            boxY + 70,
            26,
            RAYWHITE
        );
        
        // Prompt
        const char* prompt = "Press SPACE for next";
        int promptWidth = MeasureText(prompt, 22);
        DrawText(prompt, screenCenterX - promptWidth / 2, boxY + boxHeight + 35, 22, GRAY);
    }
    else
    {
        const char* readyText = "Good luck escaping the library!";
        int readyWidth = MeasureText(readyText, 28);
        DrawText(readyText, screenCenterX - readyWidth / 2, boxY + 65, 28, RAYWHITE);
        
        const char* startText = "Press ENTER to begin";
        int startWidth = MeasureText(startText, 26);
        DrawText(startText, screenCenterX - startWidth / 2, boxY + boxHeight + 35, 26, GRAY);
    }

   
}


void Menu::shutdown()
{
    //UnloadTexture(mBackgroundImage); 
}
