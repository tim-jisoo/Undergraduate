//허지수 3학년 B115233
#include <fstream>
#include <queue>
#include "mstree.h"

int Num_Nodes;
priority_queue< Edge, vector<Edge>, compare> PQ;
queue<Edge> *Q; //each vertex has a queue, queue array

void MoveIntoPQ_EdgesOfNode(int v)
{
	while( !Q[v].empty() )
	{
		PQ.push( Q[v].front() );
		Q[v].pop();
	}
}

void prim()
{
	Sets sets(Num_Nodes);
	int Num_Edges = 0;
	while(Num_Edges < Num_Nodes -1)
	{
		if(PQ.empty()) throw "No Spanning Tree Exists.\n";
		Edge e = PQ.top();//PQ로부터 weight가 가장 낮은 Edge를 e로함
		PQ.pop();
		int root0 = sets.Find(0);//0부터 시작했으므로, 0의 노드를 포함한 집합에서 찾은 root를 root0라 한다.
		int v1root = sets.Find(e.v1);
		int v2root = sets.Find(e.v2);	
		if( (v1root == root0 && v2root != root0) || (v1root != root0 && v2root == root0) ) 
		{
			sets.Union(v1root, v2root);
			Num_Edges++;		
			cout << e;
			if(v1root == root0)
				MoveIntoPQ_EdgesOfNode(v2root);
			else
				MoveIntoPQ_EdgesOfNode(v1root);	
		}
	}
}

void ReadEdges4prim(istream& is)
{
	Q = new queue<Edge>[Num_Nodes];
	Edge e;

	while(GetEdge(is, Num_Nodes, e))
	{ 
		Q[e.v1].push(e);  
		Q[e.v2].push(e); 
	}
	MoveIntoPQ_EdgesOfNode(0);
}

int main(int argc, char *argv[])
{
	ifstream is; if(argc ==1) is.open("mstree.dat"); else is.open(argv[1]);
	if(!is) { cerr << "No such input file\n"; return 1; }
	
	is >> Num_Nodes;
	if(Num_Nodes < 2) { cerr << "# of nodes must be larger than 1\n"; return 1; }
	
	try{ ReadEdges4prim(is); prim(); }
	catch(char const *str) { cerr << "Exception: " << str << endl; return 1; }
}
