#ifndef standardizer_h   
#define standardizer_h  

#include "astFunctions.h"

Node* standardizeLet(Node* root);
Node* standardizeWhere(Node* root);
Node* standardizeFuncForm(Node* root);
Node* standardizeLambda(Node* root);
Node* standardizeAnd(Node* root);
Node* standardizeWithin(Node* root);
Node* standardizeAmp(Node* root);
Node* standardizeTree(Node *root);

#endif   
