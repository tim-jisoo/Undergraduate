//허지수 3학년 B115233
#include <iostream>
#include <stack>
#include "post.h"
using namespace std;

enum {UP_POINT,CUR_POINT}; // Expression e에서 NUM형 Token을 취할 때,,,,, e.buf[i]에 존재하는 '.' 즉, float형 NUM을 처리하기 위해서 존재
									// ex)  0.1234 에서 '.'은 e.buf[i]의 char형 원소로 존재할 것임
bool Token::operator==(char b) { return len==1 && str[0] == b; } //이 함수와 아래의 함수는 isp,icp 우선순위 함수에서 사용될 것임.
bool Token::operator!=(char b) { return len!=1 || str[0] != b; }

Token::Token() {}
Token::Token(char c) : len(1), type(c) // 1문자 연산자에 대한 Token 처리 
{
	str =  new char[1];
	str[0] = c;
}
Token::Token(char c1, char c2, int ty) : len(2), type(ty) // 2문자 연산자에 대한 Token처리
{
	str = new char[2];
	str[0] = c1; str[1] = c2;
}
Token::Token(char* arr, int l, int ty = ID) : len(l), type(ty) //연산자가 아닌 피연산자임!! (ID형 또는 NUM형)
{
	str = new char[len+1]; //NULL 감안해서 len+1만큼 동적할당
	strcpy(str, arr);
	
	int criteria = UP_POINT; // criteria는 소숫점 위, 아래를 구분하는 기준이다.
				// GetNUM함수로부터 expression으로부터 추출된 arr를 넘겨받을 때,  숫자뒤에 '.'이 있을 수 있다.

	stack<int> temp;	// 소숫점 뒷자리를 처리하기 위해서 stack이 필요하다고 판단. 이유는 아래 코드를 보자
	float val_temp=0;	// 소숫점 윗자리값에 영향을 미치지 않기위해서 소숫점 아래자리의 수를 따로 계산함.

	if(type == NUM)
	{
		val = str[0] - '0'; // 예를들어 str[0]이 '0'이었다면 '0'은 ASCII코드 상에서 10진수로 48이므로 48 - 48 = 0 이 된다. (자동형변환에 의해)
	
		for(int i = 1; i < len; i++) //1자리수 이상의 NUM 형을 처리
		{
			if( str[i] == '.' ) criteria = CUR_POINT; //'.'을 읽어드리는 순간 criteria는 현재 CUR_POINT 상태가 된다.
			
			if(criteria == UP_POINT) 	val = val * 10 + str[i] - '0';
			else if(criteria == CUR_POINT)  { criteria ++; } // str[i]가 '.(소숫점)'이면 for문을 한번 넘긴다. (동시에 criteria를 바꾼다. 또 이 구문에 들어오면 안되니까)
			else				temp.push( str[i]-'0' );
		}
		while( !temp.empty() )
		{
			val_temp = val_temp*0.1 + temp.top();
			temp.pop();
		}
			val_temp *= 0.1;
		val= val + val_temp; // 소숫점 윗자리수와 아랫자리수를 더함으로써 val을 완성시켰다.
	}
	else if(type == ID) val = 0;
	else throw "must be ID or NUM";
}
bool Token::IsOperand(){ return type == ID || type == NUM; } 

ostream& operator<<(ostream& os, Token t) // Postfix함수에서 쓰임(infix notation을 postfix notation으로 출력. 단, Expression의 값을 postfix notation으로 바꾸진 않음)
{
	if(t.type == UMINUS) os << "-u";
	else if(t.type == NUM) os << t.val;
	else for( int i = 0 ; i < t.len ; i++ ) os << t.str[i];
	os << " ";
	return os;
}

bool IsAlpha(char c) { return c>='a' && c<='z' || c>='A' && c<='Z'; } 
bool IsDigit(char c) { return c>='0' && c<='9'; }
/*NextToken함수의 1*/ bool GetID(Expression &e, Token& tok)
{
	char arr[MAXLEN]; 
	int idlen;

	char c = e.buf[e.pos];
	if( !IsAlpha(c) ) return false; //변수는 항상 알파벳으로 시작해야한다.
	arr[0] = c; idlen = 1;
	e.pos++;
	while( IsAlpha(c = e.buf[e.pos]) || IsDigit(c) ) //expression에존재하는 공백또는 연산자가 들어오면 while문을 탈출함. 즉, 변수복사가 끝나면 탈출
	{
		arr[idlen++] = c;
		e.pos++;
	}
	arr[idlen] = '\0'; // e.buf => arr => arr2: Token을 저장하기위해서 heap영역에 동적할당 해야하는데 필요한만큼의 메모리만 딱잘라서 필요하므로 
	
	char* arr2 = new char[idlen+1];
	strcpy(arr2,arr); // arr2 => Token으로 넘겨받은 이유: 아래줄의 경우 호출되는 Token생성자는 매개변수로 주소값을 넘겨받는데, 주의할점은
	tok=Token(arr2, idlen, ID); // expression 전체의 주소값을 넘겨받는게 아닌 Token으로 다루기위한 ID만을 잘라서 넘겨받아야하므로 따로 배열을 복사하는 것이다.
	return true;
}

/*NextToken함수의 2*/ bool GetNUM(Expression& e, Token& tok) 
{
	char arr[MAXLEN];
	int num_len;

	char c = e.buf[e.pos];
	if( !IsDigit(c) ) return false; // NUM은 무조건 숫자로 이루어져야함. 첫자부터 숫자가아니면 out
	arr[0] = c; num_len = 1;
	e.pos++;
	while( IsDigit( c=e.buf[e.pos] ) || c == '.') // ex) expression에 123 또는1.23 또는 123. 에 대해서는 복사함. 참고로 .123은 NextToken에서 error 띄워버림  
	{
		arr[num_len++] = c;
		e.pos++;
	}
	arr[num_len] = '\0';

	char* arr3 = new char[num_len+1];
	strcpy(arr3,arr);
	tok = Token(arr3, num_len, NUM);
	return true;
}

void SkipBlanks(Expression& e) //NextToken에서 쓰임
{
	char c; // 아래의 조건을 빠져나오기 위해서는 white blank가 아닌 문자가 들어와야 된다.
	while( e.pos < e.len            &&          ( (c=e.buf[e.pos] ) == ' ' || c == '\t') )
		e.pos++; //expression의 빈칸들은 건너뛴다. (무시한다.)
}

/*NextToken함수의 3*/ bool TwoCharOp(Expression& e, Token& tok)
{
	if(e.pos>=e.len-1) return false; //연산자가 두문자로 이루어져있는데, 두문자 연산자만으로 expression이 끝나게 될경우 false를 return한다.
	char c1 = e.buf[e.pos];
	char c2 = e.buf[e.pos+1];
	int op;

	if( c1 == '<' && c2 == '=') op = LE;
	else if( c1 == '>'  &&  c2 == '=' ) op = LE; 
	else if( c1 == '='  &&  c2 == '=' ) op = EQ;
	else if( c1 == '!'  &&  c2 == '=' ) op = NE;
	else if( c1 == '&'  &&  c2 == '&' ) op = AND;
	else if( c1 == '|'  &&  c2 == '|' ) op = OR;
	else if( c1 == '-'  &&  c2 == 'u' ) op = UMINUS;
	else return false;

	tok = Token(c1, c2, op);
	e.pos += 2;
	return true;
}

/*NextToken함수의 4*/ bool OneCharOp(Expression& e, Token& tok)
{
	char c = e.buf[e.pos];
	if( c=='-' || c=='!' || c=='*' || c=='/' || c=='%' || c=='+' || c=='<' || c=='>' || c=='(' || c==')' || c=='=' )
	{
		tok = Token(c);
		e.pos++;
		return true;
	}
	return false;
}

Token NextToken(Expression& e) 
{
	static bool opndFound = false; //중위표기식에서 expression의 e.buf[0]이 -일 경우는 무조건 unary minus임을 나타낸다.
	Token tok;
	SkipBlanks(e);
	//아래의 if구문들을 잘 이해해야한다. 조건이 만족해야 if 또는 else if가 실행될 것이다.
	//즉 조건판단을 위해서 1,2,3,4 함수들이 실행될 것이다.
	//이때, 각 함수마다 적당하지 않은 Token을 입력으로 받게되면 false를 return하여 다른 else if로 넘어가게 된다.
	if(e.pos == e.len) tok = Token('#'); //읽어들일 expression이 다끝나면 '#' Token을 반한하겠다.  
/*1,2*/	else if ( GetID(e,tok) || GetNUM(e,tok) ) { } //GetID와 GetNUM함수에서 tok을 초기화 시키고있다.
/*3*/	else if ( TwoCharOp(e,tok) ) {} //마찬가지이다.
/*4*/	else if ( OneCharOp(e,tok) )
	{
		if( e.infix && tok.type == '-' && !opndFound ) // 연산자 뒤의 -일 경우는 무조건 unary minus연산이다.
			tok = Token('-','u',UMINUS);
	}
	else throw "Illegal Char Found"; // 정의된 연산자 이외의 기호가 들어오면 에러를 출력하는데
					// 참고로 expression중 실수 (예를들어 0.123)를 읽어들일 수 있다. 이때 0.123 이아닌 .123으로 저장된경우 => 마찬가지로 error를 띄움. 즉 인정안함. 			
	if(e.infix) opndFound = tok.IsOperand(); // 피연산자 뒤의 -는 binary minus이다. 
	return tok;
}

int isp(Token&); //이 함수와 아래의 함수는 Postfix에서 쓰임
int icp(Token&);

void Postfix(Expression e)
{
	stack<Token> stack;
	stack.push(Token('#'));// #을 넣음으로써 프로그램이 간단해짐. 미로문제의 벽면에 1을 채우는것과 같은 이치
	for( Token x = NextToken(e) ; x != '#' ; x = NextToken(e) ) // Expression형 e를 NextToken함수의 인자로 전달하면서 return되는 값을 Token x에 저장함. 
	{										//즉, NextToken함수는 Token을만드는 함수일 것임.
		if(x.IsOperand()) cout << x; // 피연산자이면 바로 출력, 스택에 저장 안함.
		else if(x==')')
		{
			for( ; stack.top()!='(' ; stack.pop())
				cout << stack.top();
			stack.pop(); // '('연산자는 출력이 필요없으므로 바로 pop
		}
		else
		{
			for( ; isp(stack.top())<=icp(x) ; stack.pop() )
			{
				cout <<stack.top();
			}
			stack.push(x);
		}
	}

	while(stack.top()!='#') //Expression의 모든 수식들을 token으로 처리했고, stack에 남아있는 token들을 출력한다. '#'이 나올때까지 loop실행.
	{
		cout << stack.top();
		stack.pop();
	}
	cout << endl;
	stack.pop(); //마지막 '#'은 출력하지 않고 비운다.
}

int icp(Token& t)
{
	if( t.type == '(' ) return 0;
	else if( t.type == '!' || t.type == UMINUS ) return 1;
	else if( t.type == '*' || t.type == '/' || t.type == '%' ) return 2;
	else if( t.type == '+' || t.type == '-' ) return 3;
	else if( t.type == '<' || t.type == '>' || t.type == GE || t.type == LE ) return 4;
	else if( t.type == EQ  || t.type == NE ) return 5;
	else if( t.type == AND ) return 6;
	else if( t.type == OR ) return 7;
	else if (t.type == '=') return 8;
	else if (t.type == '#') return 10;	
}

int isp(Token& t)
{
	if( t.type == '(' ) return 9;
	else return icp(t);
}
