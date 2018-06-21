#include <iostream>
#include <fstream>

using namespace std;

void ShortestPath(int total, int **W, int start, int dst, int transferTime);
void print_matrix(int** W, int nrow, int ncol);

int main()
{
	ifstream is("input.txt"); if (!is) { cerr << "input.txt does not exist\n"; return 1; }
	//////////////////////////////////////////////////////////////////////////////////////

	//0ȣ�� ���� => �Ⱦ���.
	//nline : ����ö ������ ��
	//nnode[2] = 14 : 2ȣ���� �� 14���� ��(node)�� �ִ�.
	int nline; is >> nline;
	int *  nnode = new int[nline + 1];
	int line, nnode_;
	for (int iter = 1; iter <= nline; iter++)
	{
		is >> line >> nnode_;
		nnode[line] = nnode_;
	}

	//nnode_accum[2] = 28 : 2ȣ�������� ���� ����ö �� ��
	int * nnode_accum = new int[nline + 1];
	int accum = 0; nnode_accum[0] = 0;
	for (int l = 1; l <= nline; l++)
	{
		accum += nnode[l];
		nnode_accum[l] = accum;
	}
	int total = nnode_accum[nline]; //��ü ����ö������ ��

	//Edge �迭�� �����Ѵ�.
	//0-row, 0-col�� ������� ���� ���̴�.
	int** W = new int*[total + 1];
	for (int i = 1; i <= total; i++)
	{
		W[i] = new int[total + 1];
		for (int j = 1; j <= total; j++)
			W[i][j] = 100;
	}

	////////////////////////////////////////////////////////////// Graph intialization ///////////////////////////////////////////////////////
	for (int l = 1; l <= nline; l++) //�� line����
		for (int n = nnode_accum[l - 1] + 1; n <= nnode_accum[l]; n++) //�� node����
		{
			W[n][n] = 0;							//���� ������ ���� ������ ���� ���� �翬�� 0�̴�.
			if (n == nnode_accum[l - 1] + 1)			// ù ���� ��� �״��� ���ϰ� �����
				W[n][n + 1] = 1;
			else if (n == nnode_accum[l])			// ������ ���ΰ�� ���� ���ϰ� �����
				W[n][n - 1] = 1;
			else									// ȣ���� �߰����ΰ�� ���� �׸��� �������� �����
			{
				W[n][n - 1] = 1;
				W[n][n + 1] = 1;
			}
		}

	///////////////////////////////////////////////////////////////// Transfer ///////////////////////////////////////////////////////

	//ntransfer : ȯ�¿��� ��
	int ntransfer; int line1, node1, line2, node2; int row, col;
	is >> ntransfer;
	for (int i = 1; i <= ntransfer; i++)
	{
		is >> line1 >> node1 >> line2 >> node2;
		row = nnode_accum[line1 - 1] + node1;
		col = nnode_accum[line2 - 1] + node2;
		W[row][col + 0] = 0;
		W[row][col - 1] = 1; //�̷��� �� �� �ִ� ������ ��߿��� �������� ȯ�¿��� �ƴϱ� ����
		W[row][col + 1] = 1; //���� ����

		W[col][row + 0] = 0;
		W[col][row - 1] = 1;
		W[col][row + 1] = 1;
	}

	print_matrix(W, total + 1, total + 1);
	//////////////////////////////////////////////////////////////////// test //////////////////////////////////////////////////////////////
	//ndata : �������� ��
	//transferTime : ȯ���� �� �ɸ��� �ð�
	//start : ��߿�, dst : ��������
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
	int* cost = new int[total + 1]; //�������ν� vi���� ���µ� ��� �ּҽð� cost[vi]�̴�.
	bool *s = new bool[total + 1];  //vi�� �ִܰŸ��� Ȯ���Ǹ� s[vi] = true�� �ȴ�.
	int min, vnear;					//vnear : ����ª�� cost�� ���� �ܺ� node�̴�.
	
	//////////////////////////////////// s[], cost[] inistialization ///////////////////////////////////////////////////
	for (int i = 1; i <= total; i++)
	{
		s[i] = false;
		cost[i] = W[start][i];
	}
	
	s[start] = true; //������ �ڽ��� �翬�� �ִܰŸ��� 0�̴�.
	cost[start] = 0;
	


	for (int i = 1; i <= total - 2; i++)
	{
		////////////////////////////////////////////// choose minima (greedy algorithm) ///////////////////////////////////
		min = 100;
		for (int k = 1 ; k <= total ; k++)
			if (!s[k] && (cost[k] < min)) //Ȯ������ ���� ���߿���, ��������� ���� ����ª�� cost�� ���� �� vnear�� ã�´�.
			{
				min = cost[k]; vnear = k;
			}

		s[vnear] = true;
		
		/////////////////////////////////////////////// update cost ///////////////////////////////////////////////////////////
		for (int k = 1; k <= total; k++) 
		{
			if (!s[k]) //Ȯ������ ���� ��(k)�� �߿���
			{	
				if ((vnear - k != 1) && (vnear - k != -1)) //ȯ�� �ߴٸ� (abs(vnear - k) != 1 ) ����ȣ���� ���������� ���� ���̴�.
				{
					if (cost[vnear] + W[vnear][k] + transferTime < cost[k]) //�̵��ð��� �Բ� ����ؾ��Ѵ�.
						cost[k] = cost[vnear] + W[vnear][k] + transferTime;
				}
				else                                       //ȯ������ �ʾҴٸ�
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