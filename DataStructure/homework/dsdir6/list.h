//허지수 3학년 B115233
#include <iostream>
using namespace std;

struct Node	
{
	int data;
	Node *link;
	Node(int d = 0, Node* l = 0)		//Node객체 생성시 link값이 명시되지 않으면, 0을 가르키겠음. 
		: data(d), link(l) {}
};

class IntList
{
private:
	Node* first;
	Node* last;
public:

	IntList(){ last = first = 0; }
	void Push_Back(int);
	void Push_Front(int);
	void Insert(int);
friend ostream& operator<<(ostream&, IntList&);
};
ostream& operator<<(ostream&, IntList&);

