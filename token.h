#ifndef UNTITLED_TOKEN_H
#define UNTITLED_TOKEN_H

#include <string>
using namespace std;

class Token {
    string name;

public:
    Token(string name) : name(name) {}


    /**Test**/
    string getName(){return name;}
};


#endif //UNTITLED_TOKEN_H
