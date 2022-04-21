#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <olcPixelGameEngine.h>
#include "../definitions.hh"
#include "../map/map.hh"
class Component;

class Entity {
public:
    Entity();

public:
    float x = 0, y = 0, a = 0, speed = 2.5f; // position & angle
    std::vector<Component*> components;
    Map *current;

    void start();
    void update(float fElapsedTime);
    void render(float *depth_buffer);

    Entity* setpos(float x, float y);
    Entity* setangle(float a);
    Entity* setspeed(float s);
    Entity* add_component(Component *component);
};

#endif