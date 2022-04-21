#ifndef MAP_H
#define MAP_H
#include <olcPixelGameEngine.h>
#include <vector>
#include "../definitions.hh"
class Entity;

class Map {
public:
    Map(olc::PixelGameEngine *engine);
    Map(int w, int h, olc::PixelGameEngine *engine);
    Map(const char *path, olc::PixelGameEngine *engine);
    ~Map();

public:
    int w = 16, h = 16,   // default to a 16x16 map
        spx = 8, spy = 8; // spawn coordinates default to middle of the map
    olc::PixelGameEngine *engine;
    
    int get_tile(int x, int y);
    int *get_map();
    void add_entity(Entity *e);
    Entity* player();
    
    void start();
    void update(float fElapsedTime);
    void render(float *depth_buffer);

private:
    std::vector<Entity*> entities;
    int *m;
};

#endif