//Name: Hanif Mirza
//Date: 8 may, 2015
//File Name: automata.h
//Class header file for Automata. It contains all data members and prototypes of all funtions

#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <utility>  // std::pair
#include <string>
#include <algorithm>
#include <cassert>
#include <tuple>

using namespace std;

class Automata
{
	private://data members
        vector<string>						allStates;
        vector<string> 						allSymbols;
        vector<string> 						allFinalStates;
        vector<string> 						allStackAlphabets;
        string								initialState;
        string								currentState;
        string								stackStartSymbol;
        map< tuple<string,string, string> ,vector< pair<string, string> > > 	allTransitionsMap;
        vector< tuple<string,string, string> > pathVector;



	public://Member functions prototype
        Automata();
        void 	addState(string state);
        void 	addSymbol(string symbol);
        void 	addFinalState(string finalState);
        void    addAllStackAlphabets(string stackSymbol);
        void 	setInitialState(string myInitialState);
        void	setStackStartSymbol(string stackSymbol);

        bool 	isFinalState(string state);
        void 	addTransition(string startState,string inputAlphabet,string startStack,string endState,string endStack);
        vector< tuple<string,string, string> > transitionToNextState(tuple<string,string, string> myTuple);
        bool    process(tuple<string,string, string> urTuple);
        void	displayNPDA();
        void    clearThePathVector();
        void    printPath();
};

#include "automata.cpp"
#endif
