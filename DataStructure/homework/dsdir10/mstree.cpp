//허지수 3학년 B115233
#include "mstree.h"

Sets::Sets(int number_of_elements) //Sets class 의 생성자 정의
{
	if(number_of_elements < 2) throw "Must have at least 2 elements.\n";
	n = number_of_elements;
	parent = new int[n];
	fill(parent, parent + n, -1);
}

void Sets::Union(int i, int j) { parent[i] = j; }

int Sets::Find(int i) { while( parent[i]>=0 ) i = parent[i]; return i; }

bool Edge::operator==(Edge& e2) { return (v1 == e2.v1 && v2 == e2.v2); }

bool Edge::operator!=(Edge& e2) { return (v1 != e2.v1 || v2 != e2.v2); }

ostream& operator<<(ostream &os, Edge &e) { os << "(" << e.v1 << " , " << e.v2 << ") : " << e.weight << endl; return os; }

bool GetEdge(istream& is, const int Num_Nodes, Edge &e)
{
	is >> e.v1 >> e.v2 >> e.weight;
	if(!is.good()) return false;
	
	if(e.v1<0 || e.v2>=Num_Nodes || e.v2<0 || e.v2>=Num_Nodes || e.v1==e.v2) throw "Incorrect Edge\n";
	
	if(e.v2 < e.v1) swap(e.v1, e.v2);
	return true;
}
