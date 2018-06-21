//허지수 3학년 B115233
#include <iostream>
#include "list.h"

int main()
{
	IntList i;

	i.Push_Back(5); i.Push_Back(7); i.Push_Back(9); cout << i;
	i.Push_Front(4); i.Push_Front(3); cout << i;
	i.Insert(6); i.Insert(10); i.Insert(2);
	i.Insert(5); i.Insert(2); i.Insert(10);
	cout << i;	
}
