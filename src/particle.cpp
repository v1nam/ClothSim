#include <cmath>

#include "raylib.h"
#include "particle.h"

float distanceSquared(Vector2 v1, Vector2 v2) {
    return powf(v2.x - v1.x, 2.f) + powf(v2.y - v1.y, 2.f);
}

void Particle::draw() {
    DrawPixelV(pos, WHITE);
    for (Link& link : links)
        link.draw();
}

void Particle::update(float dt) {
    if (!pinned) {
        float dtSq = dt * dt;
        Vector2 vel = Vector2{pos.x - oldPos.x, pos.y - oldPos.y};
        vel.x *= 0.99;
        vel.y *= 0.99;
        oldPos = pos;
        pos = Vector2{pos.x + vel.x + 0.5f * accel.x * dtSq, pos.y + vel.y + 0.5f * accel.y * dtSq};
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            if (distanceSquared(GetMousePosition(), pos) <= tearSizeSq)
                links.clear();
        }
    }
}

void Particle::updateLinks() {
    for (Link& link : links)
        link.solve();
    if (pinned)
        pos = pinnedPos;
}

void Particle::applyForce(Vector2 force) {
    accel = Vector2{force.x / mass, force.y / mass};
}

void Particle::pin() {
    pinned = true;
    pinnedPos = pos;
}

void Particle::attachTo(Particle *p2, float restingDist) {
    links.emplace_back(Link(this, p2, restingDist));
}

void Particle::Link::solve() {
    Vector2 diff = Vector2{p1->pos.x - p2->pos.x, p1->pos.y - p2->pos.y};
    auto d = (float)sqrt((double)diff.x * diff.x + diff.y * diff.y);
    if (d >= durability) {
        p1->links.clear();
    }
    float diffNorm = (restingDist - d) / d;

    Vector2 translate = Vector2{diff.x * 0.5f * diffNorm, diff.y * 0.5f * diffNorm};

    p1->pos.x += translate.x;
    p1->pos.y += translate.y;
    p2->pos.x -= translate.x;
    p2->pos.y -= translate.y;
}

void Particle::Link::draw() {
    DrawLineV(p1->pos, p2->pos, WHITE);
}