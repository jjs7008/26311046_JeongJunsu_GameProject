#pragma once

class Player
{
public:
    void Init();
    void Update(float dt);
    void Damage();

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return 64.0f; }
    float GetHeight() const { return 64.0f; }
    int GetLife() const { return m_life; }

private:
    float m_x = 360.0f;
    float m_y = 405.0f;
    float m_speed = 260.0f;
    int m_life = 3;
};
