#include "headers/entities.hpp"
#include "headers/gdb.hpp"

int main() {
    char buffer[BUFFER_SIZE];
    cout << "Saisir votre nom: " << endl;
    cin.getline(buffer, BUFFER_SIZE);

    Item itemX = {ITEM_X, 30};
    Item itemY = {ITEM_Y, 15};

    Position pos = {25, 25};
    Hitbox hb = {pos, 50, 100};
    Player player(buffer, hb);
    
    // Remplit l'inventaire de l'item X
    ReturnCode ret = NONE;
    int stop = 0;
    for (int i = 0; i < 20; i++) {
        auto temp = itemX;
        // Pour être sûr d'avoir mis toute la quantité de l'item
        while (temp.quantity != 0) {
            ret = player.inventory().add(&temp);
            switch (ret) {
                case INVENTORY_ITEM_MAX_REACH:
                    cout << " [ ⚠ ] Limite du slot atteinte." << endl;
                    break;
                case INVENTORY_LENGTH_MAX_REACH:
                    cout << " [ ⚠ ] Inventaire plein en " << i << " itérations." << endl;
                    stop = 1;
                    break;
                default:
                    break;
            }
            if (stop) break;
        }
        if (stop) break;
    }

    // Changement de nom (vulnérabilité)
    cout << "Saisir votre nouveau nom: " << endl;
    cin.getline(buffer, BUFFER_SIZE);
    player.changeName(buffer);
    
    player.displayInfos();

    // Pour gagner il suffit de rentrer le nouveau nom AAAAAAAAAAAAB
    if (player.inventory().hasEnoughOf(itemY, 15)) {
        cout << "Vous avez assez de " << itemY.type << ", vous avez gagné." << endl;
    } else {
        cout << "Vous n'avez pas assez de " << itemY.type << ", vous avez perdu." << endl;
    }

    cout << endl;

    // Récupère et affiche la stack
    GDB gdb(reinterpret_cast<uintptr_t>(__builtin_frame_address(0)), 0xc0);
    gdb.displayStack();
    
    return 0;
}
