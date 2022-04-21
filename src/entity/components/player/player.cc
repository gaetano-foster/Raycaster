#include "player.hh"
#include "../../entity.hh"
#include "../../../math/raycast.hh"
#include "../../../map/tiles/tilemanager.hh"

void PlayerComponent::start() {
    e->x = e->current->spx;
    e->y = e->current->spy;
}

void PlayerComponent::update(float fElapsedTime) {
    bool w = 1, a = 1, s = 1, d = 1;
    olc::vf2d inters, interw, interd, intera;
    float ray_angle = (e->a - FOV / 2.0f) + (((float)SCR_W / 2.0f) / SCR_W) * FOV;

    move(olc::A, a, { -cosf(ray_angle),  sinf(ray_angle) }, intera, fElapsedTime);
    move(olc::D, d, {  cosf(ray_angle), -sinf(ray_angle) }, interd, fElapsedTime);
    move(olc::W, w, {  sinf(ray_angle),  cosf(ray_angle) }, interw, fElapsedTime);
    move(olc::S, s, { -sinf(ray_angle), -cosf(ray_angle) }, inters, fElapsedTime);

    if (e->current->engine->GetKey(olc::RIGHT).bHeld)
        e->a += e->speed * fElapsedTime;

    if (e->current->engine->GetKey(olc::LEFT).bHeld)
        e->a -= e->speed * fElapsedTime;

    if (e->a > 2*PI)
        e->a = 0;
    else if (e->a < 0)
        e->a = 2*PI;
}

void PlayerComponent::render(float *depth_buffer) {
    
}

void PlayerComponent::move(olc::Key key, bool &k, olc::vf2d ray_dir, olc::vf2d &inter, float fElapsedTime) {
    if (e->current->engine->GetKey(key).bHeld && k) {
        olc::vf2d ray_start = { e->x, e->y };
        float distance;
        float mindist;
        olc::vi2d map_check;
        bool hitwall;
        int id = castray(ray_start, ray_dir, distance, hitwall, e->current, map_check);
        if (e->x == inter.x && e->y == inter.y) {
            k = 0;
            return;
        }
        if (hitwall) {
            inter = { (ray_start.x + ray_dir.x * distance) - ray_dir.x * fElapsedTime, (ray_start.y + ray_dir.y * distance) - ray_dir.y * fElapsedTime };
        }
        if (id) {
            if ((e->speed * fElapsedTime < distance) || !TileManager::get().get_tile(id)->solid) {
                e->x += ray_dir.x * e->speed * fElapsedTime;
                e->y += ray_dir.y * e->speed * fElapsedTime;
            } 
            else {
                e->x = inter.x; 
                e->y = inter.y;
                printf(" ");
            }
        }
    }
}