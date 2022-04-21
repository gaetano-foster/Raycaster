#ifndef STATICS_H
#define STATICS_H
#include "../../component.hh"
#include <olcPixelGameEngine.h>

class StaticRenderer : public Component {
public:
    StaticRenderer(const char *path);
    StaticRenderer();
    ~StaticRenderer();

public:
    olc::Sprite *tex;

    void start() override;
    void update(float fElapsedTime) override;
    void render(float *depth_buffer) override;
};

#endif