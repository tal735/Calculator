#include "helper.h"
#include <iostream>	//cin/cout
#include <stdlib.h> //atof

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
	// expecting to get a number here. check for missing number at end of input
	if (end_of_string(pos, stmt)){
		*status=ERROR;
		return ERROR;
	}

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

// build tree of (arg1, opertaion, agr2) ... from user input
Node* build_tree(string stmt, int* status){
	unsigned int pos = 0;
	Node* cur;
	double num;
	char op;

	// skip initial whitespaces
	skip_whitespaces(stmt,&pos);

	// get first num
	num = get_num(stmt, &pos, status);
	if(*status==ERROR){
		cout << ERROR_READ_NUM << endl;
		return 0;
	}

	//handle case of a single digit input, for example: "1"
	if(end_of_string(&pos,stmt)){
		cur = create_num_child(num,0);
		return cur;
	}
	// get op
	op = get_op(stmt, &pos, status);
	if(*status==ERROR){
		cout << ERROR_READ_OP << endl;
		return 0;
	}

	// set first root of the tree
	cur = new Node();
	cur->parent=0;	//mark as root
	cur->op=op;
	cur->type=OP_NODE;
	cur->left=create_num_child(num,cur);


	do{
		// get next number
		num = get_num(stmt, &pos, status);
		if(*status==ERROR){
			cout << ERROR_READ_NUM << endl;
			break;
		}

		// deal with last number in input
		if (end_of_string(&pos, stmt)){
			// set node of number as right child of current node and quit
			cur->right=create_num_child(num,cur);
			// return root of tree to main
			while(cur->parent!=0){
				cur=cur->parent;
			}
			break;
		}

		// get next op
		op = get_op(stmt, &pos, status);
		if(*status==ERROR){
			cout << ERROR_READ_OP << endl;
			break;
		}

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

	}while(true);

	return cur;
}

// go over tree and calculate result
double calculate_result(Node* node, int* status){
	if(*status==ERROR) return 0;
	if(node->type==NUM_NODE) return node->value;
	Add add;Sub sub;Mul mul;Div div;
	double left=calculate_result(node->left, status);
	double right=calculate_result(node->right, status);
	switch(node->op){
		case '+':
			return add.Perform(left,right);
		case '-':
			return sub.Perform(left,right);
		case '*':
			return mul.Perform(left,right);
		case '/':
			if(right == 0){
				*status=ERROR;
				return 0;
			}
			return div.Perform(left,right);
	}
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
	// make sure to start from root node
	while(node->parent!=0){
		node = node->parent;
	}

	// free children and then node itself recursively
	free_recursive(node);
}
