#ifndef UNTITLED_REGION_H
#define UNTITLED_REGION_H

#include <iostream>
#include <vector>
#include "tile.h"

using namespace std;

struct Region {

    vector<Region *> adj; // Adjacent regions
    vector<Tile> tile;
    string name;
    int num_of_players = 0;

    Region(string s) : name(s){}

    bool operator==(const string& s) const{
        return this->name == s;
    }
};


#endif //UNTITLED_REGION_H
