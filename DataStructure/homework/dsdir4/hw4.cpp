//허지수 3학년 B115233
#include <iostream>
#include "post.h"
using namespace std;

void Postfix(Expression);

int main()
{
	char linebuf[MAXLEN];
	while( cin.getline(linebuf, MAXLEN) ) // cin객체를 통해서 한줄씩 입력을 받고 있다.
	{
		Expression e(linebuf, true);
		try
		{ Postfix(e); }
		catch(char const* msg)
		{ cerr << "Exception: " << msg << endl; }
	}
}
