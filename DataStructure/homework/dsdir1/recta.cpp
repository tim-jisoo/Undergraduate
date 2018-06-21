//허지수 3학년 B115233

#include <iostream>
#include "recta.h"

Rectangle::Rectangle(int x = 0, int y = 0 , int h  = 0, int w = 0) // 생성자
: xLow(x), yLow(y), height(h), width(w) {}

void Rectangle::Print() const  // Rectangle의 정보를 출력하는 함수
{
	cout << "Position : " << xLow << " " << yLow << "; " <<"Height = " << height << " " << "Width = " << width << endl;
}

bool Rectangle::SmallerThan(const Rectangle& s) const // rectangle 크기를 비교하는 함수
{
	if(height * width < s.height*s.width) return true;
	else return false;
}

bool Rectangle :: EqualTo(const Rectangle& s) const // rectangle 이 같은지 여부를 판단하는 함수
{
	if(this == &s) return true;
	if( ( xLow == s.xLow ) && ( yLow == s.yLow) && ( height == s.height ) && ( width == s.width) ) return true;
	else return false;
}

int Rectangle::GetHeight() const {return height;}
int Rectangle::GetWidth()  const {return width;}

