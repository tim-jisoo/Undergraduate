//허지수 3학년 B115233
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct cmp
{
	bool operator()(const int i, const int j)
	{
		return i > j;	//i<j 였다면 MaxHeap이었을 것이다.
	}
};

int main()
{
	priority_queue<int, vector<int>, cmp> minPQ;
	minPQ.push(15); minPQ.push(14); minPQ.push(21); minPQ.push(2); minPQ.push(10); minPQ.push(20);
	while(!minPQ.empty())
	{
		cout << minPQ.top() << " ";
		minPQ.pop();
	}
	cout << endl;
}
