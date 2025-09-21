#include "headers/entities.hpp"
#include "headers/game.hpp"
#include "headers/gdb.hpp"

int main() {
    char buffer[BUFFER_SIZE];
    cout << "Saisir votre nom: " << endl;
    cin.getline(buffer, BUFFER_SIZE);

    Item potato = ITEM[POTATO];
    Item carrot = ITEM[CARROT];
    Item apple  = ITEM[APPLE];

    Vector2 pos = {25.0, 25.0};
    Hitbox hb = {pos, 50};
    Player player(buffer, hb, RED);

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
    player.changeName(buffer);

    player.displayInfos();

    // Pour gagner il suffit de rentrer le nouveau nom AAAAAAAAAAAAAAAAC
    if (player.inventory().hasEnoughOf(apple, AMOUNT_TO_FINISH_GAME)) {
        cout << "Vous avez (x" << player.inventory().getItemQuantity(apple) << ") " << apple.name << ". Requis : " << AMOUNT_TO_FINISH_GAME << ", vous avez gagné." << endl;
    } else {
        cout << "Vous avez (x" << player.inventory().getItemQuantity(apple) << ") " << apple.name << ". Requis : " << AMOUNT_TO_FINISH_GAME << ", vous avez perdu." << endl;
    }

    cout << endl;

    // Récupère et affiche la stack
    GDB gdb(reinterpret_cast<uintptr_t>(__builtin_frame_address(0)), 0x170);
    gdb.displayStack();
    
    return 0;
}
