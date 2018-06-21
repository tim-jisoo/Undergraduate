//허지수 3학년 B115233
#include "polya.h"

istream& operator>>(istream& is, Polynomial &p)
{
	int noofterms;
	float coef;
	int exp;

	is >> noofterms;

	for ( int i =0 ; i < noofterms ; i ++ )
	{
		is >> coef >> exp;
		p.NewTerm(coef, exp);	
	}

	return is;
}

ostream& operator<<(ostream& os, Polynomial &p)
{
	for( int i = 0 ; i < p.terms ; i ++ )
	{
		if( i == 0 && p.termArray[0].exp != 0)
		{
			if( p.termArray[0].coef == 1) os << "x^" << p.termArray[0].exp;
			else if( p.termArray[0].coef == -1) os << "-x^" << p.termArray[0].exp;
			else os << p.termArray[0].coef << "x^" << p.termArray[0].exp;
		}
		else if( i == 0 && p.termArray[0].exp == 0) os << p.termArray[0].coef;
		
		if( i > 0 && p.termArray[i].exp != 0)
		{
			if(p.termArray[i].coef > 0)
			{
				if( p.termArray[i].coef == 1) os << " +x^" << p.termArray[i].exp;
				else	os << " +" << p.termArray[i].coef << "x^" << p.termArray[i].exp;
			}
			else if(p.termArray[i].coef < 0)
			{
				if( p.termArray[i].coef == -1 ) os << " -x^" << p.termArray[i].exp;
				else	os << " " << p.termArray[i].coef << "x^" << p.termArray[i].exp;
			}
			else;	
		}
		else if( i > 0 && p.termArray[i].exp == 0)
		{	
			if(p.termArray[i].coef > 0) os << " +" << p.termArray[i].coef;
			else if(p.termArray[i].coef < 0) os << " " << p.termArray[i].coef;
			else;
		}
	}
	cout << endl;
	return os;
}

Polynomial :: Polynomial() : capacity(4), terms(0)
{
	termArray = new Term[capacity];
}

void Polynomial :: NewTerm(const float theCoeff, const int theExp)
{
	if(terms == capacity) 
	{	
		capacity *= 2;
		Term * temp = new Term[capacity];
		copy(termArray, termArray + terms, temp);
		delete [] termArray;
		termArray = temp;
	}
	termArray[terms].coef = theCoeff;
	termArray[terms].exp  = theExp;
	terms++;
}

Polynomial Polynomial :: operator+ (Polynomial& b)
{
	Polynomial c;

	int aPos = 0, bPos = 0;
	while( (aPos<terms) && (bPos<b.terms ) )
	{	
		if(termArray[aPos].exp == b.termArray[bPos].exp)
		{
			float t = termArray[aPos].coef + b.termArray[bPos].coef;
			if(t) c.NewTerm(t, termArray[aPos].exp);
			aPos++; bPos++;
		}
		
		else if(termArray[aPos].exp < b.termArray[bPos].exp)
		{
			c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
			bPos++;
		}
		
		else
		{
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
			aPos++;
		}
	}

	for(;aPos < terms ; aPos++)
		c.NewTerm(termArray[aPos].coef,termArray[aPos].exp);
	
	for(;bPos < b.terms ; bPos++)
		c.NewTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);

	return c;	
}
