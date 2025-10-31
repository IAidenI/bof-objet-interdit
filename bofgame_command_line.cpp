#include "entity.hpp"
#include "inventory.hpp"
#include "player.hpp"
#include "game.hpp"
#include "gdb.hpp"

int main() {
    Item potato = getItem(CARROT);
    Item apple = getItem(APPLE);

    Vector2 pos = {27.0, 13.0};
    Hitbox hb = {pos, 50};
    Player player("Player", hb);

    for (size_t i = 0; i < MAX_INVENTORY_LENGTH; i++) {
        if (i == MAX_INVENTORY_LENGTH - 1) {
            player.inventory().add(apple, MAX_POTATO);
        } else if (player.inventory().add(potato, MAX_POTATO)) {
            printf("[ WARNING ] Inventaire plein.\n");
        }
    }

    player.displayInfos();

    // Récupère et affiche la stack
    GDB gdb(reinterpret_cast<uintptr_t>(&player));
    gdb.displayStack();
    TextStyle style = { .text = "title", .fontSize = 25.0f, .spacing = 2.0f, .color = RED };
    gdb.displayInfoSegment(gdb.getFormatedStack(style));
    
    return 0;

    /*
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
    GDB gdb(reinterpret_cast<uintptr_t>(&player));
    gdb.displayStack();
    
    return 0;
    */
}
