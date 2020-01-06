#include <fstream>
#include "game.h"

Map* Game::map_loader(const string& f_name) {
    ifstream file(f_name);
    if (!file){
        cerr << "File does not exist: " << f_name << endl;
        exit(1);
    }
    else{
        string map_name, line;
        getline(file, map_name);
        Map *vmap = new Map(map_name);
        string edge[2];

        while (getline(file, line)){
            //Create vertices
            if (line[0] !=  '('){
                vmap->addRegion(line);
            }
            // Create edge
            else{
                size_t last{}, next{}, i{};
                while ((next = line.find_first_of("(,)", last)) != string::npos)
                {
                    string tmp = line.substr(last, next-last);
                    last = next + 1;
                    if (tmp.length() > 1){
                        edge[i] = tmp;
                        i++;
                    }
                }
                vmap->addEdge(edge[0], edge[1]);
            }
        }

        return vmap;
    }
}

vector<Card> Game::create_dCard() {
    vector<Card> cards {
        Card(4, "CHAMELEON", "KEEP", "You can discard as many stars as you wish. For each star discarded, you can change the face of one of the dice you haven't resolved."),
        Card(5, "TESLA CANON", "DISCARD", "Plus 2 stars. All monsters (including you) must discard one KEEP card."),
        Card(1, "SUPER JUMP", "DISCARD", "Jump to another region immediately."),
        Card(2, "MASSIVE", "KEEP", "MASSIVE description."),
        Card(3, "JACK'S WISH", "DISCARD", "JACK'S WISH description."),
        Card(2, "HOLY SWORD", "KEEP", "HOLY SWORD description."),
        Card(1, "AINZ SAMA", "KEEP", "AINZ SAMA description."),
        Card(6, "THE HORDE", "DISCARD", "Deals 2 damage to all monsters (excluding yourself)")};

    return cards;
}

vector<Tile> Game::create_dTile() {
    // We need 5 tiles for each category, star, energy and health

    // Star tiles
    vector<Tile> star_3(5, Tile("Building", 3, make_pair("star", 3)));
    vector<Tile> star_2(5, Tile("Building", 2, make_pair("star", 2)));
    vector<Tile> star_1(5, Tile("Building", 1, make_pair("star", 1)));

    // Energy tiles
    vector<Tile> energy_3(5, Tile("Building", 3, make_pair("energy", 3)));
    vector<Tile> energy_2(5, Tile("Building", 2, make_pair("energy", 2)));
    vector<Tile> energy_1(5, Tile("Building", 1, make_pair("energy", 1)));

    // Health tiles
    vector<Tile> health_3(5, Tile("Building", 3, make_pair("health", 3)));
    vector<Tile> health_2(5, Tile("Building", 2, make_pair("health", 2)));
    vector<Tile> health_1(5, Tile("Building", 1, make_pair("health", 1)));

    int size = star_1.size() + star_2.size() + star_3.size() +
                energy_1.size() + energy_2.size() + energy_3.size() +
                health_1.size() + health_2.size() + health_3.size();

    vector<Tile> tiles;
    tiles.reserve(size);
    tiles.insert(tiles.end(), star_1.begin(), star_1.end());
    tiles.insert(tiles.end(), star_2.begin(), star_2.end());
    tiles.insert(tiles.end(), star_3.begin(), star_3.end());
    tiles.insert(tiles.end(), energy_1.begin(), energy_1.end());
    tiles.insert(tiles.end(), energy_2.begin(), energy_2.end());
    tiles.insert(tiles.end(), energy_3.begin(), energy_3.end());
    tiles.insert(tiles.end(), health_1.begin(), health_1.end());
    tiles.insert(tiles.end(), health_2.begin(), health_2.end());
    tiles.insert(tiles.end(), health_3.begin(), health_3.end());

    return tiles;
}

void Game::create_token() {
    vector<Token> web(13, Token("Web"));
    vector<Token> jinx(13, Token("Jinx"));
    vector<Token> souvenir(5, Token("Souvenir"));
    vector<Token> carapace(15, Token("Carapace"));

    tokens["Web"] = web;
    tokens["Jinx"] = jinx;
    tokens["Souvenir"] = souvenir;
    tokens["Carapace"] = carapace;
}

void Game::assign_tile() {

    // Get the first region
    // We only need the first region since it contains a vector of adjacent regions
    Region *r = m->getMap().begin()->second;

    // Get the deck of tiles
    vector<Tile> *_tiles = tiles->getDeck();

    // Assign 9 random tiles to the first region
    r->tile.insert(r->tile.end(), _tiles->begin(), _tiles->begin()+9);

    // Assign rest of tiles to rest of regions (9 tiles per region)
    int counter = 9;
    for (auto *i : r->adj){
        i->tile.insert(i->tile.end(), _tiles->begin() + counter, _tiles->begin() + counter*2);
    }

    // Release Deck<Tile> *tiles, no longer needed
    delete tiles;
    tiles = nullptr;
}

Region* Game::find_region(const string& name){
    Region *r = m->getMap().begin()->second;

    if (r->name == name)
        return r;

    for (auto *i : r->adj){
        if (i->name == name)
            return i;
    }

    return nullptr;
}

Game::Game(const string &filename) {
    m = map_loader(filename);

    // Initialize cards and building tiles
    cards = new Deck<Card>(create_dCard());
    tiles = new Deck<Tile>(create_dTile());

    // Shuffle both cards and tiles
    cards->shuffle();
    tiles->shuffle();

    // Initialize tokens
    create_token();

    // Assign 9 tiles to each region
    assign_tile();

    m->printMap();
}

Game::~Game() {
    delete m; m = nullptr;
    delete cards; cards = nullptr;
}