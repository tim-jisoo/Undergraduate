//허지수 3학년 B115233
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <stack>
using namespace std;

template <class K, class E>
struct Node
{
	Node<K, E> *leftChild;
	K key;
	E element;
	Node<K, E> *rightChild;
	
	Node(K ky, E el, Node<K,E> *left = 0, Node<K,E> *right = 0)
		: key(ky), element(el),	leftChild(left), rightChild(right) {}
};

template <class K, class E>
class BST
{
private:
	Node<K, E> *root;

	//helper 함수들
	void Visit(Node<K,E> *);
	void Insert(Node<K,E>* &, K &, E &);
	void Inorder(Node<K,E> *);
	void Postorder(Node<K,E> *);
	void Delete(Node<K,E>* &, K &);
public:
	BST() {root = 0;}
	void Insert(K &newkey, E &el) { Insert(root, newkey, el); }
	void Inorder() { Inorder(root); }
	void Postorder() { Postorder(root); }
	bool Get(const K&, E&);
	bool Print();
	bool RankGet(int r, K &k, E &e);
	void Delete(K &oldkey) { Delete(root, oldkey); }
	void ThreeWayJoin(BST<K,E>& small, K midkey, E midel, BST<K,E>& big);
	void TwoWayJoin(BST<K,E>& small, BST<K,E>& big);
};

template <class K, class E>
void BST<K,E>::Visit(Node<K,E> *ptr)
{
	cout << ptr -> key << ":" << ptr -> element << " "; 
}

template <class K, class E>
void BST<K,E>::Insert(Node<K,E>* &ptr, K &newkey, E &el)
{
	if(ptr == 0)// root가 존재하지 않거나, leaf node인 경우 실행
		ptr = new Node<K,E>(newkey,el);
	else if(newkey < ptr->key)
		Insert(ptr->leftChild, newkey, el);
	else if(newkey > ptr->key)
		Insert(ptr->rightChild, newkey, el);
	else
		ptr -> element = el; //UPDATE element
}

template <class K, class E>
void BST<K,E>::Inorder(Node<K,E> *currentNode)
{
	if(currentNode)
	{
		Inorder(currentNode -> leftChild);
		Visit(currentNode);
		Inorder(currentNode -> rightChild);
	}
}

template <class K, class E>
void BST<K,E>::Postorder(Node<K,E> *currentNode)
{
	if(currentNode)
	{
		Postorder(currentNode->leftChild);
		Postorder(currentNode->rightChild);
		Visit(currentNode);
	}
}

template <class K, class E>
bool BST<K,E>::Get(const K& k, E& e)
{
	Node<K,E> *ptr = root;
	while(ptr)
	{
		if(k < ptr->key) ptr = ptr->leftChild;
		else if(k > ptr->key) ptr = ptr->rightChild;
		else
		{
			e = ptr->element;
			return true;
		}
	}
	return false;
}

template <class K, class E>
bool BST<K,E>::RankGet(int r, K &k, E &e) //r은 복사, k와 e는 참조
{//이원탐색트리의 순위는 중위 순서에서의 순위이다. 따라서 트리의Inorder순회를 이용해야한다.
	int ranking = 0;
	stack< Node<K,E>* > s; //단, Inorder()함수를 이용해서는 우리가 원하는 결과를 얻을 수 없으므로, stack을 이용하여 non-recursive하게 Inorder 순회를 구현한다. 
	Node<K,E>* currentNode = root;
	while(1)
	{
		while(currentNode)
		{
			s.push(currentNode);
			currentNode = currentNode -> leftChild;
		}
		if(s.empty()) return false;
		currentNode = s.top();
		s.pop(); ranking ++; 
		//Inorder순회의 visit대신 rank를 센다.
		if(ranking == r)
		{
			k = currentNode -> key;      //k값에 대입을 함으로써 k가참조하는 메모리공간의 key값을 바꾼다.
			e = currentNode -> element;  //e값에 대입을 함으로써 e가참조하는 메모리공간의 element값을 바꾼다.
			return true;
		}
		currentNode = currentNode -> rightChild;
	}
}

template <class K, class E>
void BST<K,E>::Delete(Node<K,E>* &ptr, K &oldkey) //**********************************
{//ptr을 루트로 하는 트리에서 oldkey를 키로하는 노드를 지우시오.
	Node<K,E>* temp_ptr;
	if(ptr == 0) return; //oldkey를 찾을수 없으면 아무것도 하지 않고 return한다.
	
/*1*/	if(oldkey < ptr -> key) Delete(ptr -> leftChild, oldkey);			//루트를 기준으로 왼쪽서브트리에 oldkey가 존재하는 경우
/*2*/	else if(oldkey > ptr -> key) Delete(ptr -> rightChild, oldkey);			//루트를 기준으로 오른쪽서브트리에 oldkey가 존재하는 경우
/*3*/	else 						//oldkey를 찾은경우
	{
/*자식없음*/	if( !(ptr->leftChild) && !(ptr->rightChild) ) //oldkey를 키로하는 노드의 자식들이 없을때는 그냥 지우면됨
		{
			delete ptr;//ptr은 oldkey를 키로하는 노드를 가리키는 포인터이다.
			ptr = 0; //**************************를 보면 전달받는 인자가 Node<K,E>* &ptr로참조함을 알 수 있다.
			return;  //Delete함수는 recursive 하므로 Delete(ptr->leftChild, oldkey) 또는 Delete(ptr->rightChild, oldkey)의 첫번째 인자가 0이 된다.
		}		//즉, ptr = 0으로 한다는 것은 삭제되는 노드가 leaf node이므로 아버지노드가 자식을 가리키는 노드를 0으로 만드는 것이다.
/*왼쪽아들만*/	else if( (ptr->leftChild) && !(ptr->rightChild) )
		{				
			temp_ptr = ptr;			//ptr은 oldkey를 키로하는 노드라는걸 잊지 말아야한다.
			ptr = ptr -> leftChild;		//삭제할 노드의 왼쪽자식을 삭제할 노드의 자리로 붙인다.
			delete temp_ptr;		//삭제할 노드를 반환
			return;
		}
/*오른쪽아들만*/else if( !(ptr->leftChild) && (ptr->rightChild) )
		{
			temp_ptr = ptr;
			ptr = ptr -> rightChild;
			delete temp_ptr;
			return;	
		}
/*두아들있음*/	else	//지울 노드의 오른쪽 subtree중 가장작은 노드를 지울 노드의 자리에 둔다.
		{
			Node<K,E> *temp_daddy_ptr;
			Node<K,E> *currentNode = ptr -> rightChild;
			Node<K,E> *Before_currentNode = 0;
					
	/*case1*/	if(!currentNode->leftChild) //오른쪽 subtree의 왼쪽자식이 없으면 그 subtree의 루트가 가장 작은 노드임.
			{
				temp_ptr = ptr;
				ptr -> key = currentNode -> key;
				ptr -> element = currentNode -> element;
				ptr -> rightChild = currentNode -> rightChild;
				return;				
			}
	/*case2*/	else
			{
				while(currentNode)
				{
					temp_daddy_ptr = Before_currentNode;
					Before_currentNode = currentNode;		//while loop를 벗어나면 Befor_currentNode가 가장 작은 노드가 될것임.
					currentNode = currentNode -> leftChild;
				}	
				// 이 밑으로는 가장작은 노드를 찾은 상태
				temp_ptr = ptr;
				ptr -> key = Before_currentNode -> key; //주의 할것은 ptr = Before_currentNode 로 하면안됨.
				ptr -> element = Before_currentNode -> element; //만약 그렇게하면 oldkey를 갖는 ptr(지우기전)의 rightChild를 Before_currentNode의 rightChild로 덮어 씌워버림
				temp_daddy_ptr -> leftChild = Before_currentNode -> rightChild;
				return;
			}//delete함수 :: oldkey를 찾은 경우 :: 두아들이 있는 경우:: case2
		}//delete함수 :: oldkey를 찾은경우 :: 두아들이 있는 경우
	}//delete함수 :: oldkey를 찾은 경우
}//delete함수

template <class K, class E>
void BST<K,E>::ThreeWayJoin(BST<K,E>& small, K midkey, E midel, BST<K,E>& big)
{
	root = new Node<K,E>(midkey, midel, small.root, big.root);
	small.root = 0;
	big.root = 0;
}

template <class K, class E>
void BST<K,E>::TwoWayJoin(BST<K,E>& small, BST<K,E>& big)
{
	if(!small.root) { root = big.root; big.root = 0; return; }
	if(!big.root) {root = small.root; small.root = 0; return; }
	BST small2 = small;
	
	//midkey와 mide을 알아내는 과정이 전개된다.	
	Node<K,E> *temp_ptr;
	Node<K,E> *currentNode = small2.root;
	Node<K,E> *Before_currentNode;
	while(currentNode -> rightChild)
	{	
		Before_currentNode = currentNode;
		currentNode = currentNode -> rightChild;
	}
	K KEY = currentNode -> key;
	E ELEMENT = currentNode -> element;
	Before_currentNode -> rightChild = currentNode -> leftChild;
	delete currentNode;
		
	ThreeWayJoin(small2, KEY, ELEMENT, big);		
	small.root = 0;
	big.root = 0;	
}
template <class K, class E>
bool BST<K,E>::Print()
{
	cout << endl << "Inorder traversal   : "; Inorder();
	cout << endl << "Postorder traversal : "; Postorder();
	cout << endl;
}

#endif
