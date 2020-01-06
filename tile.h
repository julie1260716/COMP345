#ifndef UNTITLED_TILE_H
#define UNTITLED_TILE_H

#include <string>
#include <iostream>
using namespace std;

class Tile {
    string name;
    int durability;
    pair<string, int> reward;
    bool flipped = false;

public:
    Tile(string name, int d, pair<string, int> r) :
        name(name), durability(d), reward(r) {}

    string getTileName() { return name; }
    void setTileName(const string& s) { name = s; }
    int getDurability() { return durability; }
    void setDurability(int d) { durability = d; }
    pair<string, int> getReward() { return reward; }
    bool isFlipped() { return flipped; }
    void flip() { flipped = !flipped; }


    /********************************Test****************************/
    void print(){
        cout << "Name:" << name << " ";
        cout << "Durability:" << durability << " ";
        cout << "Reward:" << reward.first << "-" << reward.second << endl;
    }
};


#endif //UNTITLED_TILE_H
