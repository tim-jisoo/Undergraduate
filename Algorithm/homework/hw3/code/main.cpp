#include <iostream>
#include <fstream>

using namespace std;

void ShortestPath(int total, int **W, int start, int dst, int transferTime);
void print_matrix(int** W, int nrow, int ncol);

int main()
{
	ifstream is("input.txt"); if (!is) { cerr << "input.txt does not exist\n"; return 1; }
	//////////////////////////////////////////////////////////////////////////////////////

	//0호선 없음 => 안쓴다.
	//nline : 지하철 라인의 수
	//nnode[2] = 14 : 2호선은 총 14개의 역(node)가 있다.
	int nline; is >> nline;
	int *  nnode = new int[nline + 1];
	int line, nnode_;
	for (int iter = 1; iter <= nline; iter++)
	{
		is >> line >> nnode_;
		nnode[line] = nnode_;
	}

	//nnode_accum[2] = 28 : 2호선까지의 누적 지하철 역 수
	int * nnode_accum = new int[nline + 1];
	int accum = 0; nnode_accum[0] = 0;
	for (int l = 1; l <= nline; l++)
	{
		accum += nnode[l];
		nnode_accum[l] = accum;
	}
	int total = nnode_accum[nline]; //전체 지하철역들의 수

	//Edge 배열을 생성한다.
	//0-row, 0-col은 사용하지 않을 것이다.
	int** W = new int*[total + 1];
	for (int i = 1; i <= total; i++)
	{
		W[i] = new int[total + 1];
		for (int j = 1; j <= total; j++)
			W[i][j] = 100;
	}

	////////////////////////////////////////////////////////////// Graph intialization ///////////////////////////////////////////////////////
	for (int l = 1; l <= nline; l++) //각 line에서
		for (int n = nnode_accum[l - 1] + 1; n <= nnode_accum[l]; n++) //각 node에서
		{
			W[n][n] = 0;							//같은 역에서 같은 역으로 가는 경우는 당연히 0이다.
			if (n == nnode_accum[l - 1] + 1)			// 첫 역인 경우 그다음 역하고만 연결됨
				W[n][n + 1] = 1;
			else if (n == nnode_accum[l])			// 마지막 역인경우 이전 역하고만 연결됨
				W[n][n - 1] = 1;
			else									// 호선의 중간역인경우 이전 그리고 다음역과 연결됨
			{
				W[n][n - 1] = 1;
				W[n][n + 1] = 1;
			}
		}

	///////////////////////////////////////////////////////////////// Transfer ///////////////////////////////////////////////////////

	//ntransfer : 환승역의 수
	int ntransfer; int line1, node1, line2, node2; int row, col;
	is >> ntransfer;
	for (int i = 1; i <= ntransfer; i++)
	{
		is >> line1 >> node1 >> line2 >> node2;
		row = nnode_accum[line1 - 1] + node1;
		col = nnode_accum[line2 - 1] + node2;
		W[row][col + 0] = 0;
		W[row][col - 1] = 1; //이렇게 할 수 있는 이유는 출발역과 도착역이 환승역이 아니기 때문
		W[row][col + 1] = 1; //위와 같음

		W[col][row + 0] = 0;
		W[col][row - 1] = 1;
		W[col][row + 1] = 1;
	}

	print_matrix(W, total + 1, total + 1);
	//////////////////////////////////////////////////////////////////// test //////////////////////////////////////////////////////////////
	//ndata : 데이터의 수
	//transferTime : 환승할 시 걸리는 시간
	//start : 출발역, dst : 목적지역
	int ndata, transferTime; int start, dst;
	is >> ndata;
	for (int i = 1; i <= ndata; i++)
	{
		is >> transferTime >> line1 >> node1 >> line2 >> node2;
		start = nnode_accum[line1 - 1] + node1; // cout << start << endl;
		dst = nnode_accum[line2 - 1] + node2;	// cout << dst << endl;
		ShortestPath(total, W, start, dst, transferTime);
	}
	/////////////////////////////////////////////////////////////////////////////////////
	is.close();

	delete nnode; nnode_accum;
	for (int i = 1; i <= total; i++) delete W[i];
	delete W;
}

void ShortestPath(int total, int **W, int start, int dst, int transferTime)
{
	int* cost = new int[total + 1]; //지금으로써 vi까지 가는데 드는 최소시간 cost[vi]이다.
	bool *s = new bool[total + 1];  //vi의 최단거리가 확정되면 s[vi] = true가 된다.
	int min, vnear;					//vnear : 가장짧은 cost를 갖는 외부 node이다.
	
	//////////////////////////////////// s[], cost[] inistialization ///////////////////////////////////////////////////
	for (int i = 1; i <= total; i++)
	{
		s[i] = false;
		cost[i] = W[start][i];
	}
	
	s[start] = true; //시작점 자신은 당연히 최단거리가 0이다.
	cost[start] = 0;
	


	for (int i = 1; i <= total - 2; i++)
	{
		////////////////////////////////////////////// choose minima (greedy algorithm) ///////////////////////////////////
		min = 100;
		for (int k = 1 ; k <= total ; k++)
			if (!s[k] && (cost[k] < min)) //확정되지 않은 역중에서, 출발점으로 부터 가장짧은 cost를 갖는 역 vnear를 찾는다.
			{
				min = cost[k]; vnear = k;
			}

		s[vnear] = true;
		
		/////////////////////////////////////////////// update cost ///////////////////////////////////////////////////////////
		for (int k = 1; k <= total; k++) 
		{
			if (!s[k]) //확정되지 않은 역(k)들 중에서
			{	
				if ((vnear - k != 1) && (vnear - k != -1)) //환승 했다면 (abs(vnear - k) != 1 ) 역번호들이 연속적이지 않을 것이다.
				{
					if (cost[vnear] + W[vnear][k] + transferTime < cost[k]) //이동시간을 함께 고려해야한다.
						cost[k] = cost[vnear] + W[vnear][k] + transferTime;
				}
				else                                       //환승하지 않았다면
				{
					if (cost[vnear] + W[vnear][k] < cost[k])
						cost[k] = cost[vnear] + W[vnear][k];
				}
			}
			
		}
	}


	cout << cost[dst] << endl;
	delete cost, s;
}

void print_matrix(int** W, int nrow, int ncol)
{
	int cnt = 0;
	for (int i = 0; i < nrow; i++) {
		for (int j = 0; j < ncol; j++)
		{
			if (i == 0 && j == 0) cout << 0 << "	";
			else if (i == 0 && j != 0) cout << ++cnt << "	";
			else if (i != 0 && j == 0) cout << i << "	";
			else cout << W[i][j] << "	";
		}
		cout << endl;
	}
}