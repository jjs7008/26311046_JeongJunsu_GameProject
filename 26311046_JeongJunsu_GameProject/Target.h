#pragma once

class Target
{
public:
    void Init();
    void MoveNext();

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetWidth() const { return 54.0f; }
    float GetHeight() const { return 54.0f; }

private:
    float m_x = 120.0f;
    float m_y = 390.0f;
    int m_index = 0;
};
