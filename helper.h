#include "Definitions.h"=
using namespace std;

bool end_of_string(unsigned int* pos, string stmt){
	if (*pos == stmt.length())
		return true;
	else
		return false;
}
bool is_whitespace(char c){
	if(c==' ')
		return true;
	else
		return false;
}
bool is_op(char op){
	if(op!='+' and op!='-' and op!='*' and op!='/')
		return false;
	else
		return true;
}
void skip_whitespaces(string stmt, unsigned int* pos){
	while(is_whitespace(stmt.at(*pos))){
		*pos = *pos + 1;
		if (end_of_string(pos, stmt))
			break;
	}
};

double get_num(string stmt, unsigned int* pos, int* status){
	unsigned int begin = *pos;
	unsigned int end;
	// find next number substring
	cout << "*pos: " << *pos << " , stmt.at(*pos): " << stmt.at(*pos) << endl;
	while(stmt.at(*pos)>='0' && stmt.at(*pos)<='9'){
		*pos = *pos + 1;
		if (end_of_string(pos, stmt))
			// if we get to end of string just break and convert last string to number
			break;
	}
	// edge case when we reach the end of our string
	if (end_of_string(pos, stmt) && stmt.length()>1)
		end = *pos-1;
	else
		end = *pos;

	double num = 0;
	if(*pos==begin){ // nothing to convert
		*status = ERROR;
		return 0;
	}
	else{
		// convert string to number
		string num_string = stmt.substr(begin,end);
		num = atof(num_string.c_str());
	}

	// skip white spaces
	if (!end_of_string(pos, stmt)){
		skip_whitespaces(stmt, pos);
	}

	return num;
};

char get_op(string stmt, unsigned int* pos, int* status){
	// if not end of string, we expect to find an operand
	char op = stmt.at(*pos);
	if(!is_op(op)){
		*status = ERROR;
		return 0;
	}
	*pos = *pos + 1;
	if(!end_of_string(pos, stmt))
		skip_whitespaces(stmt, pos);
	return op;
};
Node* create_num_child(double value, Node* parent){
	Node* child = new Node();
	child->value=value;
	child->type=NUM_NODE;
	child->parent=parent;
	return child;
}

void free_recursive(Node* node){
	if(node->type==NUM_NODE){
		delete node;
	}else{
		free_recursive(node->left);
		free_recursive(node->right);
		delete node;
	}

}

void free_resources(Node* node){
	if(node->parent!=0){
		node = node->parent;
	}
	free_recursive(node);
}
