#include "AggressiveComputer.h"
#include "Player.h"

void AggressiveComputer::rollDice(Player *p) {

    map<int, Dice>& dice = p->getDice();
    int count = 1;
    string reroll = "1-2-3-4-5-6";
    char c;

    do{
        cout << "Roll " << count << " dice status:" << endl;
        for (auto i : reroll){
            if (i != '-'){
                dice[i-'0'].roll();
            }
        }

        // Reset reroll
        reroll = "";

        // Reroll those that are not Attack and Destruction
        for (auto i : dice){
            cout << "Dice" << i.first << "-" << i.second.getState() << "\t";

            if (i.second.getState() != "Attack" && i.second.getState() != "Destruction"){
                reroll += to_string(i.first)+"-";
            }
        }
        cout << endl << endl;

        if (count == 3 || reroll.empty()){
            cout << "This is the final dice rolling results.\n";
            cout << "No more rerolls.\n";
            break;
        }

        count++;

    }while(count <= 3);
}

void AggressiveComputer::resolveDice(Player* p, vector<Player>* players, Map* m){

    map<string, int> d_counts = p->dice_effect_count();
    int &energy_cube = p->getEnergyCube();
    Region *region = p->getRegion();
    int &life_point = p->getHP();
    string player_name = p->getName();

    cout << endl;
    /**********************************************************************************/
    /*********************************Resolve Energy***********************************/
    /**********************************************************************************/
    int eCubes = d_counts["Energy"];
    if (eCubes != 0){
        cout << "Resolving Energy." << endl;
        energy_cube += eCubes;
        cout << player_name << " earned " << eCubes << " energy cubes." << endl;
        cout << player_name << "'s total energy cubes is now " << energy_cube << endl << endl;
    }

    /**********************************************************************************/
    /*********************************Resolve Attack***********************************/
    /**********************************************************************************/
    int damage = d_counts["Attack"];
    if (damage != 0) {
        cout << "Resolving Attack." << endl;

        // If player is in Manhattan
        // Each Attack he rolls inflict damage to all Monsters outside Manhattan
        if (region->name == "MANHATTAN") {

            cout << player_name << "'s current region is MANHATTAN. Inflicting damage to all monsters outside MANHATTAN." << endl;

            //FIXME This for loop can be simplified with ==Operator
            for (auto &i : *players) {
                if (i.getName() != player_name) {

                    // Diminish opponent's HP and set damaged flag to true
                    i.take_damage(damage);
                    cout << i.getName() << " has taken " << damage << " damage in " << i.getRegion()->name << endl;
                }
            }
        }
        // If player is not in Manhattan
        // Each Attack he rolls inflict damage to all Monsters in Manhattan
        else {
            cout << player_name << "'s current region is " << region->name << ". Inflicting damage to all monsters inside MANHATTAN." << endl;
            for (auto &i : *players) {
                if (i.getRegion()->name == "MANHATTAN") {

                    // Deal damage to monster in Manhattan
                    i.take_damage(damage);
                    cout << i.getName() << " has taken " << damage << " damage in " << i.getRegion()->name << endl;

                    // Assume computer won't move out after taking damage in Manhattan
                    if (i.getHP() > 0 && i.getName().find("Computer") == string::npos){
                        char c;
                        cout << "*******************************************************" << endl;
                        cout << "              " << i.getName() << "              " << endl;
                        cout << i.getName() << " has been damaged while in Manhattan." << endl;
                        cout << "Wanna move out? (y/n): ";
                        cin >> c;
                        if (c == 'y'){
                            change_region(m, &i);
                        }
                        cout << "*******************************************************" << endl;
                        cout << "Continuing with " << player_name << "'s turn...\n";
                        // Assume we can only have one monster in Manhattan
                        break;
                    }
                }
            }
        }
        cout << endl;
    }

    /**********************************************************************************/
    /*********************************Resolve Heal*************************************/
    /**********************************************************************************/
    int hp = d_counts["Heal"];
    if (hp != 0){
        cout << "Resolving Heal." << endl;

        // Do not heal if player is in Manhattan
        if (region->name == "MANHATTAN"){
            cout << player_name << " is in Manhattan. Cannot heal." << endl;
        }
            //Do not heal if player's hp is full
        else if (life_point == 10){
            cout << player_name << "'s life point is full. Cannot heal." << endl;
        }
        else{
            // We don't want HP go over 10
            hp = min(10-life_point, hp);
            life_point += hp;
            cout << player_name << " has healed " << hp << endl;
            cout << player_name << "'s life point is now " << life_point << endl;
        }
        cout << endl;
    }

    /**********************************************************************************/
    /*********************************Resolve Destruction******************************/
    /**********************************************************************************/

    // You cannot destroy a Unit on the same turn that it appears.
    // If you have enough Destruction to destroy a Building or an Unit, you must
    // to do so; however you are not required to optimize your destruction.
    int destruction = d_counts["Destruction"];

    if (destruction != 0){
        cout << "Resolving Destruction," << endl;

        // Display tiles on the current region
        if (region->tile.size() == 0){
            cout << "All tile units are destroyed on the current region.\n";
        }
        else{
            // Loop as long as you have enough Destruction points to destroy an tile unit
            while (true){
                cout << player_name << "'s total Destruction points are: " << destruction << endl;
                cout << "Destroyable tile units on the current borough are: ";

                vector<int> unit_num;
                int durability = 5;
                bool allFlipped = true; // Flag indicating whether all military units appeared in the same turn
                vector<Tile> tiles = region->tile;
                for (int i=0; i < tiles.size(); i++) {

                    // Quit if we have already revealed 3 tiles
                    if (i == 3) {
                        break;
                    } else {
                        // Only get the information if the tile is not flipped in the same turn and its durability is less or equal than the player's Destruction points
                        if (!tiles[i].isFlipped() && destruction >= tiles[i].getDurability()) {
                            unit_num.push_back(i);

                            durability = min(durability, tiles[i].getDurability());

                            pair<string, int> reward = tiles[i].getReward();
                            cout << endl << "Tile " << i << " - " << "Name:" << tiles[i].getTileName() << " Durability:"
                                 << tiles[i].getDurability()
                                 << " Reward:" << reward.first << "-" << reward.second;

                            allFlipped = false;
                        }
                    }
                }

                if (destruction < durability){
                    cout << " None.\n";
                    cout << player_name << " does not have enough Destruction points to destroy anything." << endl;
                    break;
                }
                else{
                    if (allFlipped){
                        cout << endl << "All military units are revealed in this turn. Cannot be destroyed." << endl;
                        break;
                    }

                    cout << endl << "It appears " << player_name << " can destroy at least one unit." << endl;
                    int tile_num = unit_num[0];

                    Tile &tile = region->tile[tile_num];

                    // Subtract player's Destruction point from selected tile durability
                    destruction -= tile.getDurability();

                    // Flip the tile if it's a building tile
                    if (tile.getTileName() == "Building"){

                        //TODO: Get rewards
                        cout << "Building unit is destroyed." << endl;
                        cout << "Getting rewards..." << endl;

                        // Flip the tile
                        tile.flip();

                        // Set tile name and durability of military unit according to previous Building durability
                        switch (tile.getDurability()){
                            case 1:
                                tile.setTileName("Infantry");
                                tile.setDurability(2);
                                cout << "An new Infantry unit has been revealed." << endl;
                                break;
                            case 2:
                                tile.setTileName("Jet");
                                tile.setDurability(3);
                                cout << "An new Jet unit has been revealed." << endl;
                                break;
                            case 3:
                                tile.setTileName("Tank");
                                tile.setDurability(4);
                                cout << "An new Tank unit has been revealed." << endl;
                                break;
                        }
                        cout << endl;
                    }
                    // Remove the tile if it's a military unit
                    else{
                        region->tile.erase(region->tile.begin() + tile_num);
                        cout << "Military unit has been destroyed." << endl;
                        cout << "Getting rewards." << endl;
                    }
                }
            }

            // Unset the flipped flag for the tiles since the turn is ended
            for (vector<Tile>::iterator it = region->tile.begin(); it != region->tile.end(); it++) {
                if ((*it).isFlipped()){
                    (*it).flip();
                }
            }
        }
        cout << endl;
    }
}

void AggressiveComputer::move(Player* p, vector<Player>* players, Map* m){
    string new_region;
    Region *region = p->getRegion();
    string name = p->getName();

    // If you were already in Manhattan, advance to the 2–4 space in the next zone up in Manhattan.
    if (region->name == "MANHATTAN"){
        // TODO: Advance to next 2-4 in manhattan
        cout << "Moving to next 2-4 Manhattan.\n";
    }
    else{
        bool empty_manhattan = true;
        for (auto &p : *players){
            if (p.getRegion()->name == "MANHATTAN")
                empty_manhattan = false;
        }

        // Move to Manhattan if it's empty.
        // When you arrive in Manhattan, place the Monster on Lower Manhattan 2–4
        if (empty_manhattan){
            cout << "Manhattan is empty. Moving " << name << " to Manhattan.\n";
            m->getRegion("MANHATTAN")->name = region->name;
            region->name = "MANHATTAN";
        }
            // If there is already 1 Monster in any zone of Manhattan, computer always stays in its current region
        else{
            cout << "Manhattan is not empty.\n";
            cout << "Stay in current borough: " << region->name;
        }
    }
}

void AggressiveComputer::buyCards(Player* p, Deck<Card>* deck){

    vector<Card> *cards = deck->getDeck();

    if (cards->size() < 3 ) return;

    //*****************Print top 3 cards*************//
    cout << "\n......Here are the available Cards....\n";
    for (int i=0; i<3; i++){
        cout << "Card #" << i << " -- " << (*cards)[i];
    }

    cout << p->getName() << " is not buying any cards.\n";
}