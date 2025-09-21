#include "../headers/entities.hpp"

// ---- Entity ----
Entity::Entity(const char *name, Hitbox hb, Color hitboxColor) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strncpy(this->name, name, MAX_NAME_LENGTH);
    this->name[MAX_NAME_LENGTH - 1] = '\0';

    this->hitbox = hb;

    this->hitboxColor = hitboxColor;
}

/*
    /!\ Fonction vulnérable avec strcpy /!\
*/
void Entity::changeName(const char *newName) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strcpy(this->name, newName);
}

// ---- PNJ ----
PNJ::PNJ(const char *name, Hitbox hb, Color hitboxColor) : Entity(name, hb, hitboxColor) {}

// ---- Player ----
Player::Player(const char *name, Hitbox hb, Color hitboxColor) : Entity(name, hb, hitboxColor) {}

void Player::displayInfos() {
    cout << endl;
    cout << "Nom du joueur : " << this->getName() << endl;
    cout << "Position du joueur : X=" << this->getHitbox().pos.x << " ; Y=" << this->getHitbox().pos.y << endl;
    this->inv.display();
    cout << endl;
}
