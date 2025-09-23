#include "player.hpp"

// ---- Player ----
Player::Player(const char *name, Hitbox hb) : Entity(name, hb) {}

void Player::displayInfos() {
    cout << endl;
    cout << "Nom du joueur : " << this->getName() << endl;
    cout << "Position du joueur : X=" << this->getHitbox().pos.x << " ; Y=" << this->getHitbox().pos.y << endl;
    this->inv.display();
    cout << endl;
}
