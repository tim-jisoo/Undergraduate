//허지수 3학년 B115233
#include <iostream>
#include "post.h"
using namespace std;

void Eval(Expression);

int main(void)
{
	char line[MAXLEN];
	while(cin.getline(line, MAXLEN) )
	{
		Expression e(line); // 후위포기식 문장이 입력되었다고 가정을하자. 
		try 
		{
			 Eval(e);
		}
		catch(char const *msg)
		{
			cerr << "Exception: " << msg << endl;
		}
	}
}
