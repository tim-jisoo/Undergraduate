//허지수 3학년 B115233
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <iostream>
using namespace std;

struct Term
{
	float coef;
	int exp;
};

class Polynomial
{
private:
	Term * termArray;
	int capacity;
	int terms;
public:
	Polynomial();
	Polynomial operator+(Polynomial&);
	void NewTerm(const float, const int);
friend ostream& operator<<(ostream&, Polynomial&);
friend istream& operator>>(istream&, Polynomial&);

};

#endif
