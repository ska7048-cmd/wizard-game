#include "CS3113/Menu.h" 
#include "CS3113/LevelA.h" 
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/WinScene.h" 
#include "CS3113/LoseScene.h" 

// Global Constants
constexpr int SCREEN_WIDTH     = 1000,
              SCREEN_HEIGHT    = 600,
              FPS              = 120,
              NUMBER_OF_LEVELS = 6;
              

constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

            
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus   = RUNNING;
int gPlayerLives = 3;

float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

Menu *gMenu = nullptr; 
LevelA *gLevelA = nullptr;
LevelB *gLevelB = nullptr;
LevelC *gLevelC = nullptr;
WinScene *gWinScene = nullptr;
LoseScene *gLoseScene = nullptr;

// Function Declarations
void switchToScene(Scene *scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene *scene)
{   
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "haunted mansion");
    InitAudioDevice();

    gMenu = new Menu(ORIGIN, "#1a1a2e");
    gLevelA = new LevelA(ORIGIN, "#3b2f2cff");
    gLevelB = new LevelB(ORIGIN, "#011627");
    gLevelC = new LevelC(ORIGIN, "#2c1810");
    gWinScene = new WinScene(ORIGIN, "#006400");   
    gLoseScene = new LoseScene(ORIGIN, "#8B0000"); 

    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);    
    gLevels.push_back(gWinScene); 
    gLevels.push_back(gLoseScene); 

    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (gCurrentScene == gMenu || gCurrentScene == gWinScene || gCurrentScene == gLoseScene)
    {
        if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
        return;  
    }

    gCurrentScene->getState().xochitl->resetMovement();

    if      (IsKeyDown(KEY_A)) gCurrentScene->getState().xochitl->moveLeft();
    else if (IsKeyDown(KEY_D)) gCurrentScene->getState().xochitl->moveRight();

    if (IsKeyPressed(KEY_W) && 
        gCurrentScene->getState().xochitl->isCollidingBottom())
    {
        gCurrentScene->getState().xochitl->jump();
        PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(gCurrentScene->getState().xochitl->getMovement()) > 1.0f) 
        gCurrentScene->getState().xochitl->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    if (gCurrentScene != gMenu && gCurrentScene != gWinScene && gCurrentScene != gLoseScene)
    {
        BeginMode2D(gCurrentScene->getState().camera);
    }

    gCurrentScene->render();

    if (gCurrentScene != gMenu && gCurrentScene != gWinScene && gCurrentScene != gLoseScene)
    {
        EndMode2D();
    
        char livesText[20];
        sprintf(livesText, "Lives: %d", gPlayerLives);
        DrawText(livesText, 20, 20, 30, RED);
    }
    

    EndDrawing();
}

void shutdown() 
{
    delete gMenu;
    delete gLevelA;
    delete gLevelB;
    delete gLevelC;
    delete gWinScene;
    delete gLoseScene;

    
    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();

        int nextID = gCurrentScene->getState().nextSceneID;
        
        if (nextID >= 0 && nextID < NUMBER_OF_LEVELS)
        {
            
            if (nextID == 0) {
                gPlayerLives = 3;
            }
            
            switchToScene(gLevels[nextID]);
        }

        render();
    }

    shutdown();

    return 0;
}