//허지수 3학년 B115233

#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

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
	bool operator<(const Rectangle&) const;
	bool operator==(const Rectangle&) const;
	int GetHeight() const;
	int GetWidth() const;

	friend ostream& operator<<(ostream&, const Rectangle&);
};

#endif
