#include "HumanPlayer.h"
#include "Player.h"
#include "map.h"
#include "PhaseSubject.h"
#include "PhaseObserver.h"
#include "StatSubject.h"
#include "StatObserver.h"


//***********CREATING NEW PHASE SUBJECT AND OBSERVER*************//
PhaseSubject* phase = new PhaseSubject();
PhaseObserver* ob = new PhaseObserver(phase);
vector<string>* log = phase->getLog();

void HumanPlayer::rollDice(Player *p) {

    map<int, Dice>& dice = p->getDice();
    int count = 1;
    string input = "1-2-3-4-5-6";
    char c;
    phase->setCurrentPlayer(p->getName());
    phase->setCurrentStep(3);
    phase->NotifyAll();
    do{
        log->clear();

        cout << "Roll " << count << " dice status:" << endl;
        for (auto i : input){
            if (i != '-'){
                dice[i-'0'].roll();
            }
        }

        for (auto i : dice)
            cout << "Dice" << i.first << "-" << i.second.getState() << "\t";
        cout << endl;

        if (count == 3){
            cout << "This is the final dice rolling results.\n";
            cout << "No more rerolls.\n";
            break;
        }

        cout << endl << "Would you like to roll another time? (Y/N) : ";
        cin >> c;
        c = toupper(c);

        // TODO: Minor bug for input validation
        while(c != 'Y' && c != 'N') {
            cout << "Bad input. Please enter 'Y' or 'N': ";
            cin >> c;
            c = toupper(c);
        }

        if (c == 'Y'){
            cout << "Which dice(s) would you like to reroll?\n";
            cout << "To reroll deice 1, dice 2 and dice 3, enter 1-2-3\n";
            cout << "(Note the order does not matter)\n";
            cout << "Enter the dice# you want to reroll: ";

            // TODO: Implement input validation
            cin >> input;
            log->push_back("Reroll #" + to_string(count));
            log->push_back("Player has chosen to reroll " + input);
            phase->NotifyAll();
            log->clear();
            count++;
        }
        else {
            log->push_back("Player has chosen NOT to reroll");
            phase->NotifyAll();
            log->clear();
            break;
        }

    }while(count <= 3);
}

void HumanPlayer::resolveDice(Player* p, vector<Player>* players, Map* m){

    map<string, int> d_counts = p->dice_effect_count();
    int &energy_cube = p->getEnergyCube();
    Region *region = p->getRegion();
    int &life_point = p->getHP();

    cout << endl;
    /**********************************************************************************/
    /*********************************Resolve Energy***********************************/
    /**********************************************************************************/
    //PHASE PATTERN
    log->clear();
    //phase->setCurrentPlayer(p->getName());
    phase->setCurrentStep(1);

    int eCubes = d_counts["Energy"];
    if (eCubes != 0){

        //cout << "Resolving Energy." << endl;
        energy_cube += eCubes;
        //cout << "You earned " << eCubes << " energy cubes." << endl;
        //cout << "Your total energy cubes is now " << energy_cube << endl << endl;
        log->push_back("Player has earned "+ to_string(eCubes) + " energy cubes.");
        log->push_back("Player's total energy cubes is now " + to_string(energy_cube));
        phase->NotifyAll();
        log->clear();
    }

    /**********************************************************************************/
    /*********************************Resolve Attack***********************************/
    /**********************************************************************************/
    log->clear();
    phase->setCurrentPlayer(p->getName());
    phase->setCurrentStep(2);
    int damage = d_counts["Attack"];
    if (damage != 0) {
        //PHASE PATTERN

        log->push_back("Resolving Attack.");
        phase->NotifyAll();
        // If player is in Manhattan
        // Each Attack he rolls inflict damage to all Monsters outside Manhattan
        if (region->name == "MANHATTAN") {

            cout << p->getName() << "'s current region is MANHATTAN. Inflicting damage to all monsters outside MANHATTAN." << endl;

            //FIXME This for loop can be simplified with ==Operator
            for (auto &i : *players) {
                if (i.getName() != p->getName()) {

                    // Diminish opponent's HP and set damaged flag to true
                    i.take_damage(damage);
                    //cout << i.getName() << " has taken " << damage << " damage in " << i.getRegion()->name << endl;
                    log->push_back(i.getName() + " has taken " + to_string(damage) + " damage in " + i.getRegion()->name);
                    phase->NotifyAll();
                    log->clear();
                }
            }
        }
            // If player is not in Manhattan
            // Each Attack he rolls inflict damage to all Monsters in Manhattan
        else {
            log->push_back(p->getName() +"'s current region is " + region->name + ". Inflicting damage to all monsters inside MANHATTAN.");
            phase->NotifyAll();

            //cout << p->getName() << "'s current region is " << region->name << ". Inflicting damage to all monsters inside MANHATTAN." << endl;
            for (auto &i : *players) {
                if (i.getRegion()->name == "MANHATTAN") {

                    // Deal damage to monster in Manhattan
                    i.take_damage(damage);
                    log->push_back(i.getName() + " has taken " + to_string(damage) + " damage in " + i.getRegion()->name);
                    phase->NotifyAll();
                    //cout << i.getName() << " has taken " << damage << " damage in " << i.getRegion()->name << endl;

                    // Pursue only if the monster is not dead
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
                        //cout << "*******************************************************" << endl;
                        //cout << "Continuing with " << p->getName() << "'s turn...\n";
                        log->push_back("*******************************************************");
                        log->push_back("Continuing with " + p->getName() + "'s turn...\n");
                        phase->NotifyAll();
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
        //cout << "Resolving Heal." << endl;
        phase->setCurrentStep(9);

        // Do not heal if player is in Manhattan
        if (region->name == "MANHATTAN"){
            //cout << p->getName() << " is in Manhattan. Cannot heal." << endl;
            log->push_back(p->getName() + " is in Manhattan. Cannot heal.");
            phase->NotifyAll();
        }
            //Do not heal if player's hp is full
        else if (life_point == 10){
            //cout << p->getName() << "'s life point is full. Cannot heal." << endl;
            log->push_back(p->getName() + "'s life points are full. Cannot heal.");
            phase->NotifyAll();
        }
        else{
            // We don't want HP go over 10
            hp = min(10-life_point, hp);
            life_point += hp;
            //cout << p->getName() << " has healed " << hp << endl;
            //cout << p->getName() << "'s life point is now " << life_point << endl;
            log->push_back(p->getName()+ " has healed " + to_string(hp));
            log->push_back(p->getName()+ "'s life point is now " + to_string(life_point));
            phase->NotifyAll();
            log->clear();
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
        //cout << "Resolving Destruction," << endl;
        //PHASE PATTERN
        phase->setCurrentPlayer(p->getName());
        phase->setCurrentStep(10);

        // Display tiles on the current region
        if (region->tile.size() == 0){
            //cout << "All tile units are destroyed on the current region.\n";
            log->push_back("All tile units are destroyed on the current region.\n");
            phase->NotifyAll();
        }
        else{
            // Loop as long as you have enough Destruction points to destroy an tile unit
            while (true){
                //cout << "Your total Destruction points are: " << destruction << endl;

                log->push_back("Player's total Destruction points are: " + to_string(destruction) +"\n");
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
                            durability = min(durability, tiles[i].getDurability());

                            pair<string, int> reward = tiles[i].getReward();
                            /*cout << endl << "Tile " << i << " - " << "Name:" << tiles[i].getTileName() << " Durability:"
                                 << tiles[i].getDurability()
                                 << " Reward:" << reward.first << "-" << reward.second;*/
                            log->push_back("Destroyable tile units on the current borough are:");
                            log->push_back("Tile " + to_string(i) + " - " + "Name:" + tiles[i].getTileName() + " Durability:" + to_string(tiles[i].getDurability()) + " Reward:" + reward.first + "-" + to_string(reward.second));


                            allFlipped = false;
                        }
                    }
                }
                phase->NotifyAll();
                if (destruction < durability){
                    /*cout << " None.\n";
                    cout << "You do not have enough Destruction points to destroy anything." << endl;*/
                    log->push_back("Destroyable tile units on the current borough are:");
                    log->push_back("None.\n You do not have enough Destruction points to destroy anything.\n");
                    phase->NotifyAll();
                    break;
                }
                else{
                    if (allFlipped){
                        cout << endl << "All military units are revealed in this turn. Cannot be destroyed." << endl;
                        break;
                    }

                    cout << endl << "It appears you can destroy at least one unit. You must do so." << endl;
                    cout << "Which tile unit you want to destroy? (Enter the number): ";
                    int tile_num;
                    cin >> tile_num;

                    Tile &tile = region->tile[tile_num];

                    // Subtract player's Destruction point from selected tile durability
                    destruction -= tile.getDurability();

                    // Flip the tile if it's a building tile
                    if (tile.getTileName() == "Building"){

                        //TODO: Get rewards
                        cout << "Building unit is destroyed." << endl;
                        log->push_back("Player has destroyed " + region->tile[tile_num].getTileName());
                        phase->NotifyAll();
                        cout << "Getting rewards..." << endl;

                        // Flip the tile
                        tile.flip();

                        // Set tile name and durability of military unit according to previous Building durability
                        switch (tile.getDurability()){
                            case 1:
                                tile.setTileName("Infantry");
                                tile.setDurability(2);
                                cout << "An new Infantry unit has been revealed." << endl;
                                log->push_back("An new Infantry unit has been revealed.");
                                break;
                            case 2:
                                tile.setTileName("Jet");
                                tile.setDurability(3);
                                cout << "An new Jet unit has been revealed." << endl;
                                log->push_back("An new Jet unit has been revealed.");
                                break;
                            case 3:
                                tile.setTileName("Tank");
                                tile.setDurability(4);
                                cout << "An new Tank unit has been revealed." << endl;
                                log->push_back("An new Tank unit has been revealed.");
                                break;
                        }
                        phase->NotifyAll();
                        cout << endl;
                    }
                        // Remove the tile if it's a military unit
                    else{
                        region->tile.erase(region->tile.begin() + tile_num);
                        /*cout << "Military unit has been destroyed." << endl;
                        cout << "Getting rewards." << endl;*/
                        log->push_back("Military unit has been destroyed.\nGetting rewards." );
                        phase->NotifyAll();
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

void HumanPlayer::move(Player* p, vector<Player>* players, Map* m){
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

        // If there is no one in Manhattan you must move there.
        // When you arrive in Manhattan, you must place your Monster on Lower Manhattan on the 2–4 space.
        if (empty_manhattan){
            cout << "Manhattan is empty. Moving " << name << " to Manhattan.\n";
            m->getRegion("MANHATTAN")->name = region->name;
            region->name = "MANHATTAN";
        }
            // If there is already 1 Monster in any zone of Manhattan, you
            // have two options: You can move to any borough that does't
            // already have 2 Monsters in it (except Manhattan), or you can
            // just stay in your borough.
        else{
            char stay;
            cout << "Manhattan is not empty.\n";
            cout << "Stay in your borough? (y/n) : ";
            cin >> stay;
            if (stay == 'n'){
                // Moving to a region that does not already contain 2 players
                change_region(m, p);
            }
        }
    }
}

void HumanPlayer::buyCards(Player* p, Deck<Card>* deck){
    log->clear();
    //PHASE OBSERVER PATTERN
    //phase->setCurrentPlayer(p->getName());
    phase->setCurrentStep(6);
    phase->NotifyAll();
    vector<Card> *cards = deck->getDeck();
    int &energy_cube = p->getEnergyCube();

    if (cards->size() < 3 ) return;

    //int min_cost = 100;
    char answer;

    //*****************Print top 3 cards*************//
    cout << "\n......Here are the available Cards....\n";
    for (int i=0; i<3; i++){
        cout << "Card #" << i << " -- " << (*cards)[i];
    }

    //*****************Discarding Cards to get NEW ones*************//
    if (energy_cube < 2){
        cout << "You don't have enough energy cubes to discard the cards.\n";
    }
    else{
        cout << "You have " << energy_cube << " Energy Cubes.." << endl;

        do {
            cout << endl << "Would you like to discard 3 available Cards and reveal 3 NEW ones? [2 Energy Cubes] (Y/N) : ";
            cin >> answer;
            answer = toupper(answer);

            while (answer != 'Y' && answer != 'N') {
                cout << endl << "Invalid input. Please enter 'Y' or 'N': ";
                cin >> answer;
                answer = toupper(answer);
            }

            if (answer == 'Y') {
                energy_cube -= 2;
                cards->erase(cards->begin(), cards->begin()+3);

                // Deal new cards
                cout << "Here are the NEW available cards\n\n";

                for (int i=0; i<3; i++){
                    cout << "Card #" << i << " -- " << (*cards)[i];
                }

                log->push_back("Player has chosen to discard the 3 available cards\n3 new cards were dealt");
                phase->NotifyAll();
            }
        } while (answer == 'Y' && energy_cube >= 2 && cards->size() >= 3);
    }

    //*****************Allowing Player to BUY a Card*************//
    if (energy_cube < 1) {
        cout << "You do not have enough Energy Cubes to buy anything at this time...\n";
    }
    else{
        cout << "Would you like to buy a Card? (Y/N) : ";
        cin >> answer;
        answer = toupper(answer);

        while (answer != 'Y' && answer != 'N') {
            cout << "Invalid input. Please enter 'Y' or 'N': ";
            cin >> answer;
            answer = toupper(answer);
        }

        if (answer == 'Y') {
            cout << "Enter the Card# of the card you wish to buy: ";
            int buyIndex;
            cin >> buyIndex;

            energy_cube = (*cards)[buyIndex].getCost();
            p->getCard().push_back((*cards)[buyIndex]);
            cards->erase(cards->begin()+buyIndex);

            // Reveal a new card
            cout << "Revealing new card.." << endl;

            cout << "Here are the NEW available cards\n\n";
            for (int i=0; i<3; i++){
                cout << "Card #" << i << " -- " << (*cards)[i];
            }

            cout << endl << "Would you like to buy another Card? (Y/N) : ";
            cin >> answer;
            answer = toupper(answer);
            while (answer != 'Y' && answer != 'N') {
                cout << "Invalid input. Please enter 'Y' or 'N': ";
                cin >> answer;
                answer = toupper(answer);
            }
        }
    }
}