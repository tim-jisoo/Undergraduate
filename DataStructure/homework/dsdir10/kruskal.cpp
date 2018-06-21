//허지수 3학년 B115233
#include <fstream>
#include <queue>
#include "mstree.h"

int Num_Nodes; // # of nodes in the graph (global variable)
priority_queue<Edge, vector<Edge>, compare> PQ;

void kruskal()
{
	Sets sets(Num_Nodes);
	int Num_Edges = 0; // # of eneges that is found up to now.
	while(Num_Edges < Num_Nodes - 1)
	{
		if(PQ.empty()) throw "No Spanning Tree Exists.\n";
		Edge e = PQ.top(); PQ.pop();
		int v1root = sets.Find(e.v1); int v2root = sets.Find(e.v2);
		if(v1root != v2root)
		{
			sets.Union(v1root, v2root);
			Num_Edges++;
			cout << e;
		}
	}
}

void ReadEdges4kruskal(istream& is)
{
	Edge e;
	while(GetEdge(is, Num_Nodes, e))
		PQ.push(e);
}

int main(int argc, char *argv[])
{
	ifstream is; if(argc == 1) is.open("mstree.dat"); 
		     else/*argc>1*/is.open(argv[1]);
	if(!is) { cerr << "No such input file\n"; return 1; }

	is >> Num_Nodes; if(Num_Nodes <2) { cerr << "# of nodes must be 2..\n"; return 1; }
	try
	{	ReadEdges4kruskal(is);
		kruskal();
	}
	catch(char const *str) { cerr << "Exception: " << str << endl; return 1; }
}

