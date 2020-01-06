/*
 * CONCRETE OBSERVER: Game Statistic Observer
 * This is the class which needs to keep itself updated with the change.
 * This class just needs to implement the Observer and register itself with
 * the ConcreteSubject and it is all set to receive the updates
 */
#include "Subject.h"
#include "Observer.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "StatObserver.h"
#include "player.h"
//#include "main.cpp"
using namespace std;

map<int, string> reg = {{1, "BRONX"}, {2, "QUEENS"}, {3, "BROOKLYN"}, {4, "STATEN-ISLAND"}, {5, "MANHATTAN"}};


StatObserver::StatObserver() {};

StatObserver::StatObserver(StatSubject* s) {
    //attach the Observer to the Subject
    _subject = s;
    _subject -> Attach(this);
};

StatObserver::~StatObserver() {
    //when we want to destroy it, we will detach it
    //from the subject
    _subject -> Detach(this);
}

void StatObserver::Update() {
    display();
};

void StatObserver::display() {
   /* int currentPlayer = _subject -> getCurrentPlayer();
    int location = _subject -> getLocation();
    int vp = _subject -> getVP();
    vector<string,char> &chart = _subject -> getChart();
    int size = chart.size();
    cout<<"\n---------------------------------------------\n";
    cout<<"   Player "<< currentPlayer  <<": " << steps[currentStep] << " Step\n";
    cout<<"---------------------------------------------\n";
    for(int i = 0; i < size; i++)
        cout<< "     " << chart.at(i) << endl;*/

    //list of players in game
    vector<Player>* pList = _subject->getPlayers();
    string r;

    cout<<"\n----------------------------------------------------------------------------------------------\n";
    cout<<"     Game Statistics\n";
    cout<<"----------------------------------------------------------------------------------------------\n";
    for(int i = 0; i < pList->size(); i++)
    {
        cout<< "     Player: " << pList->at(i).getName() << endl;
        cout<< "        Location: " << pList->at(i).getRegion()->name << endl;
        cout<< "        Victory Points: " << pList->at(i).getVP() << endl;
    }

    string bronx ="       ******\n";
    string queens="       ******************\n";
    string brooklyn = "       *******************************\n";
    string staten = "       ****************************************************\n";
    string manhattan= "       *********************************************************************\n";
    cout<<"\n----------------------------------------------------------------------------------------------\n";
    cout<<"     Borough Graph\n";
    cout<<"----------------------------------------------------------------------------------------------\n";
    for(int i = 0; i < pList->size(); i++)
    {
        cout<<pList->at(i).getName() + ":\n";
        r = pList->at(i).getRegion()->name;
        if(r.compare(reg[1]) == 0)
            cout << bronx;
        if(r.compare(reg[2]) == 0)
            cout << queens;
        if(r.compare(reg[3]) == 0)
            cout << brooklyn;
        if(r.compare(reg[4]) == 0)
            cout << staten;
        if(r.compare(reg[5]) == 0)
            cout << manhattan;

    }
    cout<<          "       Bronx       Queens      Broolyn        Staten Island        Manhattan\n";
    cout<<"----------------------------------------------------------------------------------------------\n";



};
