//허지수 3학년 B115233
#ifndef MSTREE_H
#define MSTREE_H

#include <iostream>
using namespace std;

class Sets
{
private:
	int *parent;
	int n;
public:
	Sets(int);
	void Union(int i, int j);
	int Find(int i);
};

struct Edge
{
	int v1, v2;
	double weight;
	bool operator==(Edge& e2);
	bool operator!=(Edge& e2);
};

struct compare{   bool operator()(Edge e1, Edge e2) {return e1.weight > e2.weight;}   };

ostream& operator<<(ostream &os, Edge &e);
bool GetEdge(istream &is, const int Num_Nodes, Edge &e);

#endif
