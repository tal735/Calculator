#define OP_NODE 'o'
#define NUM_NODE 'n'

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
