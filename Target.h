#pragma once

using namespace System::Drawing;

class Target {
public:
    int x, y, radius;
    bool active;

    Target(int x, int y, int radius);

    void Draw(Graphics^ g);
    bool Contains(int mx, int my);
};
