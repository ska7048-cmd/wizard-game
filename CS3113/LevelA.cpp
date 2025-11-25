#include "LevelA.h"

LevelA::LevelA()                                      : Scene { {0.0f}, nullptr   } {}
LevelA::LevelA(Vector2 origin, const char *bgHexCode) : Scene { origin, bgHexCode } {}

LevelA::~LevelA() { shutdown(); }

void LevelA::initialise()
{
   mGameState.nextSceneID = -1;

   mGameState.bgm = LoadMusicStream("assets/game/bluefeather.mp3");
   mPickupSound = LoadSound("assets/game/life_pickup.flaq");
   mSpottedSound = LoadSound("assets/game/spotted.mp3");
   mLevelCompleteSound = LoadSound("assets/audio/level_complete.wav");
   
   SetMusicVolume(mGameState.bgm, 0.5f);
   PlayMusicStream(mGameState.bgm);

   const char* itemImages[5] = {
      "assets/items/item1.png",
      "assets/items/item2.png",
      "assets/items/item3.png",
      "assets/items/item4.png",
      "assets/items/item5.png"
   };

   
   // Load background
   // mBackgroundTexture = LoadTexture("assets/sprites/library_bg.png");

   /*
      ----------- MAP -----------
   */
   mGameState.map = new Map(
      LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
      (unsigned int *) mLevelData, // grid data
      "assets/game/tileset.png",   // texture filepath
      TILE_DIMENSION,              // tile size
      5, 1,                        // texture cols & rows
      {500.0f, 300.0f}                      // in-game origin
   );

   /*
      ----------- PROTAGONIST -----------
   */
   std::map<Direction, std::vector<int>> xochitlAnimationAtlas = {
      {DOWN,  { 0, 1, 2, 3 }},       
      {RIGHT,  { 4, 5, 6, 7 }},       
      {LEFT, { 8, 9, 10, 11}},       
      {UP,    { 12, 13, 14, 15}},     
   };

   mGameState.xochitl = new Entity(
      {500.0f, 400.0f}, // position
      {50.0f, 50.0f},             // scale
      "assets/game/harry.jpeg",                   // texture file address
      ATLAS,                                    // single image or atlas?
      { 4, 4 },                                 // atlas dimensions
      xochitlAnimationAtlas,                    // actual atlas
      PLAYER                                    // entity type
   );

   mGameState.xochitl->setSpeed(150);

   /*
      ----------- MR FLINCH -----------
   */
   //Mr Flinch
   std::map<Direction, std::vector<int>> flinchAnimationAtlas = {
      {DOWN,  { 0, 1, 2 }},     
      {LEFT,  { 3, 4, 5 }},     
      {RIGHT, { 6, 7, 8 }},   
      {UP,    { 9, 10, 11 }}   
   };


  
   mMrFlinch = new Entity(
      {300.0f, 200.0f},
      {40.0f, 40.0f},  
      "assets/game/mrflinch.png",  
      ATLAS,  
      {3, 4},  
      flinchAnimationAtlas, 
      ENEMY  
   );
   mMrFlinch->setAIType(WANDERER);
   mMrFlinch->setAIState(WALKING);
   mMrFlinch->setSpeed(50); 


   /*
      ----------- CAT -----------
   */
   std::map<Direction, std::vector<int>> catAnimationAtlas = {
      {DOWN,  { 0, 1, 2, 3 }},     
      {LEFT,  { 0, 1, 2, 3 }},     
      {RIGHT, { 0, 1, 2, 3 }},   
      {UP,    { 0, 1, 2, 3 }}   
   };


  
   mCat = new Entity(
      {700.0f, 300.0f},
      {40.0f, 40.0f},  
      "assets/game/cat.png",  
      ATLAS,  
      {4, 1},  
      catAnimationAtlas, 
      ENEMY  
   );
   mCat->setAIType(FOLLOWER);
   mCat->setAIState(IDLE);
   mCat->setSpeed(50); 



   /*
      ----------- ITEMS -----------
   */

   Vector2 itemPositions[5] = {
      {200.0f, 150.0f}, 
      {800.0f, 150.0f}, 
      {350.0f, 300.0f}, 
      {650.0f, 300.0f},
      {500.0f, 450.0f}  
   };
   for (int i = 0; i <TOTAL_ITEMS; i++){
      mItems[i] = new Entity(
         itemPositions[i],
         {25.0f, 25.0f},
         itemImages[i],
         NPC
      );
      mItemCollected[i] = false;
   }
   mItemsCollectedCount = 0;
}

void LevelA::update(float deltaTime)
{
   UpdateMusicStream(mGameState.bgm);
   extern int gPlayerLives;

   mGameState.xochitl->resetMovement();

   if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
      mGameState.xochitl->moveUp();
   }
   if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
      mGameState.xochitl->moveDown();
   }
   if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
      mGameState.xochitl->moveLeft();
   }
   if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
      mGameState.xochitl->moveRight();
   }

   mGameState.xochitl->update(deltaTime, nullptr, mGameState.map, nullptr, 0);  
   mMrFlinch->update(deltaTime, mGameState.xochitl, mGameState.map, nullptr, 0);
   mCat->update(deltaTime, mGameState.xochitl, mGameState.map, nullptr, 0);

   if (mGameState.xochitl->checkCollision(mMrFlinch) || mGameState.xochitl->checkCollision(mCat)) {
      PlaySound(mSpottedSound);
      gPlayerLives--;
      
      if (gPlayerLives <= 0) {
         mGameState.nextSceneID = 5;  
      } else {
         mGameState.nextSceneID = 1;  
      }
      return;
   }

   Vector2 currentPlayerPosition = { mGameState.xochitl->getPosition().x, mGameState.xochitl->getPosition().y };

   checkItems();
   checkDoorExit();
}

void LevelA::checkItems(){
   for ( int i = 0; i < TOTAL_ITEMS; i++) {
      if(!mItemCollected[i] && mGameState.xochitl->checkCollision(mItems[i])){
         mItemCollected[i] = true;
         mItems[i]->deactivate(); 
         mItemsCollectedCount++;
         PlaySound(mPickupSound);
      }
   }
}

void LevelA::checkDoorExit(){

   Vector2 playerPos = mGameState.xochitl->getPosition();
   Vector2 mapOrigin = {500.0f, 300.0f};

   int playerTileX = (int)((playerPos.x - mapOrigin.x + (LEVEL_WIDTH * TILE_DIMENSION) / 2) / TILE_DIMENSION);
   int playerTileY = (int)((playerPos.y - mapOrigin.y + (LEVEL_HEIGHT * TILE_DIMENSION) / 2) / TILE_DIMENSION);

   if (playerTileX >= 0 && playerTileX < LEVEL_WIDTH && playerTileY >= 0 && playerTileY < LEVEL_HEIGHT) {
      int tileIndex = playerTileY * LEVEL_WIDTH + playerTileX;
      int tileType = mLevelData[tileIndex];

      if (tileType == 4) {
         mShowExitPrompt = true;
         if (mItemsCollectedCount >= TOTAL_ITEMS && IsKeyPressed(KEY_E)) {
            PlaySound(mLevelCompleteSound);
            mGameState.nextSceneID = 2; 
         }
      }
      else {
         mShowExitPrompt = false;
      }
   }
}

void LevelA::drawScreen(){
   char itemText[32];
   sprintf(itemText, "Items: %d/%d", mItemsCollectedCount, TOTAL_ITEMS);
   DrawText(itemText, 20,20,24, WHITE);


   extern int gPlayerLives;
   char livesText[32];
   sprintf(livesText, "Lives: %d", gPlayerLives);
   DrawText(livesText, 20,50,24, RED);

   if(mShowExitPrompt){
      if (mItemsCollectedCount >= TOTAL_ITEMS){
         const char* prompt = "Press E to exit to Level 2!";
         int promptWidth = MeasureText(prompt, 24); // I saw some kid in class have this and i really liked it
         DrawRectangle(0, 540, 1000, 60, ColorAlpha(BLACK, 0.7f));
         DrawText(prompt, 500 - promptWidth / 2, 550, 24, WHITE);
      }
      else{
         const char* prompt = "Door locked! Find all books first.";
         int promptWidth = MeasureText(prompt, 20); 
         DrawRectangle(0, 540, 1000, 60, ColorAlpha(BLACK, 0.7f));
         DrawText(prompt, 500 - promptWidth / 2, 555, 20, RED);
      }
   }

}

void LevelA::render()
{
   ClearBackground(ColorFromHex("#080822ff"));

/*    DrawTexturePro(
        mBackgroundTexture,
        {0, 0, (float)mBackgroundTexture.width, (float)mBackgroundTexture.height},
        {0, 0, 1000, 600}, 
        {0, 0},
        0.0f,
        WHITE
   ); */

   mGameState.map->render();

   for (int i = 0; i < TOTAL_ITEMS; i++) {
      if (!mItemCollected[i]) {
         mItems[i]->render();
      }
   }

   mGameState.xochitl->render();
   mMrFlinch->render();
   mCat->render();
   drawScreen();
}

void LevelA::shutdown()
{
   delete mGameState.xochitl;
   delete mGameState.map;
   delete mMrFlinch;
   delete mCat;

   for (int i = 0; i < TOTAL_ITEMS; i++) {
      delete mItems[i];
   }

   // UnloadTexture(mBackgroundTexture);
   UnloadMusicStream(mGameState.bgm);
   UnloadSound(mLevelCompleteSound); 
   UnloadSound(mPickupSound);
   UnloadSound(mSpottedSound);
}