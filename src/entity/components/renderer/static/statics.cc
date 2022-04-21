#include "../../../entity.hh"
#include "statics.hh"
#include <cmath>

StaticRenderer::StaticRenderer(const char *path) {
    this->tex = new olc::Sprite(path);
}

StaticRenderer::StaticRenderer() {

}

StaticRenderer::~StaticRenderer() {
    delete tex;
}

void StaticRenderer::start() {
    
}

void StaticRenderer::update(float fElapsedTime) {

}

void StaticRenderer::render(float *depth_buffer) {
    Entity player = *(e->current->player());
    
    // can the object be seen?
    olc::vf2d vec = { e->x - player.x, e->y - player.y };
    float distfromplayer = sqrtf(vec.x*vec.x+vec.y*vec.y);
    float obj_a = atan2f(cosf(player.a), sinf(player.a)) - atan2f(vec.y, vec.x);

    if (obj_a < -PI)
        obj_a += 2.0f*PI;
    if (obj_a > PI)
        obj_a -= 2.0f*PI;

    bool in_fov = fabs(obj_a) < FOV/1.5f;

    if (in_fov && distfromplayer >= 0.5f && distfromplayer < RENDER_DIST) {
        float ceil = (float)(SCR_H/2.0f) - SCR_H / distfromplayer;
        float floor = (float)SCR_H - ceil;
        float ar = (float)tex->height / (float)tex->width;
        float obj_h = floor - ceil;
        float obj_w = obj_h / ar;
        float obj_mid = (0.5f * (obj_a / (FOV / 2.0f)) + 0.5f) * (float)SCR_W;

        for (float y = 0; y < obj_h; y++) {
            for (float x = 0; x < obj_w; x++) {
                float dark = std::clamp(distfromplayer, 1.0f, 255.0f);
                olc::vf2d sample = { x / obj_w, y / obj_h };
                olc::Pixel pxcol = tex->Sample(sample.x, sample.y);
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
