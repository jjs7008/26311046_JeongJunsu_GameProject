#include <glc2d.h>
#include <Windows.h>
#include <chrono>
#include <iostream>
#include "Player.h"
#include "Target.h"
#include "Obstacle.h"

// glc2d 라이브러리 연결
#if defined(_DEBUG)
    #if defined(_M_X64)
        #pragma comment(lib, "glc2d_x64_debug.lib")
    #elif defined(_M_IX86)
        #pragma comment(lib, "glc2d_win32_debug.lib")
    #endif
#else
    #if defined(_M_X64)
        #pragma comment(lib, "glc2d_x64_release.lib")
    #elif defined(_M_IX86)
        #pragma comment(lib, "glc2d_win32_release.lib")
    #endif
#endif

enum class GameState
{
    Start,
    Play,
    Result
};

int g_backgroundTexture = -1;
int g_playerTexture = -1;
int g_targetTexture = -1;
int g_obstacleTexture = -1;

Player g_player;
Target g_target;
Obstacle g_obstacle;

GameState g_state = GameState::Start;
int g_score = 0;
bool g_clear = false;
bool g_enterPressed = false;

const float GAME_TIME = 45.0f;

auto g_prevTime = std::chrono::steady_clock::now();
auto g_playStartTime = std::chrono::steady_clock::now();

bool IsHit(float ax, float ay, float aw, float ah,
           float bx, float by, float bw, float bh)
{
    return ax < bx + bw && ax + aw > bx &&
           ay < by + bh && ay + ah > by;
}

void StartGame()
{
    g_player.Init();
    g_target.Init();
    g_obstacle.Init();

    g_score = 0;
    g_clear = false;
    g_state = GameState::Play;
    g_playStartTime = std::chrono::steady_clock::now();

    std::cout << "Game Start" << std::endl;
}

void UpdateGame(float dt)
{
    bool enterNow = (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;

    if (enterNow && !g_enterPressed)
    {
        if (g_state == GameState::Start || g_state == GameState::Result)
            StartGame();
    }
    g_enterPressed = enterNow;

    if (g_state != GameState::Play)
        return;

    g_player.Update(dt);
    g_obstacle.Update(dt);

    if (IsHit(g_player.GetX(), g_player.GetY(), g_player.GetWidth(), g_player.GetHeight(),
              g_target.GetX(), g_target.GetY(), g_target.GetWidth(), g_target.GetHeight()))
    {
        g_score += 10;
        g_target.MoveNext();
        std::cout << "Score : " << g_score << std::endl;
    }

    if (IsHit(g_player.GetX(), g_player.GetY(), g_player.GetWidth(), g_player.GetHeight(),
              g_obstacle.GetX(), g_obstacle.GetY(), g_obstacle.GetWidth(), g_obstacle.GetHeight()))
    {
        g_player.Damage();
        g_obstacle.Reset();
        std::cout << "Life : " << g_player.GetLife() << std::endl;
    }

    float playTime = std::chrono::duration<float>(
        std::chrono::steady_clock::now() - g_playStartTime).count();

    if (g_score >= 100)
    {
        g_clear = true;
        g_state = GameState::Result;
        std::cout << "CLEAR - Press Enter to restart" << std::endl;
    }
    else if (g_player.GetLife() <= 0 || playTime >= GAME_TIME)
    {
        g_clear = false;
        g_state = GameState::Result;
        std::cout << "FAIL - Press Enter to restart" << std::endl;
    }
}

int Render()
{
    auto now = std::chrono::steady_clock::now();
    float dt = std::chrono::duration<float>(now - g_prevTime).count();
    g_prevTime = now;

    if (dt > 0.05f)
        dt = 0.05f;

    UpdateGame(dt);

    VEC2 backgroundPos(0.0f, 0.0f);
    g2_Draw2D(g_backgroundTexture, {}, &backgroundPos);

    if (g_state == GameState::Start)
    {
        VEC2 playerPos(120.0f, 405.0f);
        VEC2 targetPos(650.0f, 360.0f);

        g2_Draw2D(g_playerTexture, {}, &playerPos);
        g2_Draw2D(g_targetTexture, {}, &targetPos);
        return 0;
    }

    VEC2 targetPos(g_target.GetX(), g_target.GetY());
    VEC2 obstaclePos(g_obstacle.GetX(), g_obstacle.GetY());
    VEC2 playerPos(g_player.GetX(), g_player.GetY());

    g2_Draw2D(g_targetTexture, {}, &targetPos);
    g2_Draw2D(g_obstacleTexture, {}, &obstaclePos);
    g2_Draw2D(g_playerTexture, {}, &playerPos);

    return 0;
}

int main()
{
    g2_InitSdk();
    g2_SetClearColor(0xFF172033);

    g2_SetRender(Render);
    g2_CreateWin(100, 100, 800, 600, "Target Catch - Week 02");

    g_backgroundTexture = g2_TextureLoad("Texture/background.png");
    g_playerTexture = g2_TextureLoad("Texture/player.png");
    g_targetTexture = g2_TextureLoad("Texture/goal.png");
    g_obstacleTexture = g2_TextureLoad("Texture/obstacle.png");

    std::cout << "TARGET CATCH" << std::endl;
    std::cout << "Press Enter to start" << std::endl;

    g2_Run();

    g2_TextureRelease(g_backgroundTexture);
    g2_TextureRelease(g_playerTexture);
    g2_TextureRelease(g_targetTexture);
    g2_TextureRelease(g_obstacleTexture);

    g2_DestroyWin();
    return 0;
}
