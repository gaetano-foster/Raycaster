#include "dynamics.hh"
#include <filesystem>
#include <cmath>

DynamicRenderer::DynamicRenderer(const char *path) {
    namespace fs = std::filesystem;

    int i = 0;
    for (const auto &entry : fs::directory_iterator(path)) {
        tex[i] = new olc::Sprite(entry.path());
        i++;
    }
}

DynamicRenderer::DynamicRenderer() {

}

DynamicRenderer::~DynamicRenderer() {
    for (auto &t : tex) {
        delete t;
    }
}

void DynamicRenderer::start() {
    float sa = sinf(e->a), ca = cosf(e->a);
    
    positions[0] = { e->x + sa, e->y + ca };                    // forward
    positions[1] = { e->x + (sa + ca), e->y + (ca - sa) };      // forward + right
    positions[2] = { e->x + ca, e->y + -sa };                   // right
    positions[3] = { e->x + (-sa + ca), e->y + (-ca - sa) };    // backward + right
    positions[4] = { e->x + -sa, e->y + -ca };                  // backward
    positions[5] = { e->x + (-sa - ca), e->y + (-ca + sa) };    // backward + left
    positions[6] = { e->x + -ca, e->y + sa };                   // left
    positions[7] = { e->x + (sa - ca), e->y + (ca + sa) };      // forward + left
}

void DynamicRenderer::update(float fElapsedTime) {
    float sa = sinf(e->a), ca = cosf(e->a);
    
    positions[0] = { e->x + sa, e->y + ca };                    // forward
    positions[1] = { e->x + (sa + ca), e->y + (ca - sa) };      // forward + right
    positions[2] = { e->x + ca, e->y + -sa };                   // right
    positions[3] = { e->x + (-sa + ca), e->y + (-ca - sa) };    // backward + right
    positions[4] = { e->x + -sa, e->y + -ca };                  // backward
    positions[5] = { e->x + (-sa - ca), e->y + (-ca + sa) };    // backward + left
    positions[6] = { e->x + -ca, e->y + sa };                   // left
    positions[7] = { e->x + (sa - ca), e->y + (ca + sa) };      // forward + left
}

void DynamicRenderer::render(float *depth_buffer) {
    Entity player = *(e->current->player());
    int index = 0;
    float prev = 0;
    olc::Sprite *texture;
    auto dist = [](olc::vf2d p1, olc::vf2d p2) {
        float x1 = std::min(p1.x, p2.x);
        float x2 = std::max(p1.x, p2.x);
        float y1 = std::min(p1.y, p2.y);
        float y2 = std::max(p1.y, p2.y);
        float a = x2 - x1;
        float b = y2 - y1;
        return sqrtf(a*a+b*b);
    };

    for (int i = 0; i < 8; i++) {
        float d = dist(positions[i], { player.x, player.y });
        if (d > prev) {
            prev = d;
            index = i;
            printf("%d\n", i);
        }
    }
    texture = tex[index];
    
    // can the object be seen?
    olc::vf2d vec = { e->x - player.x, e->y - player.y };
    float distfromplayer = dist({ e->x, e->y }, { player.x, player.y }); //sqrtf(vec.x*vec.x+vec.y*vec.y);
    float obj_a = atan2f(cosf(player.a), sinf(player.a)) - atan2f(vec.y, vec.x);

    if (obj_a < -PI)
        obj_a += 2.0f*PI;
    if (obj_a > PI)
        obj_a -= 2.0f*PI;

    bool in_fov = fabs(obj_a) < FOV/1.5f;

    if (in_fov && distfromplayer >= 0.5f && distfromplayer < RENDER_DIST) {
        float ceil = (float)(SCR_H/2.0f) - SCR_H / distfromplayer;
        float floor = (float)SCR_H - ceil;
        float ar = (float)texture->height / (float)texture->width;
        float obj_h = floor - ceil;
        float obj_w = obj_h / ar;
        float obj_mid = (0.5f * (obj_a / (FOV / 2.0f)) + 0.5f) * (float)SCR_W;

        for (float y = 0; y < obj_h; y++) {
            for (float x = 0; x < obj_w; x++) {
                float dark = std::clamp(distfromplayer, 1.0f, 255.0f);
                olc::vf2d sample = { x / obj_w, y / obj_h };
                olc::Pixel pxcol = texture->Sample(sample.x, sample.y);
                olc::Pixel col = olc::Pixel(pxcol.r / dark, pxcol.g / dark, pxcol.b / dark, pxcol.a);
                int column = (int)(obj_mid + x - (obj_w / 2.0f));

                if (column >= 0 && column < SCR_W) {
                    if (col.a > 228 && depth_buffer[column] >= distfromplayer) {
                        e->current->engine->Draw(column, ceil+y, col); // TODO: transparency issues
                        depth_buffer[column] = distfromplayer;
                    }
                }
            }
        }
    }
}