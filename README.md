# Calculator

![alt tag](http://www.photo-host.org/images/2017/02/13/GUSqs.png)

C++ simple calculator task.

/**
 * The function gets the term given as input by a user, and builds a corresponding tree to express the term. 
 * If a bad input is recognized (for example: 1+ , 2/a, division by zero etc..), an error message is printed to the  user.
 *
 * Example:
 *		Input: 1+4/2
 *		Tree:
 *				      |---|
 *				      | + |
 *				      |---|
 *			       /     \
 *			      /	      \
 *			     /         \
 *			|---|		      |---|
 *			| 1 |		      | / |
 *			|---|		      |---|
 *				           /     \
 *				          /       \
 *				        /	         \
 *				    |---|	        |---|
 *				    | 4 |         | 2 |
 *				    |---|         |---|
 *
 *  Output: 3
 **/
