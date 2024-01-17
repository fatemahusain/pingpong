/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
//#include "screens.h"    // NOTE: Declares global (extern) variables and screens functions



// #if defined(PLATFORM_WEB)
//     #include <emscripten/emscripten.h>
// #endif

//----------------------------------------------------------------------------------
// Shared Variables Definition (global)
// NOTE: Those variables are shared between modules through screens.h
//----------------------------------------------------------------------------------
// GameScreen currentScreen = LOGO;
// Font font = { 0 };
// Music music = { 0 };
// Sound fxCoin = { 0 };

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
// static const int screenWidth = 800;
// static const int screenHeight = 450;

// Required variables to manage screen transitions (fade-in, fade-out)
// static float transAlpha = 0.0f;
// static bool onTransition = false;
// static bool transFadeOut = false;
// static int transFromScreen = -1;
// static GameScreen transToScreen = UNKNOWN;

//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
// static void ChangeToScreen(int screen);     // Change to screen, no transition effect

// static void TransitionToScreen(int screen); // Request transition to next screen
// static void UpdateTransition(void);         // Update transition effect
// static void DrawTransition(void);           // Draw transition effect (full-screen rectangle)

// static void UpdateDrawFrame(void);          // Update and draw one frame

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------


int cpuScore = 0;
int playerScore = 0;

struct Ball {
    float x;
    float y;
    int speed_x, speed_y;
    int radius;
};

void Draw(const struct Ball *ball) {
    DrawCircle(ball->x, ball->y, ball->radius, YELLOW);
}

void ResetBall(struct Ball *ball) {
    ball->x = GetScreenWidth()/2;
    ball->y = GetScreenHeight()/2;

    int direction[2] = {1, -1};
    ball->speed_x *= direction[GetRandomValue(0,1)];
    ball->speed_y *= direction[GetRandomValue(0,1)];

}

void Update(struct Ball *ball) {
    ball->x += ball->speed_x;
    ball->y += ball->speed_y;
    if (ball->y + ball->radius >= GetScreenHeight() || ball->y - ball->radius <= 0) {
        ball->speed_y *= -1;
    }
    if (ball->x + ball->radius >= GetScreenWidth() || ball->x - ball->radius <= 0) {
        ball->speed_x *= -1;
    }
    if (ball->x + ball->radius >= GetScreenWidth()) {
        cpuScore++;
        ResetBall(ball);
    }
    if (ball->x - ball->radius <= 0) {
        playerScore++;
        ResetBall(ball);
    }
}



struct Paddle {
    float x;
    float y;
    float width;
    float height;
    int speed;
};

void DrawPaddle(struct Paddle *player) {
    DrawRectangle(player->x, player->y, player->width, player->height, WHITE);
}

void UpdatePaddle(struct Paddle *player) {
    if (IsKeyDown(KEY_UP)) {
        player->y -= player->speed;
    } 
    if (IsKeyDown(KEY_DOWN)) {
         player->y += player->speed;
    }
    if (player->y <= 0) {
        player->y = 0;
    }
    if (player->y + player->height >= GetScreenHeight()) {
        player->y = GetScreenHeight() - 120;
    }
}


struct CpuPaddle {
    float x;
    float y;
    float width;
    float height;
    int speed;

};

void DrawCpuPaddle(struct CpuPaddle *cpu) {
    DrawRectangle(cpu->x, cpu->y, cpu->width, cpu->height, WHITE);
}

void UpdateCpu(struct CpuPaddle *cpu, int ball_y) {
    if (cpu->y + cpu->height/2 > ball_y) {
        //cpu->y -= cpu->speed;
        cpu->y = ball_y;
    }
     if (cpu->y + cpu->height/2 <= ball_y) {
        //cpu->y += cpu->speed;
        cpu->y = ball_y;
    }
    

}





struct Ball ball;
struct Paddle player;
struct CpuPaddle cpu;

int main () {

    const int screenWidth = 1200;
    const int screenHeight = 800;  
    ball.radius = 20;
    ball.x = screenWidth/2;
    ball.y = screenHeight/2;
    ball.speed_y = 7;
    ball.speed_x = 7; 

    player.width = 25;
    player.height = 120;
    player.x = screenWidth - player.width - 10;
    player.y = screenHeight/2 - player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screenHeight/2 - cpu.height/2;
    cpu.speed = 6;

    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Ping Pong!");
    while(WindowShouldClose()== false) {

        BeginDrawing();
    
        //Drawing Circle
        Update(&ball);
        UpdatePaddle(&player);
        UpdateCpu(&cpu, ball.y);

        //checking for collision
        if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, (Rectangle){player.x, player.y, player.width ,player.height})) {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec((Vector2){ball.x, ball.y}, ball.radius, (Rectangle){cpu.x, cpu.y, cpu.width ,cpu.height})) {
            ball.speed_x *= -1;
        }
        

        ClearBackground(DARKPURPLE);
        DrawRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, DARKPURPLE);
        DrawCircle(screenWidth/2, screenHeight/2, 200, PURPLE);
         DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);
        Draw(&ball);
        DrawPaddle(&player);
        DrawCpuPaddle(&cpu);
        DrawText(TextFormat("%i", cpuScore), screenWidth/4 - 20,20,  80, WHITE);
        DrawText(TextFormat("%i", playerScore), 3 * screenWidth/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }
    


    CloseWindow();
}
//     SetTargetFPS(60);

//     while (WindowShouldClose() == false){
//         BeginDrawing();
//         ClearBackground(GREEN);
//         ball_x += ball_speed_x;
//         ball_y += ball_speed_y;

//         if(ball_x + ball_radius >= screenWidth  || ball_x - ball_radius <= 0)
//         {
//             ball_speed_x *= -1;
//         }

//         if(ball_y + ball_radius >= screenHeight  || ball_y - ball_radius <= 0)
//         {
//             ball_speed_y *= -1;
//         }

//         DrawCircle(ball_x,ball_y,ball_radius, WHITE);
//         EndDrawing();
//     }

//     CloseWindow();
//     return 0;
// }

// int main(void)
// {
//     // Initialization
//     //---------------------------------------------------------
//     InitWindow(screenWidth, screenHeight, "raylib game template");

//     InitAudioDevice();      // Initialize audio device

//     // Load global data (assets that must be available in all screens, i.e. font)
//     font = LoadFont("resources/mecha.png");
//     music = LoadMusicStream("resources/ambient.ogg");
//     fxCoin = LoadSound("resources/coin.wav");

//     SetMusicVolume(music, 1.0f);
//     PlayMusicStream(music);

//     // Setup and init first screen
//     currentScreen = LOGO;
//     InitLogoScreen();

// #if defined(PLATFORM_WEB)
//     emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
// #else
//     SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
//     //--------------------------------------------------------------------------------------

//     // Main game loop
//     while (!WindowShouldClose())    // Detect window close button or ESC key
//     {
//         UpdateDrawFrame();
//     }
// #endif

//     // De-Initialization
//     //--------------------------------------------------------------------------------------
//     // Unload current screen data before closing
//     switch (currentScreen)
//     {
//         case LOGO: UnloadLogoScreen(); break;
//         case TITLE: UnloadTitleScreen(); break;
//         case GAMEPLAY: UnloadGameplayScreen(); break;
//         case ENDING: UnloadEndingScreen(); break;
//         default: break;
//     }

//     // Unload global data loaded
//     UnloadFont(font);
//     UnloadMusicStream(music);
//     UnloadSound(fxCoin);

//     CloseAudioDevice();     // Close audio context

//     CloseWindow();          // Close window and OpenGL context
//     //--------------------------------------------------------------------------------------

//     return 0;
// }

// //----------------------------------------------------------------------------------
// // Module specific Functions Definition
// //----------------------------------------------------------------------------------
// // Change to next screen, no transition
// static void ChangeToScreen(GameScreen screen)
// {
//     // Unload current screen
//     switch (currentScreen)
//     {
//         case LOGO: UnloadLogoScreen(); break;
//         case TITLE: UnloadTitleScreen(); break;
//         case GAMEPLAY: UnloadGameplayScreen(); break;
//         case ENDING: UnloadEndingScreen(); break;
//         default: break;
//     }

//     // Init next screen
//     switch (screen)
//     {
//         case LOGO: InitLogoScreen(); break;
//         case TITLE: InitTitleScreen(); break;
//         case GAMEPLAY: InitGameplayScreen(); break;
//         case ENDING: InitEndingScreen(); break;
//         default: break;
//     }

//     currentScreen = screen;
// }

// // Request transition to next screen
// static void TransitionToScreen(GameScreen screen)
// {
//     onTransition = true;
//     transFadeOut = false;
//     transFromScreen = currentScreen;
//     transToScreen = screen;
//     transAlpha = 0.0f;
// }

// // Update transition effect (fade-in, fade-out)
// static void UpdateTransition(void)
// {
//     if (!transFadeOut)
//     {
//         transAlpha += 0.05f;

//         // NOTE: Due to float internal representation, condition jumps on 1.0f instead of 1.05f
//         // For that reason we compare against 1.01f, to avoid last frame loading stop
//         if (transAlpha > 1.01f)
//         {
//             transAlpha = 1.0f;

//             // Unload current screen
//             switch (transFromScreen)
//             {
//                 case LOGO: UnloadLogoScreen(); break;
//                 case TITLE: UnloadTitleScreen(); break;
//                 case OPTIONS: UnloadOptionsScreen(); break;
//                 case GAMEPLAY: UnloadGameplayScreen(); break;
//                 case ENDING: UnloadEndingScreen(); break;
//                 default: break;
//             }

//             // Load next screen
//             switch (transToScreen)
//             {
//                 case LOGO: InitLogoScreen(); break;
//                 case TITLE: InitTitleScreen(); break;
//                 case GAMEPLAY: InitGameplayScreen(); break;
//                 case ENDING: InitEndingScreen(); break;
//                 default: break;
//             }

//             currentScreen = transToScreen;

//             // Activate fade out effect to next loaded screen
//             transFadeOut = true;
//         }
//     }
//     else  // Transition fade out logic
//     {
//         transAlpha -= 0.02f;

//         if (transAlpha < -0.01f)
//         {
//             transAlpha = 0.0f;
//             transFadeOut = false;
//             onTransition = false;
//             transFromScreen = -1;
//             transToScreen = UNKNOWN;
//         }
//     }
// }

// // Draw transition effect (full-screen rectangle)
// static void DrawTransition(void)
// {
//     DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, transAlpha));
// }

// // Update and draw game frame
// static void UpdateDrawFrame(void)
// {
//     // Update
//     //----------------------------------------------------------------------------------
//     UpdateMusicStream(music);       // NOTE: Music keeps playing between screens

//     if (!onTransition)
//     {
//         switch(currentScreen)
//         {
//             case LOGO:
//             {
//                 UpdateLogoScreen();

//                 if (FinishLogoScreen()) TransitionToScreen(TITLE);

//             } break;
//             case TITLE:
//             {
//                 UpdateTitleScreen();

//                 if (FinishTitleScreen() == 1) TransitionToScreen(OPTIONS);
//                 else if (FinishTitleScreen() == 2) TransitionToScreen(GAMEPLAY);

//             } break;
//             case OPTIONS:
//             {
//                 UpdateOptionsScreen();

//                 if (FinishOptionsScreen()) TransitionToScreen(TITLE);

//             } break;
//             case GAMEPLAY:
//             {
//                 UpdateGameplayScreen();

//                 if (FinishGameplayScreen() == 1) TransitionToScreen(ENDING);
//                 //else if (FinishGameplayScreen() == 2) TransitionToScreen(TITLE);

//             } break;
//             case ENDING:
//             {
//                 UpdateEndingScreen();

//                 if (FinishEndingScreen() == 1) TransitionToScreen(TITLE);

//             } break;
//             default: break;
//         }
//     }
//     else UpdateTransition();    // Update transition (fade-in, fade-out)
//     //----------------------------------------------------------------------------------

//     // Draw
//     //----------------------------------------------------------------------------------
//     BeginDrawing();

//         ClearBackground(RAYWHITE);

//         switch(currentScreen)
//         {
//             case LOGO: DrawLogoScreen(); break;
//             case TITLE: DrawTitleScreen(); break;
//             case OPTIONS: DrawOptionsScreen(); break;
//             case GAMEPLAY: DrawGameplayScreen(); break;
//             case ENDING: DrawEndingScreen(); break;
//             default: break;
//         }

//         // Draw full screen rectangle in front of everything
//         if (onTransition) DrawTransition();

//         //DrawFPS(10, 10);

//     EndDrawing();
//     //----------------------------------------------------------------------------------
// }
