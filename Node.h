#ifndef NODE_H
#define NODE_H

class Node{
public:
	Node* left;
	Node* parent;
	Node* right;
	char op;
	double value;
	char type; //'n' , 'o'
	Node(){};

};

#endif
