//허지수 3학년 B115233
#ifndef RECTANGLE_H
#define RECTANGLE_H

using namespace std;

class Rectangle
{
private:
	int xLow;
	int yLow;
	int height;
	int width;
public:
	Rectangle(int, int, int, int);
	void Print() const;
	bool SmallerThan(const Rectangle&) const;
	bool EqualTo(const Rectangle&) const;
	int GetHeight() const;
	int GetWidth() const;
	/* 위의 const 선언들은 코드의 안정성을 보장하기 위해 삽입 했습니다. */
};
#endif
