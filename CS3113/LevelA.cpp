#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
   mGameState.nextSceneID = -1;

   mGameState.bgm = LoadMusicStream("assets/game/classichorror1.mp3");
   mGameState.fallingSound = LoadSound("assets/game/Falling.mp3");
   SetMusicVolume(mGameState.bgm, 0.53f);
   // PlayMusicStream(gState.bgm);

   mGameState.jumpSound = LoadSound("assets/game/jumpland.wav");
   mLevelCompleteSound = LoadSound("assets/game/level_complete.mp3");
   mBackgroundTexture = LoadTexture("assets/game/background.png");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/game/tileset.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      8, 1,                        // texture cols & rows
      mOrigin                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
      {DOWN,  { 0, 1, 2 }},       
      {LEFT,  { 3, 4, 5 }},       
      {RIGHT, { 6, 7, 8 }},       
      {UP,    { 9, 10, 11 }},     
   };

   std::map<Direction, std::vector<int>> enemyAnimationAtlas = {
      {DOWN,  { 0, 1, 2, 3 }},     
      {LEFT,  { 4, 5, 6, 7 }},     
      {RIGHT, { 8, 9, 10, 11 }},   
      {UP,    { 12, 13, 14, 15 }}   
   };


   float sizeRatio  = 50.0f / 64.0f;
   

   // Assets from @see https://sscary.itch.io/the-adventurer-female
   mGameState.xochitl = new Entity(
      {mOrigin.x, mOrigin.y - 350.0f}, // position
      {70.0f * sizeRatio, 75.0f},             // scale
      "assets/game/witch.jpg",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4.6, 3 },                                 // atlas dimensions
      xochitlAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.xochitl->setJumpingPower(550.0f);
   mGameState.xochitl->setColliderDimensions({
      mGameState.xochitl->getScale().x / 0.5f,
      mGameState.xochitl->getScale().y / 1.0f
   });
   mGameState.xochitl->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   mEnemy = new Entity(
      {mOrigin.x + 250.0f, mOrigin.y - 100.0f},
      {40.0f, 40.0f},  
      "assets/game/spooky.jpg",  
      ATLAS,  
      {4, 4},  
      enemyAnimationAtlas, 
      ENEMY  
   );
   mEnemy->setAIType(FOLLOWER);
   mEnemy->setAIState(IDLE);
   mEnemy->setSpeed(50); 
   mEnemy->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

   /*
      ----------- CAMERA -----------
   */
   mGameState.camera = { 0 };                                    // zero initialize
   mGameState.camera.target = mGameState.xochitl->getPosition(); // camera follows player
   mGameState.camera.offset = mOrigin;                           // camera offset to center of screen
   mGameState.camera.rotation = 0.0f;                            // no rotation
   mGameState.camera.zoom = 1.0f;                                // default zoom
}

void LevelA::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);
   extern int gPlayerLives;

   mGameState.xochitl->update(
      deltaTime,      // delta time / fixed timestep
      nullptr,        // player
      mGameState.map, // map
      nullptr,        // collidable entities
      0               // col. entity count
   );

   mEnemy->update(deltaTime, mGameState.xochitl, mGameState.map, nullptr, 0);

   if (mGameState.xochitl->checkCollision(mEnemy)) {
      PlaySound(mGameState.fallingSound);
      gPlayerLives--;
      
      if (gPlayerLives <= 0) {
         mGameState.nextSceneID = 5;  
      } else {
         mGameState.nextSceneID = 1;  
      }
      return;
   }

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mGameState.xochitl->getPosition().y };

   if (mGameState.xochitl->getPosition().y > 800.0f) {
      PlaySound(mLevelCompleteSound);
      mGameState.nextSceneID = 2;
      return;
   }
   
   panCamera(&mGameState.camera, &currentPlayerPosition);

   Vector2 playerPos = mGameState.xochitl->getPosition();

   float feetY = playerPos.y + (mGameState.xochitl->getScale().y / 2.0f) + 5.0f;  // i had to get help with this
   int playerTileX = (int)((playerPos.x - mOrigin.x + (LEVEL_WIDTH * TILE_DIMENSION) / 2) / TILE_DIMENSION);
   int playerTileY = (int)((feetY - mOrigin.y + (LEVEL_HEIGHT * TILE_DIMENSION) / 2) / TILE_DIMENSION);

   if (playerTileX >= 0 && playerTileX < LEVEL_WIDTH && 
      playerTileY >= 0 && playerTileY < LEVEL_HEIGHT) {
      
      int tileIndex = playerTileY * LEVEL_WIDTH + playerTileX;
      int tileType = mLevelData[tileIndex];

      if (tileType == 2) {
         gPlayerLives--;
         
         if (gPlayerLives <= 0) {
               mGameState.nextSceneID = 5;  
         } else {
               mGameState.nextSceneID = 1;  
         }
      }
   }
}

void LevelA::render()
{
   float parallaxSpeed = 0.3f;
   float bgOffsetX = -mGameState.camera.target.x * parallaxSpeed;
   float bgOffsetY = -mGameState.camera.target.y * parallaxSpeed * 0.2f;

   DrawTexturePro(
        mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},
        {0, 0, 1000, 600}, 
        {0, 0},
        0.0f,
        WHITE
   );
   DrawTexturePro(
        mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},
        {bgOffsetX - 500, -300, 2000, 1200}, 
        {0, 0},
        0.0f,
        WHITE
   );

   ClearBackground(ColorFromHex(mBGColourHexCode));

   mGameState.xochitl->render();
   mEnemy->render();
   mGameState.map->render();
}

void LevelA::shutdown()
{
   
   delete mGameState.xochitl;
   delete mGameState.map;
   delete mEnemy;

   UnloadTexture(mBackgroundTexture);
   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mLevelCompleteSound);
   UnloadSound(mGameState.fallingSound);
}