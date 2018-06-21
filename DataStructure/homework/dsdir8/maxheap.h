//허지수 3학년 B115233
#include <iostream>
using namespace std;

template <class T>
void ChangeSize1D(T* &a, const int oldSize, const int newSize);

template <class T>
class MaxHeap
{
private:
	int heapSize;
	int capacity;
	T *heap;
	template <class T2> friend ostream& operator<<(ostream&, MaxHeap<T2>&);
public:
	MaxHeap(int);
	void Push(const T& e);
	void Pop();
	bool IsEmpty() { return heapSize == 0;}
	T Top(){ return heap[1];} // maxheap은 0번째 원소를 사용하지 않고 1번째 index부터 사용한다. 따라서, 가장 max값(priority value)은 1번 index에 위치한다.
};

template <class T>
void ChangeSize1D(T* &a, const int oldSize, const int newSize)
{
	if(newSize<0) throw "New Size must be >=0";
	T* temp = new T[newSize];
	int number = min(oldSize, newSize);
	copy(a, a+number, temp);
	delete[] a;
	a =  temp;
}

template <class T>
ostream& operator<<(ostream& os, MaxHeap<T>& H)
{
	os << "<Heap Contents> ";
	for(int i = 1; i <= H.heapSize;  i++)
		os << i << ":" << H.heap[i] << " ";
	os << endl;
}

template <class T>
MaxHeap<T>::MaxHeap(int theCapacity = 10) : heapSize(0)
{
	if(theCapacity < 1) throw "Must be +ve";
	capacity = theCapacity;
	heap = new T[capacity+1]; //0번째 index는 사용하지 않기 때문에 capacity + 1만큼의 공간을 동적할당하였다.
}

template <class T>
void MaxHeap<T>::Push(const T& e)
{
	if(heapSize == capacity)
	{
		ChangeSize1D(heap, capacity+1, 2*capacity +1);
		capacity *= 2;
	}
	int currentNode = ++heapSize; //push함수는 완전이진트리의 가장 마지막 노드부터 차근차근 비교해가므로
	while(currentNode!=1 && heap[currentNode/2]<e) //삽입하는 e값이 아버지보다 크면
	{
		heap[currentNode] = heap[currentNode/2]; //아버지노드를 끌어내리고
		currentNode /= 2;
	}
	heap[currentNode] = e;	
}

template <class T>
void MaxHeap<T>::Pop()
{
	if(IsEmpty()) throw "Heap is empty. Cannot delete.";
	heap[1]. ~T(); //priority queue는 가장 우선순위가 높은 원소가 먼저 삭제됨 (root)
	T lastE = heap[heapSize--];
	
	int currentNode = 1; //삭제는 밑으로 내려가면서 비교한다.
	int child = 2;
	while(child<=heapSize) //왼쪽아들이 heapsize보다 큰 경우는 왼족아들이 존재하지 않는 경우이다.
	{
		if(child<heapSize && heap[child] < heap[child+1]) child++;
		//오른쪽아들이 존재하는 경우, 그리고 오른쪽 아들이 큰경우 오른쪽아들을 child로 설정
		//오른쪽아들이 존재한다 하더라도, 왼쪽아들이 큰경우는 왼쪽아들을 child로 설정
		//만약 child == heapSize이면 오른쪽아들이 존재하지 않는 경우이다. (if문 실행안됨)
		if(lastE>=heap[child]) break;
		heap[currentNode] = heap[child];//아들이 더큰경우이므로 아들을 끌어올리고
		currentNode = child;//currentNode는 밑의 Lv로 내려간다.
		child *=2; //여기서 child는 항상 짝수(왼쪽아들) 로 설정된다.
	}
	heap[currentNode] = lastE;
}

