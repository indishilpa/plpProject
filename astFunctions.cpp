#include "astFunctions.h"

Node :: Node(string s){
  	this->leftChild = NULL;
	this->rightChild = NULL;
  	this->data = s;
}

int NT;
ifstream infile;
string keyword;
stack<Node*> treeStack;
int IDENTIFIER_TOKEN = 1000;
int INTEGER_TOKEN = 2000;
int STRING_TOKEN = 3000;
int KEYWORD_TOKEN = 4000;
string keyList[20] = {"let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec"};
char operatorList[27] = {'+' , '-' , '*' , '<' , '>' , '&' , '.', '@' , '/' , ':' , '=' , '~' , '|' , '$', '!' , '#' , '%' , 'ˆ' , '_' , '[' , ']', '{' , '}' , '"' , '‘' , '?'};
char stringList[9] = {'\t', '\n', '\\', '\'', '(', ')', ';', ',', ' '};

  		
void buildTree(string s, int num){	
	Node* n = new Node(s);
	Node* r = NULL;				
	while(num != 0){	
		if(treeStack.size() == 0){
			cout << "Parse error -- empty stack!\n";
			exit(1);
		}
		Node* curr = treeStack.top();	
		treeStack.pop();
		curr->rightChild = r;	
		r = curr;		
		num--;
	}
	n->leftChild = r;
	treeStack.push(n);	
}

int getNextToken(){
	while(!infile.eof()) {
		char c = infile.peek();			
		if (c == '/') {			
			infile.get(c);
			char c2 = infile.peek();			
			if(c2 == '/') { 	// found a comment.
				infile.get(c);							
				while(c != '\n' && !infile.eof()){
					infile.get(c);
				}
			}else{			
				keyword = "";
				keyword += c;
				return KEYWORD_TOKEN;
			}	
		}else if(checkIfLetter(c)){	 							
			keyword = "";
			while((checkIfLetter(c) || checkIfDigit(c) || c == '_' ) && !infile.eof()){
				infile.get(c);			
				keyword += c;			
				c = infile.peek();
			}
			if(infile.eof() && treeStack.size() > 1){
				cout << "Error... stack not empty but reached end of file.\n";
				exit(1);
			}
			string* p = find(keyList, keyList + 20, keyword);
			if(p != (keyList + 20)){		
				return KEYWORD_TOKEN;
			}else{
				return IDENTIFIER_TOKEN;
			}
		}else if(find(operatorList, operatorList + 26, c) != operatorList + 26){
			keyword = "";
			infile.get(c);			
			char c2 = infile.peek();
			keyword += c;
			if(find(operatorList, operatorList + 26, c2) != operatorList + 26){
				infile.get(c);
				keyword += c;						
			}
			return KEYWORD_TOKEN;
		}else if(checkIfDigit(c)){		
			keyword = "";					
			while(checkIfDigit(c) && !infile.eof()){
				infile.get(c);			
				keyword += c;			
				c = infile.peek();
			}
			if(infile.eof() && treeStack.size() > 1){
				cout << "Error... stack not empty but reached end of file.\n";
				exit(1);
			}
			return INTEGER_TOKEN;
		}else if(checkIfPunction(c)){
			infile.get(c);
			keyword = "";
			keyword += c;
			return KEYWORD_TOKEN;
		}else if(c == '\''){						
			infile.get(c);
			keyword = "";
			keyword += c;
			do{
				infile.get(c);
				keyword += c;
				if ((find(stringList, stringList + 9, c) == stringList + 9) && 
					(find(operatorList, operatorList + 26, c) == operatorList + 26) && !checkIfLetter(c) && !checkIfDigit(c)){
					return 0;
				}
			}while(c != '\'' && !infile.eof());
			if(infile.eof() && treeStack.size() > 1){
				cout << "Error... stack not empty but reached end of file.\n";
				exit(1);
			}
			return STRING_TOKEN;
		}else{
			infile.get(c);
		}		
	}
	return 0;
}


void read(string token){
	if(NT == 4000 && token != keyword){
		cout << "Error: Token mismatch expected : " << keyword << "\n";
		exit(1);
	}
	if (NT >= 1000 && NT < 4000){
		string s = "";
		if(NT == 1000){	
			if(token == "id"){
				s = "<ID:" + keyword + ">";					
			}else{
				cout << "Error: Token mismatch expected: " << keyword << "\n";
				exit(1);
			}			
		}else if(NT == 2000){			
			if(token == "int"){
				s = "<INT:" + keyword + ">";						
			}else{
				cout << "Error: Token mismatch expected: " << keyword << "\n";
				exit(1);
			}				
		}else if(NT == 3000){
			if(token == "str"){
				s = "<STR:" + keyword + ">";			
			}else{
				cout << "Error: Token mismatch expected: " << keyword << "\n";
				exit(1);
			}			
		}else{
			cout << ("Error!");
			exit(1);			
		}

		buildTree(s, 0);
	}
	NT = getNextToken();
}

bool checkIfLetter(char x){
	if((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z')){
		return true;
	}	 
	return false;
}

bool checkIfDigit(char x){
	if ( x >= '0' && x <= '9') {
		return true;
	}
	return false;
}

bool checkIfPunction(char x){
	 if ( x == '(' || x == ')' || x == ';' || x == ',') {
	 	return true;
	 }
	 return false;
}

void parseVl(){
	read("id");	
	if(NT == 4000 && keyword == ","){
		read(",");
		read("id");
		int n = 1;
		while(NT == 4000 && keyword == ","){
			read(",");
			read("id");
			n++;
		}		
		buildTree(",", n + 1);
	}
}

void parseVb(){
	if(NT == 1000){
		read("id");
	}else if(NT == 4000 && keyword == "("){			
		read("(");
		if(NT == 1000){		
			parseVl();			
			read(")");
		}else if(NT == 4000 && keyword == ")"){
			read(")");
			buildTree("()", 2);
		}		
	}
}

void parseDb(){
	if(NT == 1000){
		parseVl();
		if(NT == 4000 && keyword == "="){
			read("=");
			parseE();
			buildTree("=", 2);			
		}else if(NT == 1000 || (NT == 4000 && keyword == "(")){				
			parseVb();
			int n = 1;
			while(NT == 1000 || (NT == 4000 && keyword == "(")){
				parseVb();
				n++;
			}
			read("=");		
			parseE();			
			buildTree("function_form" , n+2);
		}
	}else if(NT == 4000 && keyword == "("){	
		read("(");		
		parseD();
		read(")");
	}
}

void parseDr(){
	if(NT == 4000 && keyword == "rec"){		
		read("rec");
		parseDb();
		buildTree("rec", 1);
	}else{		
		parseDb();
	}
}

void parseDa(){
	parseDr();
	if(NT == 4000 && keyword == "and"){
		read("and");
		parseDr();
		int n = 1;
		while(NT == 4000 && keyword == "and"){
			read("and");
			parseDr();
			n++;
		}
		buildTree("and", n+1);
	}
}

void parseD(){
	parseDa();
	if(NT == 4000  && keyword == "within"){
		read("within");
		parseD();
		buildTree("within", 2) ;
	}
}

void parseRn(){
	if(NT == 1000){				
		read("id");
	}else if(NT == 2000){		
		read("int");	
	}else if(NT == 3000){
		read("str");		
	}else if(NT == 4000){
		if(keyword == "true"){
			read("true");
			buildTree("<true>", 0);
		}else if(keyword == "false"){
			read("false");
			buildTree("<false>", 0);
		}else if(keyword == "nil"){
			read("nil");
			buildTree("<nil>" , 0);
		}else if(keyword == "("){					
			read("(");			
			parseE();
			read(")");								
		}else if(keyword == "dummy"){
			read("dummy");
			buildTree("<dummy>", 0);
		}
	}
}

void parseR(){
	parseRn();
	while((NT >= 1000 && NT <= 3000) || (NT == 4000 && (keyword == "true" || keyword == "false" || keyword == "nil" || keyword == "(" || keyword == "dummy"))){		
		parseRn();			
		buildTree("gamma", 2);
	}
}

void parseAp(){
	parseR();
	while(NT == 4000 && keyword == "@"){
		read("@");
		read("id");
		parseR();
		buildTree("@", 3);
	}
}

void parseAf(){
	parseAp();
	if(NT == 4000 && keyword == "**"){
		read("**");
		parseAf();
		buildTree("**", 2);
	}
}

void parseAt(){
	parseAf();
	while(NT == 4000 && (keyword == "*" || keyword == "/")){
		if(keyword == "*"){
			read("*");
			parseAf();
			buildTree("*", 2);
		}else if(keyword == "/"){		
			read("/");
			parseAf();
			buildTree("/", 2);
		}		
	}
}

void parseA(){
	if(NT == 4000 && (keyword == "+" || keyword == "-")){
		if(keyword == "+"){
			read("+");
			parseAt();
		}else if(keyword == "-"){
			read("-");
			parseAt();
			buildTree("neg", 1);
		}
	}else{	
		parseAt();
	}
	while(NT == 4000 && (keyword == "+" || keyword == "-")){
		if(keyword == "+"){
			read("+");
			parseAt();
			buildTree("+", 2);
		}else if(keyword == "-"){
			read("-");
			parseAt();
			buildTree("-", 2);
		}
	}
}

void parseBp(){	
	parseA();
	string keyOperators[10] = {"gr", ">", "ge", ">=", "ls", "<", "le", "<=", "eq", "ne"};
	if(NT == 4000 && find(keyOperators, keyOperators + 10, keyword) != keyOperators + 10){
		if(keyword == "gr" || keyword == ">"){
			read(keyword);
			parseA();
			buildTree("gr", 2);
		}else if(keyword == "ge" || keyword == ">="){
			read(keyword);
			parseA();
			buildTree("ge", 2);
		}else if(keyword == "ls" || keyword == "<"){
			read(keyword);
			parseA();
			buildTree("ls", 2);
		}else if(keyword == "le" || keyword == "<="){
			read(keyword);
			parseA();
			buildTree("le", 2);
		}else if(keyword == "eq"){		
			read(keyword);
			parseA();
			buildTree("eq", 2);
		}else if(keyword == "ne"){
			read(keyword);
			parseA();
			buildTree("ne", 2);
		}
	}
}

void parseBs(){
	if(NT == 4000 && keyword == "not"){
		read("not");
		parseBp();
		buildTree("not", 1);
	}else{
		parseBp();
	}
}

void parseBt(){		
	parseBs();
	while(NT == 4000 && keyword == "&"){
		read("&");
		parseBs();
		buildTree("&", 2);
	}
}

void parseB(){
	parseBt();
	while(NT == 4000 && keyword == "or"){
		read("or");
		parseBt();
		buildTree("or", 2);
	}
}

void parseTc(){
	parseB();
	if(NT == 4000 && keyword == "->"){
		read("->");
		parseTc();
		read("|");	
		parseTc();
		buildTree("->", 3);
	}
}

void parseTa(){
	parseTc();
	while(NT == 4000 && keyword == "aug"){
		read("aug");
		parseTc();
		buildTree("aug", 2);
	}
}

void parseT(){
	parseTa();
	if(NT == 4000 && keyword == ","){
		read(",");
		parseTa();		
		int n = 1;
		while(NT == 4000 && keyword == ","){
			read(",");
			parseTa();
			n++;
		}
		buildTree("tau", n + 1);
	}
}

void parseEw(){
	parseT();
	if(NT == 4000 && keyword == "where"){		
		read("where");
		parseDr();
		buildTree("where", 2);
	}	
}

void parseE(){	
	if(NT == 4000 && keyword == "let"){		
		read("let");
		parseD();		
		read("in");		
		parseE();
		buildTree("let", 2);			
	}else if(NT == 4000 && keyword == "fn"){
		read("fn");
		parseVb();
		int n= 1;
		while(NT == 1000 || (NT == 4000 && keyword == "(")){
			parseVb();
			n++;
		}
		read(".");
		parseE();
		buildTree("lambda", n+1);
	}else{		
		parseEw();
	}
}
