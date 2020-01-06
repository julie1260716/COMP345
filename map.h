#ifndef UNTITLED_MAP_H
#define UNTITLED_MAP_H

#include "region.h"
#include <map>

using namespace std;

typedef map<string, Region *> vmap;

class Map {
    vmap m;
    string name;

    bool checkValidMap();
public:
    Map() : name("KING OF NEW YORK"){}
    Map(string s) : name(s){}
    ~Map();
    void addRegion(const string&);
    void addEdge(const string& r1, const string& r2);
    void printMap();
    bool checkDuplicate(const string& r1, const string& r2);
    vmap& getMap() { return m; }
    string getMapName() { return name; }
    Region* getRegion(const string& name) { return m[name]; }
};


#endif //UNTITLED_MAP_H
