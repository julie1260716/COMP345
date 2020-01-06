#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#include <string>
#include <vector>
#include "card.h"
#include "deck.h"
#include "tile.h"
#include "token.h"
#include "map.h"

#include <string>
using namespace std;

class Game {
    Deck<Card> *cards;
    Deck<Tile> *tiles;
    map<string, vector<Token>> tokens;
    Map* m;

    vector<Card> create_dCard();
    vector<Tile> create_dTile();
    void create_token();
    void assign_tile();
    Map* map_loader(const string& f_name);

public:
    Region* find_region(const string& name);
    string getMapName() { return m->getMapName(); }
    Map* getMap() { return m; }
    Deck<Card>* getDeck() { return cards; }
    Game(const string& filename);
    ~Game();

    /********************************Test****************************/
    void printcards(){
        cards->print();
    }

    void printtile(){
        tiles->print();
    }

    void printtoken(){
        for (map<string, vector<Token>>::iterator it = tokens.begin(); it != tokens.end(); it++){
            cout << "[" << it->first << "]";

            for (auto r : it->second){
                cout << "----" << r.getName();
            }
            cout << endl;
        }
    }

    void print_region_tiles() {
        Region *r = m->getMap().begin()->second;

        cout << "Region: " << r->name << endl;
        for (auto i : r->tile) {
            i.print();
        }
        cout << endl;

        for (auto i : r->adj){
            cout << "Region: " << i->name << endl;
            for (auto j : i->tile) {
                j.print();
            }
            cout << endl;
        }
    }

};


#endif //UNTITLED_GAME_H
