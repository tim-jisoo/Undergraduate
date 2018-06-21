//허지수 3학년 B115233
#include <iostream>
#include "rectb.h"

Rectangle::Rectangle(int x = 0, int y = 0 , int h  = 0, int w = 0) // 생성자
: xLow(x), yLow(y), height(h), width(w) {} //이니셜라이저로 초기화

ostream& operator<<(ostream& os,const Rectangle& s)// 전역함수 기반의 연산자 오버로딩
{
	os  << "Position: " << s.xLow << " " << s.yLow << "; " << "Height = " << s.height << " " << "Width = " << s.width  << endl;
}

bool Rectangle::operator< (const Rectangle& s) const // rectangle 의 크기를 비교하는 함수
{
	if(height * width < s.height*s.width) return true;
	else return false;
}

bool Rectangle :: operator==(const Rectangle& s) const // rectangle 이 같은지 여부를 판단하는 함수
{
	if(this == &s) return true;
	if( ( xLow == s.xLow ) && ( yLow == s.yLow) && ( height == s.height ) && ( width == s.width) )  return true;
	else return false;
}

int Rectangle::GetHeight() const {return height;}
int Rectangle::GetWidth() const {return width;}

