#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

template <typename T>
struct Node
{
	Node<T> * leftChild;
	T data;
	Node<T> * rightChild;
	int x;
	int LV;
	
	Node(T d, Node<T> * left = 0, Node<T> * right = 0)
		:data(d), leftChild(left), rightChild(right) {}
};

struct Max_mode
{ bool operator()(int a, int b) { return a<b; } };

struct Min_mode
{ bool operator()(int a, int b) { return a>b; } };

template<typename T>
class Tree
{
private:
	Node<T>* root;
	queue< Node<T>* > ptrQ;
	
	void Inorder(Node<T> *, int &);
public:
	priority_queue<int, vector<int>, Max_mode> *maxQ;
	priority_queue<int, vector<int>, Min_mode> *minQ;

	Tree() { root = 0; ptrQ.push(root); }
	int Insert(T, T, T);
	void Inorder() { int cnt = 1; Inorder(root,cnt); }
};

void getData(istream& is, Tree<int>& tree, int& depth);
void getWidth(Tree<int>& tree, int depth);

int main(int argc, char* argv[])
{
	int index = 0; Tree<int> tree[100];
	int NumOfData;
	
	ifstream is("input.txt"); if(!is) { cerr << "input.txt does not exist\n"; return 1;}		
	is >> NumOfData;
	while(index < NumOfData)
	{	
		int depth = 0;
		getData(is, tree[index],depth);
		getWidth(tree[index++],depth);			
	}	
	is.close();
}

void getData(istream& is, Tree<int>& tree, int& depth)
{
	int index = 1; int tmp_depth;
	int NumOfNode, value, leftC_val, rightC_val;
	is >> NumOfNode;
	while(index++ <= NumOfNode)
	{
		is >> value >> leftC_val >> rightC_val;
		tmp_depth = tree.Insert(value,leftC_val,rightC_val);
		if(depth < tmp_depth) depth = tmp_depth;
	} 	
}

template <typename T>
int Tree<T>::Insert(T value, T leftC_val, T rightC_val)
{
	Node<T> * ptr = ptrQ.front();
	if( ptr == 0 )
	{
		ptrQ.pop();
		root = new Node<T>(value); root -> LV = 1;
		if(leftC_val > 0) {	root -> leftChild = new Node<T>(leftC_val);	(root -> leftChild) -> LV = 2;	ptrQ.push(root -> leftChild);	}
		if(rightC_val >0) {	root -> rightChild = new Node<T>(rightC_val);	(root -> rightChild) -> LV = 2;	ptrQ.push(root -> rightChild);	}
		if(leftC_val>0 ||rightC_val>0) return 2;
		return 1;
	}
	else
	{
		ptrQ.pop();
		if(leftC_val > 0)  {	ptr -> leftChild = new Node<T>(leftC_val); 	( ptr-> leftChild) -> LV = (ptr -> LV) +1;	ptrQ.push(ptr -> leftChild);	}		
		if(rightC_val > 0) {	ptr -> rightChild = new Node<T>(rightC_val);	(ptr -> rightChild) -> LV = (ptr -> LV) +1;	ptrQ.push(ptr -> rightChild);	}
		if(leftC_val > 0 || rightC_val > 0 ) return (ptr->LV)+1;
		return ptr -> LV;
	}	
}

template <typename T>
void Tree<T>::Inorder(Node<T>* ptr, int& cnt)
{
	if(ptr)
	{
		Inorder(ptr->leftChild,cnt);
		ptr -> x = cnt++; maxQ[ptr -> LV].push(ptr -> x); minQ[ptr -> LV].push(ptr -> x);
		//cout << ptr-> data << "," << ptr -> x << "," << ptr -> LV << endl;
		Inorder(ptr -> rightChild, cnt);		
	}
}

void getWidth(Tree<int>& tree, int depth)
{
	int width[depth]; int level; int WIDTH = 0;
	tree.maxQ = new priority_queue<int, vector<int>, Max_mode>[depth+1];
	tree.minQ = new priority_queue<int, vector<int>, Min_mode>[depth+1];
	tree.Inorder();
	for(int LV = 1; LV <= depth; LV++)
	{
		width[LV] = tree.maxQ[LV].top() - tree.minQ[LV].top() + 1;
		if(WIDTH < width[LV]) { WIDTH = width[LV]; level = LV; }
		//cout << LV << " " << width[LV] << endl; 
	}
	cout << level <<" "<<  WIDTH << endl;	
}
