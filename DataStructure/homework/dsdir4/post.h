//허지수 3학년 B115233
#ifndef POSTFIX_H
#define POSTFIX_H

// token types for non one-char tokens
#define	ID	257
#define NUM	258
#define EQ	259
#define NE	260
#define GE	261
#define LE	262
#define AND	263
#define OR	264
#define UMINUS	265

#define MAXLEN	80

#include <cstring>

struct Expression
{
	char* buf;
	int pos;
	int len;
	bool infix; // true for infix expression

	Expression(char* linebuf, bool inf=false)
	: buf(linebuf), pos(0), len(strlen(buf)), infix(inf) {}
};

struct Token
{
	int type; // ASCII code for 1-char op
	char* str;
	int len;
	float val;

	Token();
	Token(char);
	Token(char, char, int);
	Token(char*, int, int);
	bool IsOperand();

	bool operator==(char);	
	bool operator!=(char);
};	

#endif
