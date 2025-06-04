#include "Target.h"

Target::Target(int x, int y, int radius) : x(x), y(y), radius(radius), active(true) {}

void Target::Draw(Graphics^ g) {
    if (!active) return;
    // Внешний круг — красный
    g->FillEllipse(Brushes::Red, x - radius, y - radius, radius * 2, radius * 2);
    // Средний круг — белый
    g->FillEllipse(Brushes::White, x - radius * 0.6, y - radius * 0.6, radius * 1.2, radius * 1.2);
    // Внутренний круг — темно-синий
    SolidBrush^ blue = gcnew SolidBrush(Color::FromArgb(80, 60, 210, 255));
    g->FillEllipse(blue, x - radius * 0.3, y - radius * 0.3, radius * 0.6, radius * 0.6);
    // Контур — чёрный
    g->DrawEllipse(Pens::Black, x - radius, y - radius, radius * 2, radius * 2);
}

bool Target::Contains(int mx, int my) {
    int dx = x - mx, dy = y - my;
    return active && (dx * dx + dy * dy <= radius * radius);
}
