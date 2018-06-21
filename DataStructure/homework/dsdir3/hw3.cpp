//허지수 3학년 B115233
#include <iostream>
#include <fstream>

using namespace std;

void getdata(istream&, int&, int&);
void Path(int, int);

int main(int argc, char* argv[])
{
	int m,p; // m by p maze
	if(argc == 1)
		cerr << "Usage: " << argv[0] << " maze_data_file" << endl;
	else
	{
		ifstream is(argv[1]);
		if(!is)
		{
			cerr << argv[1] << "does not exist\n";  // argv[1]에 해당하는 파일이 존재하지 않으면 error 를 띄우겠다.
			return 1;
		}
		cout << "For maze datafile (" << argv[1] << ") \n"; 
		getdata(is,m,p);
		is.close();
		Path(m,p); // 미로찾기 프로그램을 함수 하나로 구현함.
	}
}
