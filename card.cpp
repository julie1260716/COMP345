#include "card.h"

ostream& operator<<(ostream& os, const Card& card){
    os << "Name:" << card.name << "\tType:" << card.type << "\tEffect:" << card.effect << endl;
    return os;
}