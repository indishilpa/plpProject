/*
	Created by Shilpa Goel 
	UFID - 49199201
*/

#include "astFunctions.h"
using namespace std;

string opList[16] = {"or", "gr", "ge", "ls", "le", "eq", "ne", ">", "<", ">=", "<=", "+", "-", "*", "/", "**"};
string UopList[2] = {"not", "neg"};

void printTree(Node *root, int level){
	if(!root){
		return;
	}

	int temp = level;
	cout << root->data << "\n";
	if(root->leftChild != NULL){
		while(temp >= 0){
			cout << ".";
			temp--;
		}
		printTree(root->leftChild, level + 1);
	}
	temp = level - 1;
	if(root->rightChild != NULL){
		while(temp >= 0){
			cout << ".";
			temp--;
		}
		printTree(root->rightChild, level);
	}
}

void deleteTree(Node *p) {    
    if (!p) return;
    deleteTree(p->leftChild);
    deleteTree(p->rightChild);
            
    delete p;
}

void startParsing(){
	NT = getNextToken();
	if(NT != 0){	
		parseE();
	}
}

int main (int argc, char *argv[])
{
	if(argc > 3){
		cout << "Format: cppFileName -ast RpalFileLocation\n";
	}else if (argc == 3){			
	   infile.open(argv[2]); 
	   startParsing();
	   infile.close();
	   string argument = argv[1];
	   
	   if(treeStack.size() != 0){
	   	Node *tree = treeStack.top();
	   	if(argument == "-ast"){
	    	printTree(tree, 0);
	   	}
	   	   
	   	deleteTree(tree);
	   	treeStack.pop();	 
	   }
	}else if(argc == 2){
		string argString = argv[1] ;
		if(argString != "-ast"){
			infile.open(argv[1]); 
	    	startParsing();
	    	infile.close();
	    
	    	vector<string> st, del;
			if(treeStack.size() != 0){
	   			Node *tree = treeStack.top();
	   			printTree(tree, 0);				// ast	  
	   			cout << "standardizing tree\n";
				tree = standardizeTree(tree);
				createStackDelta();
	   			printTree(tree, 0);				// standardized tree
	   			deleteTree(tree);
	   			treeStack.pop();	 
	   		}
		}		
	}else{
		cout << "wrong input params\n";
	}
	return 0;
}
