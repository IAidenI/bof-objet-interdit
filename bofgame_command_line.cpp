#include "entity.hpp"
#include "inventory.hpp"
#include "player.hpp"
#include "game.hpp"
#include "gdb.hpp"

int main() {
    char buffer[BUFFER_SIZE];
    cout << "Saisir votre nom: " << endl;
    cin.getline(buffer, BUFFER_SIZE);

    Item potato = getItem(POTATO);
    Item carrot = getItem(CARROT);
    Item apple  = getItem(APPLE);

    Vector2 pos = {25.0, 25.0};
    Hitbox hb = {pos, 50};
    Player player(buffer, hb);

    player.inventory().add(potato, MAX_POTATO);
    player.inventory().add(carrot, MAX_CARROT);
    player.inventory().add(apple, MAX_APPLE);

    player.inventory().add(carrot, MAX_CARROT);
    player.inventory().add(potato, MAX_POTATO);
    player.inventory().add(apple, MAX_APPLE);

    player.inventory().add(carrot, MAX_CARROT);
    player.inventory().add(apple, MAX_APPLE);
    player.inventory().add(potato, MAX_POTATO);

    player.displayInfos();

    //player.inventory().remove(potato, 20);

    // Changement de nom (vulnérabilité)
    cout << "Saisir votre nouveau nom: " << endl;
    cin.getline(buffer, BUFFER_SIZE);
    player.inventory().changeItemName(buffer, 0);

    player.displayInfos();

    // Pour gagner il suffit de rentrer le nouveau nom CCCCCCCCCCCCCCCCC
    if (player.inventory().hasEnoughOf(ID_APPLE, AMOUNT_TO_FINISH_GAME)) {
        cout << "Vous avez (x" << player.inventory().getItemQuantity(ID_APPLE) << ") " << apple.getName() << ". Requis : " << AMOUNT_TO_FINISH_GAME << ", vous avez gagné." << endl;
    } else {
        cout << "Vous avez (x" << player.inventory().getItemQuantity(ID_APPLE) << ") " << apple.getName() << ". Requis : " << AMOUNT_TO_FINISH_GAME << ", vous avez perdu." << endl;
    }

    cout << endl;

    // Récupère et affiche la stack
    GDB gdb(reinterpret_cast<uintptr_t>(__builtin_frame_address(0)), 0x1c0);
    gdb.displayStack();
    
    return 0;
}
