
#include <iostream>	//cin/cout
#include <cstdlib> 	//malloc
#include <string>	//stmt
#include <stdlib.h> /* atof */
#include "Node.h"
#include "Operation.h"
#define ERROR 0
#define OK 1
#define EOS 'E'

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
void skip_whitespaces(string stmt, unsigned int* pos, int* status){
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
		cout << "number was not found in input" << endl;
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
		skip_whitespaces(stmt, pos, status);
	}

	return num;
};
char get_op(string stmt, unsigned int* pos, int* status){
	// check for end of string first
	if(end_of_string(pos, stmt)){
		return EOS;
	}
	// if not end of string, we expect to find an operand
	char op = stmt.at(*pos);
	if(!is_op(op)){
		cout << "get_op error" << endl;
		*status = ERROR;
		return 0;
	}
	*pos = *pos + 1;
	if(!end_of_string(pos, stmt))
		skip_whitespaces(stmt, pos, status);
	return op;
};
Node* create_num_child(double value, Node* parent){
	Node* child = new Node();
	child->value=value;
	child->type=NUM_NODE;
	child->parent=parent;
	return child;
}
Node* build_tree_node(double num, char op){
	Node* op_node = new Node();
	op_node->op=op;
	op_node->type=OP_NODE;
	op_node->left=create_num_child(num,op_node);

	return op_node;
}

// build tree of (arg1, agr2, opertaion) ... from user input
Node* build_tree(string stmt, int* status){
	unsigned int pos = 0;
	Node* temp;
	double num;
	char op;

	// skip initial whitespaces
	skip_whitespaces(stmt,&pos,status);

	// get first num
	num = get_num(stmt, &pos, status);
	if(status==ERROR){
		cout << "Error getting NUM. Terminating.." << endl;
		return 0;
	}

	//handle case of a single digit input, for example: "1"
	if(end_of_string(&pos,stmt)){
		temp = create_num_child(num,0);
		return temp;
	}
	// get op
	op = get_op(stmt, &pos, status);
	if(status==ERROR){
		cout << "Error getting OP. Terminating.." << endl;
		return 0;
	}

	temp = build_tree_node(num,op);
	temp->parent=0;	//set temp as root

	Node* cur = temp;
	while(true){
		// get num
		num = get_num(stmt, &pos, status);
		if(status==ERROR){
			cout << "Error getting NUM. Terminating.." << endl;
			return 0;
		}
		//cout << "num1: " << num1 << endl;

		// deal with last num
		if (end_of_string(&pos, stmt)){
			// set node of num as right child of current node and quit
			cur->right=create_num_child(num,cur);
			// go back to root
			while(cur->parent!=0){
				cur=cur->parent;
			}
			break;
		}

		// get op
		op = get_op(stmt, &pos, status);
		if(status==ERROR){
			cout << "Error getting OP. Terminating.." << endl;
			return 0;
		}
		//cout << "op: " << op << endl;

		// create temporary node to insert to tree
		Node* next = new Node();
		next->op = op;
		next->type=OP_NODE;

		bool cur_op_add_or_sub = cur->op=='-' || cur->op=='+';
		bool cur_op_mul_or_div = cur->op=='*' || cur->op=='/';
		bool new_op_add_or_sub = op=='-' || op=='+';
		bool new_op_mul_or_div = op=='*' || op=='/';

		if((cur_op_add_or_sub && new_op_add_or_sub) || (cur_op_mul_or_div && new_op_mul_or_div)){
			cur->right=create_num_child(num, cur);							// adjust cur right son
			next->left=cur;													// adjust left child of new node
			if(cur->parent==0){												// adjust parents
				next->parent=0;
			}else{
				cur->parent->right=next;
				next->parent=cur->parent;
			}
			cur->parent=next;
		}
		else if(cur_op_mul_or_div && new_op_add_or_sub){
			cur->right=create_num_child(num, cur); // adjust cur right son
			Node* upper = cur;
			// find first parent who is '+' or '-'
			while(upper->op=='*' || upper->op=='/'){
				if(upper->parent==0)
					break;
				upper=upper->parent;
			}
			next->left=upper;			//child of new operation is current operation
			next->parent=upper->parent;	//set parents
			upper->parent=next;
		}
		else if(cur_op_add_or_sub && new_op_mul_or_div){
			cur->right=next;							//set right child of cur
			next->left=create_num_child(num, next);		//set left child of next
			next->parent=cur;							//set parent of next
		}
		cur = next;
	}

	return cur;
}

// go over tree and calculate result
double calculate_result(Node* node, int* status){
	if(node->type==NUM_NODE) return node->value; //cout << "returning: " << node->value << endl;
	Add add;Sub sub;Mul mul;Div div;
	char op = node->op;
	double left=calculate_result(node->left,status);
	double right=calculate_result(node->right,status);
	switch(op){
		case '+':
			return add.Perform(left,right);
			break;
		case '-':
			return sub.Perform(left,right);
			break;
		case '*':
			return mul.Perform(left,right);
			break;
		case '/':
			return div.Perform(left,right);
			break;
	}
}


int main() {
	string stmt;
	int status = OK;
	// ask user what to calculate
	cout << "Calculate: " << endl;
	getline(cin,stmt);

	// build tree of user input
	Node* root = build_tree(stmt, &status);
	if(status==ERROR){
		cout << "Your formula contains some errors. Terminating..." << endl;
		return 0;
	}

	// run across tree by some-order and calculate result
	double result = calculate_result(root, &status);
	if(status==ERROR){
		cout << "Could not calculate result (maybe you tried to divide by 0?). Terminating..." << endl;
		return 0;
	}

	// print result
	cout << "Result: " << result << endl;

	return 0;
}
