#include <vector>

#include "raylib.h"
#include "particle.h"

#define GRAVITY Vector2{0.0, 490.f}

void initCurtain(std::vector<Particle>& curtain, int screenWidth) {
    const int curtainWidth = 60;
    const int curtainHeight = 50;
    const int restingDist = 6;

    float xOff = (float)screenWidth / 2.f - (curtainWidth * restingDist) / 2.f;
    float yOff = 40.f;

    for (int y(0); y < curtainHeight; y++) {
        for (int x(0); x < curtainWidth; x++) {
            Particle p = Particle(Vector2{xOff + (float)x * restingDist, yOff + (float)y}, 1.f);
            p.applyForce(Vector2{GRAVITY.x * p.mass, GRAVITY.y * p.mass});
            curtain.push_back(p);
        }
    }

    for (int y(0); y < curtainHeight; y++) {
        for (int x(0); x < curtainWidth; x++) {
            int ind = y * curtainWidth + x;
            Particle &newParticle = curtain[ind];
            if (x != 0)
                newParticle.attachTo(&curtain[ind - 1], restingDist);
            if (y != 0)
                newParticle.attachTo(&curtain[(y - 1) * curtainWidth + x], restingDist);
            if (y == 0)
                newParticle.pin();
        }
    }
}

int main() {
    std::vector<Particle> curtain;

    const int screenWidth = 800;
    const int screenHeight = 600;
    initCurtain(curtain, screenWidth);

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Cloth Simulation");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        for (int i(0); i < 3; i++) {
            for (Particle& p : curtain)
                p.updateLinks();
        }
        for (Particle& p : curtain) {
            p.update(GetFrameTime());
            p.draw();
        }
        if (IsKeyPressed(KEY_R)) {
            curtain.clear();
            initCurtain(curtain, screenWidth);
        }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
