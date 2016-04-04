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

Node* standardizeLet(Node* root){
	Node* eqNode = root->leftChild;
	Node* x = eqNode->leftChild;
	Node* e = x->rightChild;
	Node* p = eqNode->rightChild;

	Node* n = new Node("gamma");
	n->leftChild = new Node("lambda");
	n->leftChild->rightChild = e;
	n->leftChild->leftChild = x;
	x->rightChild = p;
		
	return n;
}

Node* standardizeWhere(Node* root){
	Node* eqNode = root->leftChild->rightChild;
	Node* x = eqNode->leftChild;
	Node* e = x->rightChild;
	Node* p = root->leftChild;

	Node* n = new Node("gamma");
	n->leftChild = new Node("lambda");
	n->leftChild->rightChild = e;
	n->leftChild->leftChild = x;
	x->rightChild = p;
	p->rightChild = NULL;
		
	return n;
}

Node* standardizeOp(Node* root){
	Node* e1 = root->leftChild;
	Node* e2 = e1->rightChild;

	Node* n = new Node("gamma");
	n->leftChild = new Node("gamma");
	n->leftChild->rightChild = e2;
	n->leftChild->leftChild = root;
	root->leftChild = NULL;
	root->rightChild = e1;
	e1->rightChild = NULL;

	return n;
}

// standardizing Unary operator
Node* standardizeUop(Node* root){
	Node* e = root->leftChild;
	
	Node* n = new Node("gamma");
	n->leftChild = root;
	root->leftChild = NULL;
	root->rightChild = e;
	return n;
}

Node* standardizeFuncForm(Node* root){
	Node* p = root->leftChild;	
	Node* temp = p;
	Node* n = new Node("=");
	n->leftChild = p;		
	
	while(temp->rightChild != NULL){
		temp = temp->rightChild;			
		if(temp->rightChild == NULL){			
			break;
		}
		Node* l = new Node("lambda");
		l->leftChild = 	temp;
		p->rightChild = l;
		p = temp;		
	}	
	return n;
}

Node* standardizeLambda(Node* root){
	Node* p = root->leftChild;	
	Node* temp = p;
	
	while(temp->rightChild != NULL){
		temp = temp->rightChild;			
		if(temp->rightChild == NULL){			
			break;
		}
		Node* l = new Node("lambda");
		l->leftChild = 	temp;
		p->rightChild = l;
		p = temp;		
	}	
	return root;
}

Node* standardizeAnd(Node* root){
	Node* firstEq = root->leftChild;
	Node* output = new Node("=");
	Node* comma = new Node(",");
	Node* tau = new Node("tau");
	
	output->leftChild = comma;
	output->leftChild->rightChild = tau;	

	Node* t = firstEq;
	comma->leftChild = t->leftChild;
	tau->leftChild = t->leftChild->rightChild;
	Node* variables = comma->leftChild;
	Node* values = tau->leftChild;

	t = t->rightChild;

	while(t != NULL){
		variables->rightChild = t->leftChild;
		variables = variables->rightChild;

		values->rightChild = t->leftChild->rightChild;
		values = values->rightChild;
		t = t->rightChild;
	}

	variables->rightChild = NULL;
	values->rightChild = NULL;
	return output;
}

Node* standardizeWithin(Node* root){
	Node* gamma = new Node("gamma");
	Node* lambda = new Node("lambda");

	Node* firstEq = root->leftChild;
	Node* x1 = firstEq->leftChild;
	Node* e1 = x1->rightChild;

	Node* secondEq = firstEq->rightChild;
	Node* x2 = secondEq->leftChild;
	Node* e2 = x2->rightChild;

	Node* output = new Node("=");
	output->leftChild = x2;
	x2->rightChild = gamma;
	gamma->leftChild = lambda;
	lambda->rightChild = e1;
	lambda->leftChild = x1;
	x1->rightChild = e2;

	return output;
}

Node* standardizeAmp(Node* root){
	Node* e1 = root->leftChild;
	Node* n = e1->rightChild;
	Node* e2 = n->rightChild;

	Node* output = new Node("gamma");
	Node* gamma = new Node("gamma");
	output->leftChild = gamma;
	gamma->rightChild = e2;
	gamma->leftChild = n;
	n->rightChild = e1;
	e1->rightChild = NULL;

	return output;
}

Node* standardizeTree(Node *root){
	if(root != NULL){
		root->leftChild = standardizeTree(root->leftChild);
		root->rightChild = standardizeTree(root->rightChild);
		
		if(root->data == "let"){						
			Node* output = standardizeLet(root);
			output->rightChild = root->rightChild;
			return output;
		}else if(root->data == "where"){
			Node* output = standardizeWhere(root);
			output->rightChild = root->rightChild;
			return output; 
		}else if(root->data == "function_form"){			
			Node* output = standardizeFuncForm(root);
			output->rightChild = root->rightChild;
			return output;
		}else if(root->data == "lambda"){
			Node* output = standardizeLambda(root);
			output->rightChild = root->rightChild;
			return output;
		}else if(root->data == "and"){
			Node* output = standardizeAnd(root);
			output->rightChild = root->rightChild;
			return output;
		}else if(root->data == "within"){
			Node* output = standardizeWithin(root);
			output->rightChild = root->rightChild;
			return output;
		}else if(root->data == "@"){
			Node* output = standardizeAmp(root);
			output->rightChild = root->rightChild;
			return output;
		}
	}	
	return root;
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
	    
			if(treeStack.size() != 0){
	   			Node *tree = treeStack.top();
	   			printTree(tree, 0);				// ast	  
	   			cout << "standardizing tree\n";
				tree = standardizeTree(tree);
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
