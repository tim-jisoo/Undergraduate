//허지수 3학년 B115233
#include <iostream>
#include "recta.h"

int main (void)
{
	Rectangle r(2,3,6,6);
	Rectangle s(1,2,5,9);

	cout << "<rectangle r> "; r.Print();
	cout << "<rectangle s> "; s.Print();
	if (r.SmallerThan(s))
		cout << "s is bigger";
	else if (r.EqualTo(s))
		cout << "Same Size";
	else
		cout << "r is bigger";
	cout << endl;
}
