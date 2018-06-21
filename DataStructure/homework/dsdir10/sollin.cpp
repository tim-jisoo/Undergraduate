//허지수 3학년 B115233
#include <fstream>
#include <queue>
#include <algorithm>
#include "mstree.h"

#define		YES	1
#define		NO	0

int Num_Nodes; // # of nodes in the graph (global variable)
priority_queue<Edge, vector<Edge>, compare> *PQ;

bool Edge_compare(Edge e1, Edge e2)
{
	return (e1.v1 < e2.v1) || (e1.v1 == e2.v1 && e1.v2 < e2.v2);
}

void sollin()
{ 	
	Sets sets(Num_Nodes);
	int Num_Edges = 0;
	int Is_it_root[Num_Nodes]; fill(Is_it_root, Is_it_root + Num_Nodes, NO); //각 단계에서 선택된 minimum-cost Edge들이 저장된다. (각각의 노드마다 추가되는 간선이 중복될수있음)
	vector<Edge> SpanningTree_Edges;
	vector<Edge> v; //중복된 간선이 저장된 벡터
	int vector_index=0;
	
	int Num_Roots;	
	while(1)	
	{
		Num_Roots = 0;	
		for(int node = 0; node < Num_Nodes; node++)
		{	
			int root_of_node = sets.Find(node);	
			if(Is_it_root[root_of_node] == NO)
			{
				Is_it_root[root_of_node] = YES; //YES로 체크 되면, 해당 index와 같은 노드는 root임 -> 트리를 나누는기준!!!!!
				Num_Roots++;
			}
		}
		if(Num_Roots == 1) break;//Root가 1개이면 spanning tree를 다찾은상태.
		//이 for loop를 벗어나면 각 트리의 루트가 결정 됨. BigO(n)	
		
		for(int node = 0; node < Num_Nodes; node++)
		{
			if(Is_it_root[node] == YES)
			{
				Edge e = PQ[node].top(); PQ[node].pop(); //각 root에 대해서 최소간선을 선택
				v.push_back(e);
			}
		}//이 for loop를 벗어나면 각 트리마다 최소비용간선을 선택해서 저장을함 BigO(n)
		sort(v.begin(), v.end(), Edge_compare); //sort를 사용해서 v벡터 정렬함. 같은 엣지는 연속되게 정렬될것임.
		
		while(!v.empty())
		{
			Edge e1 = v.front(); v.erase(v.begin());
			if(!v.empty())//v가 비어있는경우 바로 밑의 문장을 실행하면 오류뜸.
			{
				Edge e2 = v.front();
				if( e1 == e2 ) v.erase(v.begin());
			}
			SpanningTree_Edges.push_back(e1);
		}//이 while loop 를 벗어나면 v컨테이너에 중복되지 않은 최소 간선들만 선택 되어진것임.
		
		Num_Edges = SpanningTree_Edges.size(); if(Num_Edges == 0) { cout << "No Spanning Tree Exists.\n"; break; } //선택할 간선이 없으면 끝난다.
		while( vector_index < Num_Edges )
		{
			Edge Choosen_Edge = SpanningTree_Edges[vector_index++];//vector index는 가 이 loop안에서 국한되지 않음을 주의해라.
			int v1root = sets.Find(Choosen_Edge.v1); int v2root = sets.Find(Choosen_Edge.v2);
			//선택된 edge의 vertex1의 root를 찾아서 v1root, vertex2의 root를 찾아서 v2root라 함.
			if(v1root > v2root) swap(v1root,v2root); sets.Union(v2root, v1root);//숫자가 작은vertex를 부모로 삼겠다. 오른쪽 인자가 parent가 될 smaller vertex
			while( !PQ[ v2root ].empty() )
			{	 
				PQ[ sets.Find(v1root) ].push( PQ[ v2root ].top() ); 
				PQ[ v2root ].pop(); 
			}//작은 vertex의 간선들을 parent vertex에게 몰아준다. 정말 중요한기능임.
			cout << Choosen_Edge;
		
		}//이 for loop를 벗어나면 new tree들이 형성됨
		fill(Is_it_root, Is_it_root + Num_Nodes, NO);//각 단계 별로 root, 즉 트리 또는 집합이 바뀌므로 다시 초기화 해줘야함.
	}
}

void ReadEdges4sollin(istream& is)
{
	PQ = new priority_queue<Edge, vector<Edge>, compare>[Num_Nodes]; 
	Edge e;

	while(GetEdge(is, Num_Nodes, e)) { PQ[e.v1].push(e); PQ[e.v2].push(e); }
}

int main(int argc, char *argv[])
{
	ifstream is; if(argc == 1) is.open("mstree.dat"); else is.open(argv[1]);
	if(!is) { cerr << "No such input file\n"; return 1; }

	is >> Num_Nodes; if(Num_Nodes <2) { cerr << "# of nodes must be 2..\n"; return 1; }
	
	try
	{	ReadEdges4sollin(is);
		sollin();
	}
	catch(char const *str) { cerr << "Exception: " << str << endl; return 1; }
}

