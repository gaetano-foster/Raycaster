#include "entity.hh"
#include "components/component.hh"

Entity::Entity() {

}

void Entity::start() {
    for (auto c : components)
        c->start();
}

void Entity::update(float fElapsedTime) {
    for (auto c : components)
        c->update(fElapsedTime);
}

void Entity::render(float *depth_buffer) {
    for (auto c : components)
        c->render(depth_buffer);
}

Entity* Entity::setpos(float x, float y) {
    this->x = x;
    this->y = y;
    return this;
}

Entity* Entity::setangle(float a) {
    this->a = a;
    return this;
}

Entity* Entity::setspeed(float s) {
    this->speed = s;
    return this;
}

Entity* Entity::add_component(Component *component) {
    components.push_back(component);
    component->e = this;
    return this;
}