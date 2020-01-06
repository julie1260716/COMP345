#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include <string>
#include <memory>
#include "region.h"
#include "dice.h"
#include "map.h"
#include "deck.h"
#include "card.h"
#include "Strategy.h"


using namespace std;

class Player {
private:
    string name;
    int victory_point = 0;
    int life_point = 10;
    int energy_cube = 0;
    Region* region;
    map<int, Dice> dice;
    map<int, Dice> green_dice;
    vector<Card> cards;
    Strategy* strategy;

public:
    Player(string monster, Region *r);

    /******************Turn moves methods********************/
    void rollDice();
    void resolveDice(vector<Player>*, Map*);
    void move(vector<Player>*, Map*);
    void buyCards(Deck<Card>*);
    /********************************************************/

    /**
     *
     * @return Map of dice effect name and its counts after rolling
     */
    map<string, int> dice_effect_count();

    string getName() { return name; }
    void take_damage(int d) { life_point -= d; }
    int getVP() {return victory_point;};
    int& getHP() { return life_point; }
    int& getEnergyCube() { return energy_cube; }
    Region* getRegion () { return region; }
    vector<Card>& getCard() { return cards; }
    void firstRoll();

    void setStrategy(Strategy *newStrategy){ strategy = newStrategy; }
    void addComputerName(string pc_level) { this->name += pc_level; }

    /**
     * Method used in first round for counting total Attack
     * in 6 black dices and 2 green dices
     * @return Total number of Attack
     */
    int countAttacks() const;

    /**
     * Used for determining the start order of players.
     * Used for comp function object in vector.
     * @param p Player
     * @return
     */
    bool operator<(const Player &p) const{
        return p.countAttacks() < this->countAttacks();
    }

    /**
     * Change player's region to a region that is not Manhattan and
     * which contains less than 2 players.
     * Defined in HumanPlayer.cpp
     */
    friend void change_region(Map*, Player*);

    map<int, Dice>& getDice() { return dice; }
    map<int, Dice>& getGreenDice() { return green_dice; }
    Strategy *getStrategy() const;

    /********************************Test****************************/
    void checkregionstate(){
        cout << region->name << ":" << region->num_of_players << endl;
    }
};


#endif //UNTITLED_PLAYER_H
