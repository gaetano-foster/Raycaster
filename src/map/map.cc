#include "map.hh"
#include <iostream>
#include <strstream>
#include <algorithm>
#include <cmath>
#include "../entity/entity.hh"
#include "../math/raycast.hh"
#include "tiles/tilemanager.hh"

std::vector<std::string> m_split(std::string str, std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

Map::Map(olc::PixelGameEngine *engine) {
    m = new int[w*h]{0};
    this->engine = engine;
}

Map::Map(int w, int h, olc::PixelGameEngine *engine) {
    this->w = w;
    this->h = h;
    m = new int[w*h]{0};
    this->engine = engine;
}

Map::Map(const char *path, olc::PixelGameEngine *engine) {
    std::ifstream f(path);
	if (!f.is_open()) {
        m = new int[w*h]{0};
        std::cout << "Error! Failed to load file at " << path << '\n'; 
		return;
    }

    int i = 0;
	while (!f.eof()) {
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;
        std::vector<std::string> tokens = m_split(line, " ");

        if (i == 0) {
            w = std::stoi(tokens[0]);
            h = std::stoi(tokens[1]);
            m = new int[w*h];
        }
        else if (i == 1) {
            spx = std::stoi(tokens[0]);
            spy = std::stoi(tokens[1]);
        }
        else {
            int y = i - 2;
            for (int x = 0; x < w; x++) {
                m[y*w+x] = std::stoi(tokens[x]);
            }
        }

        i++;
	}
    this->engine = engine;
}

Map::~Map() {
    delete[] m;
}

int Map::get_tile(int x, int y) {
    return m[y*w+x];
}

int *Map::get_map() {
    return m;
}

void Map::add_entity(Entity *e) {
    entities.push_back(e);
    e->current = this;
}

Entity* Map::player() {
    return entities[0];
}

void Map::start() {
    for (auto &e : entities) {
        e->start();
    }
}

void Map::update(float fElapsedTime) {
    for (auto &e : entities) {
        e->update(fElapsedTime);
    }
}

void Map::render(float *depth_buffer) {
    for (int y = 0; y < SCR_H; y++) {
        for (int x = 0; x < SCR_W; x++) {
        	// For each column, calculate the projected ray angle into world space
        	float ray_a = (entities[0]->a - FOV/2.0f) + ((float)x / (float)SCR_W) * FOV;
        	float dist = 0.0f;
            olc::vi2d map_check;
        	bool hit = false;
        	int id = castray({ entities[0]->x, entities[0]->y }, { sinf(ray_a), cosf(ray_a) }, dist, hit, this, map_check);
            // determine where ray has hit wall
            olc::vf2d sample_point;
            olc::vf2d block_mid = { (int)map_check.x + 0.5f, (int)map_check.y + 0.5f };
            olc::vf2d test_point = { entities[0]->x + sinf(ray_a) * dist, entities[0]->y + cosf(ray_a) * dist };

            float test_angle = atan2f((test_point.y - block_mid.y), (test_point.x - block_mid.x));
            if (test_angle >= (-PI*0.25f) && test_angle < (PI*0.25f))
                sample_point.x = test_point.y - (float)map_check.y;
            if (test_angle >= (PI*0.25f) && test_angle < (PI*0.75f))
                sample_point.x = test_point.x - (float)map_check.x;
            if (test_angle < (-PI*0.25f) && test_angle >= (-PI*0.75f))
                sample_point.x = test_point.x - (float)map_check.x;
            if (test_angle >= (PI*0.75f) || test_angle < (-PI*0.75f))
                sample_point.x = test_point.y - (float)map_check.y;

        	// Calculate distance to ceiling and floor
            float ca = ray_a - entities[0]->a;
            if (ca < 0) {
                ca += 2*PI;
            }
            if (ca>2*PI) {
                ca -= 2*PI;
            }
            float fd = dist * cosf(ca);
        	int ceil = (float)(SCR_H/2.0) - SCR_H / fd;
        	int floor = SCR_H - ceil;
            depth_buffer[x] = fd;
            // Each Row
            if (y <= ceil) {
                engine->Draw(x, y, olc::CYAN);
            }
    	    else if (y > ceil && y <= floor) {
                if (id) {
                    sample_point.y = ((float)y - (float)ceil) / ((float)floor - (float)ceil);
                    float dark = std::clamp(dist, 1.0f, 255.0f);
                    olc::Pixel sprtcol = TileManager::get().get_tile(id)->tex->Sample(sample_point.x, sample_point.y);
    	    	    olc::Pixel col = olc::Pixel(sprtcol.r / dark, sprtcol.g / dark, sprtcol.b / dark);
                    if (dist < RENDER_DIST)
    	    	        engine->Draw(x, y, col);
                }
            }
    	    else if (y > floor) {			
    	    	float b = 255 - std::clamp(255.0f - 80 * (((float)y - SCR_H / 2.0f) / ((float)SCR_H / 2.0f)), 0.0f, 255.0f);
    	    	engine->Draw(x, y, olc::Pixel(b/3, b/3, b/3));
    	    }
        }
    }

    for (auto &e : entities) {
        e->render(depth_buffer);
    }
}