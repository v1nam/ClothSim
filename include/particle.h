#ifndef PARTICLES_PARTICLE_H
#define PARTICLES_PARTICLE_H

#include <vector>

#include "raylib.h"

class Particle {
private:
    Vector2 accel = Vector2{0.f, 0.f};
    Vector2 oldPos;
    Vector2 pinnedPos = Vector2{0.f, 0.f};
    bool pinned = false;
    const float tearSizeSq = 25.f;
    class Link {
    private:
        Particle* p1;
        Particle* p2;
        float restingDist;
        const float durability = 60.f;
    public:
        Link(Particle* pm1, Particle* pm2, float rd): p1(pm1), p2(pm2), restingDist(rd) {}
        void solve();
        void draw();
    };
    std::vector<Link> links;
public:
    float mass;
    Vector2 pos;
    Particle(Vector2 p, float m): pos(p), oldPos(p), mass(m) {}
    void draw();
    void update(float dt);
    void applyForce(Vector2 force);
    void pin();
    void attachTo(Particle* p2, float restingDist);
    void updateLinks();
};
#endif //PARTICLES_PARTICLE_H
