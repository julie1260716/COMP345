#include "game.h"
#include "dice.h"
#include "player.h"
#include "region.h"
#include "AggressiveComputer.h"
#include "HumanPlayer.h"
#include "ModerateComputer.h"
#include <fstream>
#include <math.h>
#include <algorithm>
#include "PhaseSubject.h"
#include "PhaseObserver.h"
#include "StatSubject.h"
#include "StatObserver.h"
#include <iostream>

// Monsters
map<int, string> monsters = {{1, "Captain Fish"}, {2, "Sheriff"}, {3, "Kong"}, {4, "Rob"}, {5, "Mantis"}, {6, "Drakonis"}};

// Regions
map<int, string> regions = {{1, "BRONX"}, {2, "QUEENS"}, {3, "BROOKLYN"}, {4, "STATEN-ISLAND"}};

// Player Types
map<int, string> player_types = {{1, "Human Player"}, {2, "Aggressive Computer"}, {3, "Moderate Computer"}};


int main()
{
    srand(time(NULL));

    //***********CREATING NEW PHASE SUBJECT AND OBSERVER*************//
    PhaseSubject* phase = new PhaseSubject();       //create a new SUBJECT: PHASE
    PhaseObserver* ob = new PhaseObserver(phase);   //create a new OBJECT: OB (that will observe the phase subject)
    vector<string>* log = phase->getLog();          //Log the dynamic events
    //***********CREATING NEW STATISTIC SUBJECT AND OBSERVER*************//
    StatSubject* stats = new StatSubject();         //create a new SUBJECT: STATS
    StatObserver* obj = new StatObserver(stats);    //create a new OBJECT: OBJ (that will observe the stats subject)



    cout << "Enter the file name for your map: ";
    string name;
    cin >> name;
    Game game(name);

    size_t num_of_players;
    cout << "Welcome to " << game.getMapName() << endl;
    cout << "Choose the number of players you wish to create (Min-2, Max-6): ";
    cin >> num_of_players;


    vector<Player> players;
    //***********HOLDS THE LIST OF CURRENT PLAYERS (SUBJECTS)*************//
    vector<Player>* ptr = &players;     //pointer to list of current players
    stats->setPlayers(ptr);             //sets the subject to the current list of player

    for (int i=1; i<=num_of_players; i++){
        int monster_number, region_number, player_type_number;
            //PHASE OBSERVER PATTERN
            phase->setCurrentPlayer(to_string(i));
            phase->setCurrentStep(8);
            phase->NotifyAll();
        cout << "Choose Monster for Player" << i << endl;
        for (auto i : monsters){
            cout << i.first << "=" << i.second << "\n";
        }
        cout << "Enter monster number for Player" << i << ":";
        cin >> monster_number;
            //PHASE OBSERVER PATTERN
            log->push_back("Player has chosen the Monster: " + monsters[monster_number] );
            phase->NotifyAll();
        Region *r;
        cout << "Choose Region for Player" << i << endl;
        for (auto i : regions){
            cout << i.first << "=" << i.second << "\n";
        }
        cout << "Enter region number for Player" << i << ":";
        cin >> region_number;

        r = game.find_region(regions[region_number]);

        if (r){
            Player p = Player(monsters[monster_number], r);
            // Creating player
            players.push_back(p);
            //adding players to the PLAYER LIST
            stats->NotifyAll();
            r->num_of_players++;
        }
        else{
            cout << "Region does not exist.";
            exit(1);
        }

        // Erase the chosen monster from map since one monster can only be chosen once
        monsters.erase(monster_number);

        // Erase the region from map if that region contains two players
        if (r->num_of_players == 2)
            regions.erase(region_number);
            //PHASE OBSERVER PATTERN
            log->push_back("Player has chosen the Region: " + r->name );
            phase->NotifyAll();
        // Choose player type
        // HumanPlayer, AggressiveComputer, ModerateComputer
        cout << "Choose player type for Player" << i << endl;
        for (auto i : player_types){
            cout << i.first << "=" << i.second << "\n";
        }
        cout << "Enter player type number for Player" << i << ":";
        cin >> player_type_number;
        string type;

        switch(player_type_number){
            case 1:
                players[i-1].setStrategy(new HumanPlayer());
                type = "Human Player";
                break;
            case 2:
                players[i-1].setStrategy(new AggressiveComputer());
                players[i-1].addComputerName(" [AggressiveComputer]");
                type = "Aggressive Computer";
                break;
            case 3:
                players[i-1].setStrategy(new ModerateComputer());
                players[i-1].addComputerName(" [ModerateComputer]");
                type = "Moderate Computer";
                break;
        }
        //PHASE OBSERVER PATTERN
        log->push_back("Player has chosen the Type: " +  type);
        phase->NotifyAll();
        //PHASE PATTERN CLEAR LOG
        log->clear();

    }//end of for loop
    cout << "Rolling first round dices" << endl;

    for (auto &p : players){
        cout << "-------------------------------------------------------------------------------\n";
        cout << p.getName() << " rolling dices...\n";

        // Check if player is computer
        if (p.getStrategy()){

        }
        p.firstRoll();
        cout << "Black dices result: \n";
        for (auto i : p.getDice())
            cout << "Dice" << i.first << "-" << i.second.getState() << "\t";
        cout << endl << endl;
        cout << "Green dices result: \n";
        for (auto i : p.getGreenDice())
            cout << "Dice" << i.first << "-" << i.second.getState() << "\t";
        cout << endl << endl;
        cout << "Total number of Attacks: " << p.countAttacks() << endl << endl;
    }

    cout << "**********************************************************************************\n";
    cout << "The start order is:\n";
    sort(players.begin(), players.end());

    cout << players[0].getName();
    for (vector<Player>::iterator it = players.begin()+1; it != players.end(); it++)
        cout << "->" << it->getName();
    cout << endl << endl;

    // Main loop, ends only if one player remains in the game
    while (players.size() > 1){
        for (auto &p : players){
            // Roll dice
            cout << "-------------------------------------------------------------------------------\n";


            p.rollDice();

            //PHASE OBSERVER PATTERN: RESOLVE DIE STEP***********
            phase->setCurrentStep(4);
            // Resolve dice
            p.resolveDice(&players, game.getMap());

            for (vector<Player>::iterator it = players.begin(); it != players.end(); it++) {
                // Remove monster if its HP <= 0
                if ((*it).getHP() <= 0){
                    //cout << (*it).getName() << " has no more health points. It has been eliminated from the game.\n";
                    log->push_back((*it).getName() + " has no more health points. It has been eliminated from the game.");
                    phase->NotifyAll();
                    log->clear();
                    (*it).getRegion()->num_of_players--;

                    players.erase(it);
                    it--;   // Because erase shifts the vector
                }
                else{
                    //cout << (*it).getName() << ": HP=" << (*it).getHP() << endl;
                    log->push_back((*it).getName() + ": HP = " + to_string((*it).getHP()));
                    phase->NotifyAll();
                    log->clear();
                }
            }
            cout << endl;

            // Quit the loop if there's only one remaining player
            if (players.size() == 1){
                break;
            }
            //PHASE OBSERVER PATTERN: MOVE STEP
            phase->setCurrentPlayer(p.getName());
            phase->setCurrentStep(5);
            phase->NotifyAll();
            // Move
            p.move(&players, game.getMap());
            //STATS OBSERVER PATTERN
            stats->NotifyAll();


            // Buy cards
            p.buyCards(game.getDeck());
        }
    }
    cout << players[0].getName() << " wins the game!" << endl;
    cout << endl;

    return 0;
}
