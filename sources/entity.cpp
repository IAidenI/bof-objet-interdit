#include "entity.hpp"

// ---- Entity ----
Entity::Entity(const char *name, Hitbox hb, const char *texture) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strncpy(this->name, name, MAX_NAME_LENGTH);
    this->name[MAX_NAME_LENGTH - 1] = '\0';

    this->hitbox = hb;

    this->texture = LoadTexture(texture);
}

Entity::Entity(Entity&& other) noexcept : hitbox(other.hitbox), texture(other.texture) {
    memcpy(name, other.name, MAX_NAME_LENGTH);
    other.texture = {0};
}

Entity& Entity::operator=(Entity&& other) noexcept {
    if (this != &other) {
        if (texture.id != 0) UnloadTexture(texture);
        hitbox  = other.hitbox;
        texture = other.texture;
        memcpy(name, other.name, MAX_NAME_LENGTH);
        other.texture = {0};
    }
    return *this;
}

Entity::~Entity() {
    if (texture.id != 0) UnloadTexture(texture);
}

/*
    /!\ Fonction vulnérable avec strcpy /!\
*/
void Entity::changeName(const char *newName) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strcpy(this->name, newName);
}
