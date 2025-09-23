#include "entity.hpp"

// ---- Entity ----
Entity::Entity(const char *name, Hitbox hb) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strncpy(this->name, name, MAX_NAME_LENGTH);
    this->name[MAX_NAME_LENGTH - 1] = '\0';

    this->hitbox = hb;
}

/*
    /!\ Fonction vulnérable avec strcpy /!\
*/
void Entity::changeName(const char *newName) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strcpy(this->name, newName);
}
