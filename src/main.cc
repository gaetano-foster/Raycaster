#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <array>
#include "map/map.hh"
#include "entity/entity.hh"
#include "entity/components/player/player.hh"
#include "entity/components/renderer/static/statics.hh"
#include "entity/components/renderer/dynamic/dynamics.hh"
#include "map/tiles/tilemanager.hh"
#include "definitions.hh"

class Raycaster : public olc::PixelGameEngine {
public:
    Raycaster() {
        sAppName = "Raycaster";
    }

public:
	bool OnUserCreate() override {
        TileManager::get().load();
        depth_buffer = new float[SCR_W];
        map = new Map("res/map.lvl", this);
        map->add_entity((new Entity())->add_component(new PlayerComponent()));
        map->add_entity((new Entity())->setpos(8.5f, 8.5f)->add_component(new DynamicRenderer("res/man/")));
        map->start();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        map->update(fElapsedTime);
        map->render(depth_buffer);

		return true;
	}
private:
    Map *map;
    float *depth_buffer;
};

int main() {
    Raycaster demo;
    if (demo.Construct(SCR_W, SCR_H, 4, 4))
        demo.Start();
    putchar('\n');
    return 0;
}