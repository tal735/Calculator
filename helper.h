#ifndef HELPER_H
#define HELPER_H

#include "Definitions.h"
#include "Operation.h"
#include "Node.h"
#include <string>
using namespace std;

/**
 * The function returns true/false if the index *pos points to the end of the string.
 * Assuming:	1. 0 <= *pos <= stmt.length()
 *		2. variable stmt is initialized
**/
bool end_of_string(unsigned int* pos, string stmt);



/**
 * The function checks and returns if variable c represents a whitespace character.
**/
bool is_whitespace(char c);



/**
 * The function checks and returns if variable op is one of the allowed operations: +,-,*,/ and returns true/false accordingly.
**/
bool is_op(char op);



/**
 * The function gets a string variable 'stmt' and an index '*pos' and advances *pos to the first non-whitespace index in the string (including terminating byte).
 * Assuming:	1. 0 <= *pos < stmt.length()
 *		2. variable stmt is initialized
**/
void skip_whitespaces(string stmt, unsigned int* pos);



/**
 * The function gets a string variable 'stmt', an index '*pos' and a program flow status variable '*status'.
 * It tries to get the very next full sequence of characters that represents a number in the string, starting from given index,
 * converts the sequence into a double and returns the converted number.
 * If no number is found in the string, *status will indicate that an ERROR had occured.
 * Assuming:	1. 0 <=* pos <= stmt.length()
 *		2. variable stmt is initialized
 *		3. variable *status is initialized
**/
double get_num(string stmt, unsigned int* pos, int* status);



/**
 * The function gets a string variable 'stmt', an index '*pos' and a program flow status variable '*status'.
 * It expects to tackle only a math operation (+,-,*,/) , starting from given index.
 * If operation can not be found, *status will indicate that an ERROR had occured.
 * Assuming:	1. 0 <= *pos < stmt.length()
 *		2. variable stmt is initialized
**/
char get_op(string stmt, unsigned int* pos, int* status);



/**
 * The function creates a left-child node to 'parent'-node, and sets child's parent to be given 'parent'
 *
 * Assuming:	1. variable 'value' is initizlied
 *		2. variable 'parent' is initialized
**/
Node* create_num_child(double value, Node* parent);



/**
 * The function gets the term given as input bu user, and builds a corresponding tree to express the term. 
 * If a bad input is recognized (for example: 1+ , 2/a  etc..), *status will indicate an ERROR. 
 *
 * Example:
 *		Input: 1+4/2
 *		Tree:
 *				|---|
 *				| + |
 *				|---|
 *			       /     \
 *			      /	      \
 *			     /         \
 *			|---|		|---|
 *			| 1 |		| / |
 *			|---|		|---|
 *				       /     \
 *				      /       \
 *				     /	       \
 *				|---|	        |---|
 *				| 4 |           | 2 |
 *				|---|           |---|
 *
 * Assuming:	1. variable stmt is initialized
 *		2. variable *status is initialized
**/
Node* build_tree(string stmt, int* status);



/**
 * The function gets a the root of the expression-tree and recursively calculated the term given as input by user. 
 * If an error occurs (trying to divide by zero), *status will indicate an ERROR. 
 *
 * Assuming:	1. variable node is initialized and allocated and points to the root of the expression-tree.
 *		2. variable *status is initialized
**/
double calculate_result(Node* node, int* status);



/**
 * The function gets a tree node and recursively deletes its left and right children first, and then deleting itself.
**/
void free_recursive(Node* node);



/**
 * The function is called when wanting to delete the full expression-tree allocated by the program.
 * It can be called using any tree node as it will go up to the root node, and call free_recursive on root.
 * Assumption:	1. variable 'node' is initialized and allocated.
**/
void free_resources(Node* node);

#endif
