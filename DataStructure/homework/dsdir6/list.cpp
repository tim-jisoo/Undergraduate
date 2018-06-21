//허지수 3학년 B115233
#include <iostream>
#include "list.h"
#include <cstdlib>
ostream& operator<<(ostream& os, IntList& i)
{
	Node* ptr = i.first;
	while(ptr!=0)
	{
		os << ptr -> data << " ";
		ptr = ptr -> link;
	}
	os << endl;
	return os;
}

void IntList::Push_Back(int e)
{
	if(!first) //공백 리스트인 경우
		first = last = new Node(e);
	else
	{
		last -> link = new Node(e); // 새로운 노드를 생성한 후, 마지막노드의 링크가 새로운 녿를 가르키도록 한다.
		last = last -> link;		//그리고 마지막노드를 추가된 노드로 재설정 한다
	}
}

void IntList::Push_Front(int e)
{
	if(!first) //공백 리스트인 경우
		first = last = new Node(e);
	else if(first -> data < e)	//첫번째 노드의 data값이 입력값보다 작을 경우 아래의 메시지를 띄우고 프로그램을 강제 종료시킨다.
	{
		cout << "Push the num, less than " << first -> data << endl;
		exit(0);
	}
	else if(first -> data == e) {} //첫번째 노드의 data값이 입력값과 같은 경우 무시한다.
	else
	{
		Node* temp = new Node(e);
		temp -> link = first;	//새로이 생성된 노드의 링크를 first로 가르키도록하고
		first = temp;		//첫번째 노드를 추가된 노드로 재설정한다.
	}
}
void IntList::Insert(int e)
{
	if(!first)				//***************************************  공백 리스트 인경우
		first = last = new Node(e);	//***************************************  밑의 라인들은 공배리스트가 아닌경우
	else if(first -> data > e)		//first 노드의 data가 e 보다 큰경우는 e가 list 값들중 가장 작은 값이라는 뜻이다.
	{					//즉, Push_Front(e)와 같은 기능을 한다.
		Node* temp =  new Node(e);
		temp -> link = first;
		first = temp;
	}
	else if(first -> data < e)		//first 노드의 data가 e보다 작은 경우는,,,,,,  e가 적어도 첫번째 노드 이후의 노드에 추가 되어야한다.
	{
		Node* previous = first;		//previous가 current를 쫓아가도록 하였다.
		Node * current = first -> link;
		while(current)
		{
			if(current -> data == e) break;
			else if((previous -> data < e) && (current -> data > e)) //e가 previous 노드와 current노드 사이의 값이면
			{							 // 그 사이에 노드를 삽입하고 while문을 탈출한다.
				Node * temp = new Node(e);
				temp -> link = previous -> link;
				previous -> link = temp;		
				break;
			}
			else							//위 else if문을 만족하지 못하면 previous와 current를 각각 다음노드로 이동시킨다.
			{
				previous = current;
				current = current -> link;
			}
			
			if(current == 0)					// previous가 last 노드를 가리키게 되면 e는 list에 있는 data들중에 가장 큰 data이다.
			{
				last -> link = new Node(e);
				last = last -> link;
			}//if
		}//while						
	}//else if
}//fucntion scope

