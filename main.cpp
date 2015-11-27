
#include <iostream>	//cin/cout
#include <cstdlib> 	//malloc
#include <stdlib.h> /* atof */
#include "Node.h"
#include "Operation.h"
#include "helper.h"
#include "Definitions.h"
using namespace std;
Node* root;


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
	if(status==ERROR){
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
	if(status==ERROR){
		cout << ERROR_READ_OP << endl;
		return 0;
	}

	// set first node of tree
	cur = new Node();
	cur->parent=0;	//mark as root
	cur->op=op;
	cur->type=OP_NODE;
	cur->left=create_num_child(num,cur);


	while(true){
		// get num
		num = get_num(stmt, &pos, status);
		if(status==ERROR){
			cout << ERROR_READ_NUM << endl;
			return 0;
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

		// get op
		op = get_op(stmt, &pos, status);
		if(status==ERROR){
			cout << ERROR_READ_OP << endl;
			return 0;
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
	}

	return cur;
}

// go over tree and calculate result
double calculate_result(Node* node, int* status){
	if(*status==ERROR) return 0;
	if(node->type==NUM_NODE) return node->value;
	Add add;Sub sub;Mul mul;Div div;
	char op = node->op;
	double left=calculate_result(node->left, status);
	double right=calculate_result(node->right, status);
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

			if(right == 0){
				cout << "Div-by-0 error. Terminating..." << endl;
				return 0;
			}
			return div.Perform(left,right);
			break;
	}
}


int main() {
	int status = OK;
	string stmt;
	double result;

	// ask user what to calculate
	cout << "Calculate: " << endl;
	getline(cin,stmt);

	// build tree of user input
	root = build_tree(stmt, &status);
	if(status==ERROR){
		cout << "Your formula contains some errors. Terminating..." << endl;
		goto end;
	}

	// run across tree by order and calculate result
	result = calculate_result(root, &status);
	if(status==ERROR){
		cout << "Could not calculate result. Terminating..." << endl;
		goto end;
	}
	// print result
	cout << result << endl;

end:
	// free resources
	free_resources(root);
	// quit
	if(status==OK)
		return 0;
	else
		exit(1);
}
