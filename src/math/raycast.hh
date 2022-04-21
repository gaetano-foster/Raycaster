#ifndef RAYCAST_H
#define RAYCAST_H
#include <olcPixelGameEngine.h>
#include "../map/map.hh"

int castray(olc::vf2d ray_start, olc::vf2d ray_dir, float &distance, bool &hitwall, Map *map, olc::vi2d &map_check);

#endif