//허지수 3학년 B115233
#include <iostream>
#include <map>
#include <stack>
#include "post.h"

using namespace std;

ostream& operator<<(ostream&, Token);
Token NextToken(Expression&);
struct cmp_str
{
	bool operator() (char const * a, char const * b)
	{
		return strcmp(a,b) < 0 ;
	}
};

map<char*, float, cmp_str> ST;
map<char*, float, cmp_str>::iterator iter;

float GetVal(Token& opnd) // 1. 피연산자 처리
{
	if(opnd.type == NUM) //NUM인경우 val을 return
		return opnd.val; 
	else	//ID인 경우
	{
		if( ST.find(opnd.str) == ST.end() ) return 0;
		return ST[opnd.str];
	
	}
}

Token UnaryOp(int op, Token& x) // 2. 단항 연산자 처리
{
	if( !x.IsOperand() ) throw "Operand only for Unary Operator";
	// x가 피연산자여야만 밑으로 간다.
	Token temp; temp.type = NUM;
	if(op==UMINUS)
		temp.val = -GetVal(x);
	else if(op == '!')
		temp.val = !GetVal(x);
	else
		throw "Invalid unary operator";
	return temp;
}

Token BinaryOp(int op, Token& left, Token& right) // 3. 이항 연산자 처리
{
	if( !left.IsOperand() || !right.IsOperand() ) throw "Two Operands required for Binary Operation";
	
	Token temp; temp.type = NUM;
	float val1 = GetVal(left), val2 = GetVal(right);
	if( op == '+' ) temp.val = val1 + val2;
	else if( op == '-' ) temp.val = val1 - val2;
	else if( op == '*' ) temp.val = val1 * val2;
	else if( op == '/' ) temp.val = val1 / val2;
	else if( op == '<' ) temp.val = val1 < val2;
	else if( op == '>' ) temp.val = val1 > val2;
	else if( op == GE )  temp.val = val1 >= val2;
	else if( op == LE )  temp.val = val1 <= val2;
	else if( op == EQ )  temp.val = val1 == val2;
	else if( op == NE )  temp.val = val1 != val2;
	else if( op == AND ) temp.val = val1 && val2;
	else if( op == OR )  temp.val = val1 || val2;

	else if( op == '=' && left.type == ID )
	{
		iter = ST.find(left.str);
		if( iter != ST.end() ) //같은 변수네임이 존재하면 지운다.
			ST.erase(left.str);
		 ST[left.str] = val2;
		return left; ////////////////////////////////////////////
	}
	else		     throw "No such binary operator";
	return temp;
}

void Eval(Expression e)
{
	stack<Token> stack;
	Token opnd1, opnd2; //피연산자 Token 선언
	
	for( Token x = NextToken(e) ; x != '#' ; x = NextToken(e) )
	{
		if( x.IsOperand() ) // 피연산자는 stack에 넣는다.
			stack.push(x);
		else
		{
			int op = x.type; //Token x의 Operation type을 op변수에 저장
			if( op == UMINUS || op == '!') //단항 연산자의 경우
			{
				opnd1 = stack.top(); //피연산자 꺼내서
				stack.pop();		
				stack.push( UnaryOp(op, opnd1) ); // 연산결과를 다시 stack에 push
			}
			else //이항연산의 경우
			{
				opnd2 = stack.top(); // 항상 연산자기준으로 오른쪽에 있는 피연산자가 stack에 나중에 쌓임. 
				stack.pop();
	
				opnd1 = stack.top();
				stack.pop();

				stack.push( BinaryOp(op, opnd1, opnd2) );
			}
				
		}
	}
	cout << stack.top() << endl;
	stack.pop(); //계산 결과가 저장된 stack.top()을 출력하고 pop해버림
}
