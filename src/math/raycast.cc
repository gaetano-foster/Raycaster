#include "raycast.hh"
#include "../definitions.hh"

int castray(olc::vf2d ray_start, olc::vf2d ray_dir, float &distance, bool &hitwall, Map *map, olc::vi2d &map_check) {
    olc::vf2d step_size = (olc::vf2d) { sqrt(1 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x)), sqrt(1 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y)) };
    map_check = (olc::vi2d) { (int)ray_start.x, (int)ray_start.y };
    olc::vf2d rayl_1d;
    olc::vi2d step;

    if (ray_dir.x < 0) {
		step.x = -1;
		rayl_1d.x = (ray_start.x - (map_check.x)) * step_size.x;
	}
	else {
		step.x = 1;
		rayl_1d.x = ((map_check.x + 1) - ray_start.x) * step_size.x;
	}

	if (ray_dir.y < 0) {
		step.y = -1;
	    rayl_1d.y = (ray_start.y - (map_check.y)) * step_size.y;
	}
	else {
		step.y = 1;
		rayl_1d.y = ((map_check.y + 1) - ray_start.y) * step_size.y;
	}

    hitwall = 0;
    distance = 0;

    while (!hitwall && distance < RENDER_DIST) {
		// Walk along shortest path
		if (rayl_1d.x < rayl_1d.y) {
			map_check.x += step.x;
	        distance = rayl_1d.x;
	        rayl_1d.x += step_size.x;
	    }
	    else {
	    	map_check.y += step.y;
			distance = rayl_1d.y;
			rayl_1d.y += step_size.y;
		}

		// Test tile at new test point
		if (map_check.x >= 0 && map_check.x < map->w && map_check.y >= 0 && map_check.y < map->h) {
			if (map->get_tile(map_check.x, map_check.y) > 0) {
				hitwall = 1;
	   		}
    	}
    }
	if (distance > RENDER_DIST)
		distance = RENDER_DIST;
	return map->get_tile(map_check.x, map_check.y);
}