#ifndef DYNAMICS_HH
#define DYNAMICS_HH
#include "../../component.hh"
#include <array>
#include "../../../entity.hh"
#include <olcPixelGameEngine.h>

class DynamicRenderer : public Component {
public:
    DynamicRenderer(const char *path);
    DynamicRenderer();
    ~DynamicRenderer();

public:
    std::array<olc::Sprite*, 8> tex;

    void start() override;
    void update(float fElapsedTime) override;
    void render(float *depth_buffer) override;

private:
    std::array<olc::vf2d, 8> positions;
};

#endif