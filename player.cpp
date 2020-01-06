#include "player.h"
#include "map.h"
#include "StatSubject.h"
#include "PhaseSubject.h"
#include "PhaseObserver.h"
#include <iostream>

using namespace std;

void change_region(Map *m, Player *p){
    //***********CREATING NEW PHASE SUBJECT AND OBSERVER*************//
    PhaseSubject* phase = new PhaseSubject();
    PhaseObserver* ob = new PhaseObserver(phase);
    vector<string>* log = phase->getLog();
    //***********CREATING NEW STATS SUBJECT AND OBSERVER*************//
    StatSubject* stats = new StatSubject();
    StatSubject* sub = stats->getSubject();

    cout << "The available boroughs are\n";

    vector<Region *> adj = p->region->adj;

    for (auto i : adj){
        if (i->num_of_players < 2 && i->name != "MANHATTAN"){
            cout << i->name << " - " << i->num_of_players << " monsters.\n";
        }
    }
    sub->NotifyAll();
    cout << "Enter region name you wish to move: ";
    string region_name;
    cin >> region_name;

    // Decrement player counts for current region
    p->region->num_of_players--;

    // Switch to the new region and increment its player counts
    p->region = m->getRegion(region_name);
    p->region->num_of_players++;

    //cout << p->name << "'s current region is now " << p->region->name << endl;
    log->push_back(p->name + "'s current region is now " + p->region->name );
    phase->NotifyAll();
    sub->NotifyAll();
}

map<string, int> Player::dice_effect_count() {
    map<string, int> d = {{"Energy", 0}, {"Attack", 0},{"Destruction", 0},
                             {"Heal", 0}, {"Star", 0}, {"Ouch", 0}};

    for (auto i : dice){
        d[i.second.getState()]++;
    }

    return d;
}

Player::Player(string monster, Region *r) : name(monster), region(r) {

    // Initialize 6 black dices
    for (int i=1; i<7; dice[i++]=Dice());

    // Initialize 2 green dices
    for (int i=1; i<3; green_dice[i++]=Dice());
}

void Player::rollDice() {
    strategy->rollDice(this);
}

void Player::resolveDice(vector<Player> *players, Map *m) {
    strategy->resolveDice(this, players, m);
}

void Player::move(vector<Player> *players, Map *m) {
    strategy->move(this, players, m);
}


//TODO: Still a lot to fix. But just assume for now
void Player::buyCards(Deck<Card>* deck){
    strategy->buyCards(this, deck);
}

void Player::firstRoll(){

    // Rolling black dices
    for (auto &i : dice){
        i.second.roll();
    }

    // Rolling green dices
    for (auto &i : green_dice){
        i.second.roll();
    }
}

int Player::countAttacks() const{

    int total_attacks{};

    // Counting attacks in 6 black dices
    for (auto &i : dice){
        if (i.second.getState() == "Attack")
            total_attacks++;
    }

    // Counting attacks in 2 green dices
    for (auto &i : green_dice){
        if (i.second.getState() == "Attack")
            total_attacks++;
    }

    return total_attacks;
}

Strategy *Player::getStrategy() const {
    return strategy;
}
