// 허지수 3학년 B115233
#include <iostream>
#include "rectb.h"

int main (void)
{
	Rectangle r(2,3,6,6);
	Rectangle s(1,2,5,9);

	cout << "<rectangle r> " << r << "<rectangle s> " << s;

	if (r<s)
		cout << "s is bigger";
	else if (r==s)
		cout << "Same Size";
	else
		cout << "r is bigger";
	cout << endl;
}
