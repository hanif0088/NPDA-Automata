//Name: Hanif Mirza
//Date: 8 may, 2015
//File Name: automata.cpp
//Member functions defined for the Automata class.

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <utility>      // std::pair
#include <string>
#include <algorithm>
#include <cassert>
#include <tuple>
#include "automata.h"

using namespace std;

//Constructor
Automata::Automata()
{

}
//===========================================================================
/*This function will create  a map (key-value) of transitions. The "key" will be a 3-tuple (the current state,
a input symbol, and a stack top symbol) and the "value" will be a vector of pairs (each pair consist of the next
state and the sequence of stack symbols that will replace the stack top)*/

void Automata::addTransition(string startState,string inputAlphabet,string startStack,string endState,string endStack)
{
	string myInputAlphabet = inputAlphabet;
	if(myInputAlphabet =="*")
	{
		myInputAlphabet= "";
	}
	tuple<string,string, string> keyTuple(startState,myInputAlphabet,startStack);//creating 3-tuple

	string myEndStack = endStack;
	if(myEndStack == "*")
	{
		myEndStack = "";
	}
	pair <string,string> myPairValue(endState,myEndStack);

	allTransitionsMap[keyTuple].push_back(myPairValue);//adding the value to vector of pairs
}

//================================================================
/*This funtion  will make transition from current state to possible different states for
a given tuple(startState,inputString and stackString ). So, it will return a vector of
instantaneous descriptions (besically tuples) */

vector< tuple<string,string, string> > Automata::transitionToNextState(tuple<string,string, string> urTuple)
{
	vector< tuple<string,string, string> > vectorOfTuple;

	tuple<string,string, string> myTuple = urTuple;

	//getting all informations from the given tuple as in argument
	string startState = get<0>(myTuple);
	string inputString = get<1>(myTuple);
	string stackString = get<2>(myTuple);

	string newInputString="";
	char myCharSymbol='\0';
	if( inputString != "")
	{
		newInputString= inputString.substr(1);// this will be the new string that needs to be process next time
		myCharSymbol = inputString.at(0);// getting the start symbol from the inputString
	}
	string	inputSymbol(1,myCharSymbol);//converting the symbol from character to string

	char myStackChar='\0';
	if(stackString != "")
	{
		myStackChar = stackString.at(0);//getting the stack top from input stack String
	}
	string	stackTop(1,myStackChar);//converting the stack top from character to string

	tuple< string,string, string > myKeyTuple(startState,inputSymbol,stackTop);//creating a key tuple to the find the value from map

	vector< pair<string, string> > myValueVector = allTransitionsMap[myKeyTuple];//got the "value" which is the vector of pairs

	string newStackString="";
	string newState="";
	string myStack="";
	for(int i=0; i<myValueVector.size();i++)//accessing all the pairs from the vector
	{
		pair<string, string> myPairValue = myValueVector.at(i);
		newState = myPairValue.first;
		myStack = myPairValue.second;

		if(stackString != "")
		{
			newStackString = stackString.substr(1);//getting rest of the input stack String except the stack top
		}

		newStackString = myStack + newStackString;//concatenating rest of the input stack with the stack string that we got from pair

		tuple< string,string, string > myNewTuple (newState ,newInputString, newStackString );//new instantaneous description (tuple) that needs to be process next time

		vectorOfTuple.push_back(myNewTuple );//adding the new instantaneous description to the vector
	}

	//do the same thing as before with empty string because transition with empty string is possible in NPDA
	tuple< string,string, string > myKeyTuple2 (startState ,"", stackTop );

	if(allTransitionsMap.count(myKeyTuple2) == 1)//check is there is any value exists for the key tuple
	{
		string newStackString2="";
		string newState2="";
		string myStack2="";
		vector< pair<string, string> > myValueVector2 = allTransitionsMap[myKeyTuple2];
		for(int i=0; i<myValueVector2.size();i++)
		{
			pair<string, string> myPairValue2 = myValueVector2.at(i);
			newState2 = myPairValue2.first;
			myStack2 = myPairValue2.second;

			if(stackString != "")
			{
				newStackString2 = stackString.substr(1);
			}

			newStackString2 = myStack2 + newStackString2;

			tuple< string,string, string > myNewTuple2 (newState2 ,inputString, newStackString2 );
			vectorOfTuple.push_back(myNewTuple2 );
		}
	}
	return vectorOfTuple;
}
//=================================================================================
//This function will process an input string through the NPDA by using recursive backtracking.
//It will return true if the string is accepted and false otherwise
bool Automata::process(tuple<string,string, string> urTuple)
{
	tuple<string,string, string> myTuple = urTuple;
	pathVector.push_back(urTuple);//adding it to the path vector to display it later if it is accepted by NPDA

	string state = get<0>(myTuple);
	string inputString = get<1>(myTuple);
	if( isFinalState(state) && inputString=="" )//base case
	{
		return true;
	}

	//getting the vector of instantaneous descriptions from transitionToNextState() that needs to be processed
	vector< tuple<string,string, string> > vectorOfTuple = transitionToNextState(urTuple);

	for(int i=0; i<vectorOfTuple.size();i++)
	{
		if( process( vectorOfTuple.at(i) )== true )//recursively processing all instantaneous descriptions
		{
			return true;
		}
	}
	pathVector.pop_back();//if it rejected then remove it from the path vector
	return false;
}

//====================================================================
//displaying the DFA description
void Automata::displayNPDA()
{
	cout<<"----------N P D A----------"<<endl;
	cout<<"<states>"<<endl;
	for(int i=0;i<allStates.size();i++)
	{
		cout<<allStates[i]<<" ";//displaying all the States
	}
	cout<<endl;

	cout<<"<alphabet>"<<endl;
	for(int i=0;i<allSymbols.size();i++)
	{
		cout<<allSymbols[i]<<" ";//displaying allSymbols
	}
	cout<<endl;

	cout<<"<stack>"<<endl;
	for(int i=0;i<allStackAlphabets.size();i++)
	{
		cout<<allStackAlphabets[i]<<" ";//displaying allSymbols
	}
	cout<<endl;

	//displaying the transition funtions
	cout<<"<transition>"<<endl;
	map< tuple<string,string, string> ,vector< pair<string, string> > >::iterator it;//getting all iterators from map
	for (it = allTransitionsMap.begin(); it != allTransitionsMap.end(); ++it)
	{
		tuple<string,string, string> keyTuple = it->first;//getting the key from map
		vector< pair<string, string> > myValueVector = it->second ;//getting the value from map
		for(int i=0; i<myValueVector.size();i++)
		{
			pair<string, string> myPairValue = myValueVector.at(i);

			string startState = get<0>(keyTuple);
			string inputAlphabet = get<1>(keyTuple);
			string startStack = get<2>(keyTuple);
			string endState = myPairValue.first;
			string endStack = myPairValue.second;
			if(inputAlphabet =="")
			{
				inputAlphabet = "*";
			}
			if( endStack == "")
			{
				endStack = "*";
			}
			cout<<"("<<startState <<","<< inputAlphabet <<"," <<startStack<<")" << "->("<< endState<<","<< endStack <<")"<<endl;
		}
	}

	cout<<"<initial state>"<<endl;
	cout<<initialState<<endl;

	cout<<"<stack start>"<<endl;
	cout<<stackStartSymbol<<endl;

	cout<<"<final states>"<<endl;
	for(int i=0;i<allFinalStates.size();i++)
	{
		cout<<allFinalStates[i]<<" ";//displaying all the final States
	}
	cout<<endl;
	cout<<"----------------------------"<<endl;
	cout<<endl;
}
//============================================================================
//this function will add all states to the vector of allStates
void Automata::addState(string state)
{
	allStates.push_back(state);
}

//=====================================================================
//this function will add all symbols to the vector of allSymbols
void Automata::addSymbol(string symbol)
{
	allSymbols.push_back(symbol);
}

//===========================================================================
//this function will add all final states to the vector of allFinalStates
void Automata::addFinalState(string finalState)
{
	allFinalStates.push_back(finalState);
}

//======================================================================================
//this function will add the stack alphabets to the vector of allStackAlphabets
void Automata::addAllStackAlphabets(string stackSymbol)
{
	allStackAlphabets.push_back(stackSymbol);
}

//=========================================================================
//this function will return true if it's the final state and return false otherwise by traversing the vector of allFinalStates
bool Automata::isFinalState(string state)
{
	bool isFound=false;
	int i=0;
	while( i < allFinalStates.size() && !isFound )
	{
		if(allFinalStates[i]==state)
	    {
	    	isFound=true;
		}
	    i++;
    }
    return isFound;
}

//===========================================================================
//this function will set the initial state
void Automata::setInitialState(string myInitialState)
{
	initialState = myInitialState;
	currentState = myInitialState;
}

//===========================================================================
//this function will set the StackStartSymbol
void	Automata::setStackStartSymbol(string stackSymbol)
{
	stackStartSymbol = stackSymbol;
}

//============================================================================
//this function will clear the sequence of all the instantaneous descriptions (path vector)
void Automata::clearThePathVector()
{
	pathVector.clear();
}
//===========================================================================
//this function will print the sequence of all the instantaneous descriptions (path vector)
void Automata::printPath()
{
	//print the first tuple (instantaneous descriptions)
	cout<<endl;
	tuple<string,string, string> myTuple = pathVector.at(0);
	string startState = get<0>(myTuple);
	string inputString = get<1>(myTuple);
	string stackString = get<2>(myTuple);
	if(inputString == "")
	{
		inputString = "*";
	}
	if(stackString == "")
	{
		stackString = "*";
	}
	cout<< "("<<startState<<","<< inputString<< ","<< stackString<<")"<<endl;

	//print the rest of the tuple (instantaneous descriptions)
	for(int i=1; i<pathVector.size();i++)
	{
		tuple<string,string, string> myTuple = pathVector.at(i);
		string startState = get<0>(myTuple);
		string inputString = get<1>(myTuple);
		string stackString = get<2>(myTuple);
		if(inputString == "")
		{
			inputString = "*";
		}
		if(stackString == "")
		{
			stackString = "*";
		}

		cout<<"|- "<< "("<<startState<<","<< inputString<< ","<< stackString<<")"<<endl;
	}
}



