//Name: Hanif Mirza
//Date: 8 may, 2015
//File Name: prog2.cpp

/*Program Description: This program will read the description of a NPDA from an input file and construct the NPDA.
It will determine using the NPDA functions whether this string is accepted or rejected by automata. The program
will also display the sequence of all the instantaneous descriptions.*/

#include <iostream>
#include <cstring>
#include <string>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <cassert>
#include <tuple>
#include "automata.h"

using namespace std;

void loadFile(ifstream &inFile,Automata &myAutomata,string &initialState,string &stackStartSymbol);

int main(int argc, char* argv[])
{
	ifstream 	inFile;
	Automata	myAutomata;
	string 		myString;
	string		myInitialState;
	string      myStackStartSymbol;

	inFile.open(argv[1]);//opening an input file
	if(inFile.fail())//opening a file should be confirmed
	{
		cout << "Automata file could not be opened!" << endl;
		return(1);
	}

	loadFile(inFile,myAutomata,myInitialState,myStackStartSymbol);//function call to load from inFile stream
	myAutomata.displayNPDA();//displaying the NPDA description

	while(true)
	{
		cout<<"Enter a string to process (Ctrl^C to end): ";
		getline(cin, myString);
		cout<<endl;
		myAutomata.clearThePathVector();//clear the sequence of all the instantaneous descriptions (path vector) before processing
		if (myString.empty())// if input is an empty string
		{

			tuple<string,string, string> myTuple(myInitialState,"",myStackStartSymbol );//create a tuple with empty string to check if the NPDA accepts it or not
			if(myAutomata.process(myTuple)== true )
			{
				myAutomata.printPath();//if it accepts then display all of the instantaneous descriptions
				cout<<"Accepted!"<<endl;
				cout<<endl;
			}
			else
			{
				cout<<"Rejected!"<<endl;
				cout<<endl;
			}
		}

		else// process if the string isn't empty
		{
			tuple<string,string, string> myTuple(myInitialState,myString,myStackStartSymbol );//create a tuple with the input string to check if the NPDA accepts it or not
			if(myAutomata.process(myTuple)== true )
			{
				myAutomata.printPath();//if it accepts then display all of the instantaneous descriptions
				cout<<"Accepted!"<<endl;
				cout<<endl;
			}
			else
			{
				cout<<"Rejected!"<<endl;
				cout<<endl;
			}
		}//end of else
	}

	inFile.close();
	return 0;
}//end of main

/*This function will load the inFile stream and populate the data memeber in Automata class with all the datas in the stream */
void loadFile(ifstream &inFile,Automata &myAutomata,string &initialState,string &stackStartSymbol)
{
	char	    title[50];
	string		states;
	string		alphabets;
	string      stackSymbol;

	string		startState;
	string		inputAlphabet;
	string		startStack;
	string		endState;
	string		endStack;

	string		myInitialState;
	string		myStackStartSymbol;
	string		finalState;

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>states;
		myAutomata.addState(states);//adding all states to the vector of states
		inFile>>ws;
	}
	while(inFile.peek()!='<');

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>alphabets;
		myAutomata.addSymbol(alphabets);//adding all symbols to the vector of symbols
		inFile>>ws;
	}
	while(inFile.peek()!='<');

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>stackSymbol;
		myAutomata.addAllStackAlphabets(stackSymbol);//adding all stack alphabets
		inFile>>ws;
	}
	while(inFile.peek()!='<');

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>startState;
		inFile>>inputAlphabet;
		inFile>>startStack;
		inFile>>endState;
		inFile>>endStack;
		myAutomata.addTransition(startState,inputAlphabet,startStack,endState,endStack);//creating all Transition functions
		inFile>>ws;
	}
	while(inFile.peek()!='<');

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>myInitialState;
		initialState = myInitialState;
		myAutomata.setInitialState(myInitialState);//set the initial State
		inFile>>ws;
	}
	while(inFile.peek()!='<');

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>myStackStartSymbol;
		stackStartSymbol = myStackStartSymbol;
		myAutomata.setStackStartSymbol(myStackStartSymbol);//set the stackSymbol
		inFile>>ws;
	}
	while(inFile.peek()!='<');

	inFile.getline(title,100,'\n');//skip the title at the begining
	do
	{
		inFile>>finalState;
		myAutomata.addFinalState(finalState); //adding all final states to the vector of final states
		inFile>>ws;
	}
	while(inFile.peek()!=EOF);

}