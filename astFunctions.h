#ifndef elevator_h   
#define elevator_h  

#include <fstream>
#include <iostream>
#include <string>    
#include <stack>
#include <algorithm>
using namespace std; 

class Node{  
  	public:
		Node *leftChild; 
  		Node* rightChild; 
  		string data; 
  	
	  	Node(string s);
};

extern stack<Node*> treeStack;

int getNextToken();

bool checkIfPunction(char x);
bool checkIfDigit(char x);
bool checkIfLetter(char x);

extern char stringList[9];
extern char operatorList[27];
extern string keyList[20];
extern int KEYWORD_TOKEN;
extern int STRING_TOKEN;
extern int INTEGER_TOKEN;
extern int IDENTIFIER_TOKEN;
extern int NT;
extern ifstream infile;
extern string keyword;

void read(string token);

void parseE();
void parseEw();
void parseT();
void parseTa();
void parseTc();
void parseB();
void parseBt();
void parseBs();
void parseBp();
void parseA();
void parseAt();
void parseAf();
void parseAp();
void parseR();
void parseRn();
void parseD();
void parseDa();
void parseDr();
void parseDb();
void parseVb();
void parseVl();

#endif   
