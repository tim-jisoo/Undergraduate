//허지수 3학년 B115233
#include "polyb.h"

int main()
{
	Polynomial p1,p2,p3;
	
	cin >> p1 >> p2; // istream 클래스 객체와 Polynomial 클래스의 객체간의>> 연산자 오버로딩
	p3 = p1 * p2; // Polynomial 클래스객체 간의 *연산자 오버로딩
	cout << p1 << p2 << p3; // ostream 클래스 객체와 Polynomial 클래스 객체간의 << 연산자 오버로딩


}
