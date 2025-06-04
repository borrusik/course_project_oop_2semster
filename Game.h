#pragma once

#include <vector>
#include "Target.h"

using namespace System::Drawing;

class Game {
public:
    std::vector<Target*> targets;
    int score;
    int timeLeft;

    int width, height;

    Game();
    ~Game();

    void InitTargets(int n, int w, int h);

    void Draw(Graphics^ g);

    void OnClick(int mx, int my);

    void Tick();

    void Resize(int w, int h);
};
