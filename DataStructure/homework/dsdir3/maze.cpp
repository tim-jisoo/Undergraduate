//허지수 3학년 B115233
#include <iostream>
#include <stack>
using namespace std;
const int MAXSIZE = 100; //up to 100 by 100 maze allowed
bool maze[MAXSIZE+2][MAXSIZE+2] = {0};
bool mark[MAXSIZE+1][MAXSIZE+1] = {0};

enum directions {N, NE, E, SE, S, SW, W, NW};
struct offsets
{
	int a, b;
} move[8] = { -1,0,	-1,1,	0,1,	1,1,	1,0,	1,-1,	0,-1,	-1,-1 };

struct Items
{
	int x, y, dir; 
	Items(int xx=0, int yy=0, int dd=0):x(xx),y(yy),dir(dd) {}
};

template <typename T>
ostream& operator<<(ostream& os, stack<T>& s)
{
	stack<T> temp_stack; // 임시스택인 temp_stack 선언 
	while(!s.empty())
	{
		temp_stack.push(s.top());  //임시스택에 s의 top을 push하고 있음.
		s.pop();	           //s의 top을 pop해야 그다음 s의 activation record를 꺼낼 수 있음.
	}
	
	while(!temp_stack.empty())
	{
		os << " -> " << temp_stack.top();
		temp_stack.pop();
	}
	return os;
}

ostream& operator<<(ostream& os, Items& item)
{
	static int count = 0; // 5개의 Items가 출력될 때마다 줄을 바꾸기 위해서
	os << "(" << item.x << "," << item.y << ")";
	count++; 
	if( (count%5) == 0 ) cout << endl;
	return os;
}

void Path(const int m, const int p)
{
	int cnt=0;

	mark[1][1] = 1;
	stack<Items> stack;
	Items temp(1,1,E);
	stack.push(temp);
	
	while(!stack.empty())
	{
		temp = stack.top();
		stack.pop();
 
		int i = temp.x; int j = temp.y; int d = temp.dir;
		while(d<8) //while(d<8)문 을 벗어나는 것은 미로가막혀서 다시 뒤로 돌아가는 길임
		{	
			int g = i + move[d].a; int h = j+ move[d].b; 
			if( (g==m) && (h==p) ) //출구 도착
			{
				cout << stack;
				temp.x = i; temp.y = j; cout << " -> " << temp;
				temp.x = m; temp.y = p; cout << " -> " << temp << endl;

				cout << "#nodes visited = " << cnt+2 << " out of " << m*p << endl; // 방문한 node수 출력
				return;
			}
			if( (!maze[g][h]) && (!mark[g][h]) ) //미로에 길이 있는데, 한번도 가본적이 없던 길인 경우
			{
				mark[g][h] = 1;
				temp.x = i; temp.y = j; temp.dir = d+1;
				stack.push(temp); ++cnt; //node를 방문할 때마다 cnt를 1 증가시킴
				i=g; j=h; d=N;
			}
			else d++;
		}
	}
	cout << "No path in maze." << endl;
}

void getdata(istream& is, int& m, int& p) //자료화일을 읽어드려 maze에 저장한다.
{
	is >> m >> p;	
	for(int i=0;i<=m+1;i++)   //미로의 경계(4면)을 막아둔다.
	{	
		maze[i][0] = 1;
		maze[i][p+1] = 1;
	}
	for(int j=1;j<=p; j++)
	{
		maze[0][j] = 1;
		maze[m+1][j] = 1;
	}
	for(int i=1; i<=m; i++)
		for(int j=1; j<=p; j++)
			is >> maze[i][j];
}
