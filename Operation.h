#ifndef OPERATION_H
#define OPERATION_H

class Operation{
public:
	virtual double Perform(double A, double B) = 0;
};


 class Add : public Operation {
 public:
	double Perform(double A, double B)  {return A+B;}
 };
 class Sub : public Operation {
 public:
	double Perform(double A, double B)  {return A-B;}
 };
 class Mul : public Operation {
 public:
	double Perform(double A, double B)  {return A*B;}
 };
 class Div : public Operation {
 public:
	double Perform(double A, double B)  {return A/B;}
 };

#endif
