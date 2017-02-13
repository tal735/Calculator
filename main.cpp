
#include <iostream>	//cin/cout
#include "helper.h"
#include "Definitions.h"
#include "Node.h"
#include <string>
using namespace std;

int main() {
	int status = OK;
	string stmt;
	double result;
	Node* root;

	// ask user what to calculate
	cout << "Calculate: ";
	getline(cin,stmt);

	// check no-input case
	if(stmt.length()==0){
		cout << MSG_NO_INPUT;
		return 0;
	}

	// build tree of user input
	root = build_tree(stmt, &status);
	if(status==ERROR){
		cout << ERROR_INCORRENT_INPUT << endl;
		goto end;
	}

	// run across tree by order and calculate result
	result = calculate_result(root, &status);
	if(status==ERROR){
		cout << ERROR_DIV_BY_ZERO  << endl; //ERROR_UNABLE_TO_CALC
		goto end;
	}
	// print result
	cout << result << endl;

end:
	// free resources
	free_resources(root);
	// quit
	return 0;
}
