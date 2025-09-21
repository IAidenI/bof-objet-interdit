#include "../headers/entities.hpp"

// ---- Entity ----
Entity::Entity(Hitbox hb) {
    this->hitbox = hb;
}

// ---- Player ----
Player::Player(const char *name, Hitbox hb) : Entity(hb) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strncpy(this->name, name, MAX_NAME_LENGTH);
    this->name[MAX_NAME_LENGTH - 1] = '\0';
}

/*
    /!\ Fonction vulnérable avec strcpy /!\
*/
void Player::changeName(const char *newName) {
    memset(this->name, 0, MAX_NAME_LENGTH);
    strcpy(this->name, newName);
}

void Player::displayInfos() {
    cout << endl;
    cout << "Nom du joueur : " << this->name << endl;
    cout << "Position du joueur : X=" << this->getHitbox().pos.x << " ; Y=" << this->getHitbox().pos.y << endl;
    this->inv.display();
    cout << endl;
}
