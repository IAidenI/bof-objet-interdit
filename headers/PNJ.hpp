#ifndef PNJ_H
#define PNJ_H

#include "entity.hpp"

// ---- Classe PNJ ----
class PNJ : public Entity {    
    public:
        // Constructeur par défaut
        PNJ() = default;

        PNJ(const char *name, Hitbox hb, Color hitboxColor);
};

#endif // PNJ_H