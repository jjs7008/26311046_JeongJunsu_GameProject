#include <glc2d.h>
#include <cmath>
#include <chrono>

// glc2d library link
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

int g_backgroundTexture = -1;
int g_playerTexture = -1;
int g_goalTexture = -1;

constexpr float WINDOW_WIDTH = 800.0f;
constexpr float PLAYER_Y = 430.0f;
constexpr float MOVE_CENTER_X = 360.0f;
constexpr float MOVE_RANGE = 260.0f;

const auto g_startTime = std::chrono::steady_clock::now();

int Render()
{
    // Background
    VEC2 backgroundPos(0.0f, 0.0f);
    g2_Draw2D(g_backgroundTexture, {}, &backgroundPos);

    // Goal object
    VEC2 goalPos(675.0f, 105.0f);
    g2_Draw2D(g_goalTexture, {}, &goalPos);

    // Simple automatic movement so the first-week build already contains game logic.
    const auto now = std::chrono::steady_clock::now();
    const float seconds = std::chrono::duration<float>(now - g_startTime).count();
    const float playerX = MOVE_CENTER_X + std::sin(seconds * 1.6f) * MOVE_RANGE;

    VEC2 playerPos(playerX, PLAYER_Y);
    g2_Draw2D(g_playerTexture, {}, &playerPos);

    return 0;
}

int main()
{
    // Initialize glc2d.
    g2_InitSdk();

    // Window background color used while resources are being prepared.
    g2_SetClearColor(0xFF172033);

    // Register render callback and create window.
    g2_SetRender(Render);
    g2_CreateWin(100, 100, 800, 600, "Jung Junsu - Week 01 Game Project");

    // Load week-01 prototype resources.
    g_backgroundTexture = g2_TextureLoad("Texture/background.png");
    g_playerTexture = g2_TextureLoad("Texture/player.png");
    g_goalTexture = g2_TextureLoad("Texture/goal.png");

    // Start the game loop.
    g2_Run();

    // Release resources.
    g2_TextureRelease(g_backgroundTexture);
    g2_TextureRelease(g_playerTexture);
    g2_TextureRelease(g_goalTexture);

    g2_DestroyWin();
    return 0;
}
