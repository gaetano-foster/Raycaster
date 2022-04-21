#ifndef PLAYER_H
#define PLAYER_H
#include <olcPixelGameEngine.h>
#include "../component.hh"
#include "../../../definitions.hh"

class PlayerComponent : public Component {
public:
    void start() override;
    void update(float fElapsedTime) override;
    void render(float *depth_buffer) override;

private:
    void move(olc::Key key, bool &k, olc::vf2d ray_dir, olc::vf2d &inter, float fElapsedTime);
};

#endif