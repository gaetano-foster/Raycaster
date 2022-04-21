#ifndef COMPONENT_H
#define COMPONENT_H
class Entity;

class Component {
public:
    Entity *e;

    virtual void start()=0;
    virtual void update(float fElapsedTime)=0;
    virtual void render(float *depth_buffer)=0;
};

#endif