#pragma once

class Obstacle
{
public:
    void Init();
    void Update(float dt);
    void Reset();

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return 54.0f; }
    float GetHeight() const { return 54.0f; }

private:
    float m_x = 520.0f;
    float m_y = 130.0f;
    float m_speed = 150.0f;
    int m_index = 0;
};
