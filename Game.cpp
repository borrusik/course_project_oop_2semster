#include "Game.h"
#include <cstdlib>
#include <ctime>

// Конструктор: сбросить всё, seed random
Game::Game() : score(0), timeLeft(30), width(0), height(0) {
    std::srand((unsigned)std::time(0));
}

Game::~Game() {
    // Очистить все мишени
    for (auto t : targets) {
        delete t;
    }
    targets.clear();
}

void Game::InitTargets(int n, int w, int h) {
    width = w;
    height = h;
    for (auto t : targets) delete t;
    targets.clear();

    int radius = 20;
    int max_attempts = 100;

    for (int i = 0; i < n; ++i) {
        int attempts = 0;
        bool ok = false;
        int x = 0, y = 0;
        while (!ok && attempts < max_attempts) {
            x = rand() % (width - 2 * radius) + radius;
            y = rand() % (height - 2 * radius) + radius;
            ok = true;
            // Проверка на пересечение с другими
            for (auto t : targets) {
                int dx = t->x - x;
                int dy = t->y - y;
                if (dx * dx + dy * dy < (2 * radius) * (2 * radius)) {
                    ok = false;
                    break;
                }
            }
            ++attempts;
        }
        if (ok) {
            targets.push_back(new Target(x, y, radius));
        }
    }
}

void Game::Draw(Graphics^ g) {
    for (auto t : targets) t->Draw(g);
}

void Game::OnClick(int mx, int my) {
    for (auto t : targets) {
        if (t->Contains(mx, my)) {
            score++;

            // Респаун на новом месте без пересечений
            int attempts = 0;
            int max_attempts = 100;
            int radius = t->radius;
            bool ok = false;
            int new_x = t->x, new_y = t->y;

            while (!ok && attempts < max_attempts) {
                new_x = rand() % (width - 2 * radius) + radius;
                new_y = rand() % (height - 2 * radius) + radius;
                ok = true;
                // Проверка на пересечение только с другими активными мишенями
                for (auto other : targets) {
                    if (other != t && other->active) {
                        int dx = other->x - new_x;
                        int dy = other->y -
