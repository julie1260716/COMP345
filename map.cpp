#include "map.h"

Map::~Map(){
    cout << "Cleaning up...";

    for (vmap::iterator it=m.begin(); it != m.end(); it++){
        delete it->second;
    }
    cout << "\nDone";
}

void Map::addRegion(const string &name)
{
    vmap::iterator itr = m.find(name);
    if (itr == m.end())
    {
        Region *r = new Region(name);
        m[name] = r;
        return;
    }
    cout << name << " already exists in the map!\n";
}

void Map::addEdge(const string& r1, const string& r2)
{
    vmap::iterator itr1 = m.find(r1),
            itr2 = m.find(r2);
    if (itr1 == m.end() || itr2 == m.end()){
        cout << "\nNot able to connect ---- " << r1 << " <--> " << r2 << endl;
        cout << "At least one of the region is not in the map!\n\n";
        exit(1);
    }

    if (checkDuplicate(r1, r2)){
        cout << r1 << " and " << r2 << " are already connected!\n\n";
    }
    else{
        Region *f = (itr1->second);
        Region *t = (itr2->second);
        t->adj.push_back(f);
        f->adj.push_back(t);
    }
//    pair<int, Region*> edge = make_pair(cost, t);
}

bool Map::checkDuplicate(const string& r1, const string& r2){
    vector<Region*>::iterator it;

    for (auto r : m[r1]->adj){
        if (r->name == r2)
            return true;
    }

    return false;
}

void Map::printMap() {
    cout << "Name of map: " << name << endl;
    for (vmap::iterator it = m.begin(); it != m.end(); it++) {
        cout << "[" << it->first << "]";

        for (auto r : it->second->adj) {
            cout << "----" << r->name;
        }
        cout << endl;
    }
    cout << endl;

    if (checkValidMap()) {
        cout << "Map is connected.\n\n";
    } else {
        cout << "Some regions appear to be disconnected.\n";
        cout << "Map is disconnected\n\n";
        exit(1);
    }
}

bool Map::checkValidMap() {
    int total_regions = m.size();

    // If the connected regions of a particular region does not equal to total regions
    // we know that the map is not connected.
    return total_regions==m.begin()->second->adj.size()+1;
}