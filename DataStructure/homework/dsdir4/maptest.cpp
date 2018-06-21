//허지수 3학년 B115233
#include <iostream>
#include <map>
using namespace std;

struct cmp_str
{
	bool operator() (char const* a, char const *b)
	{
		return strcmp(a, b) < 0; 
	}
};

map<char*, float, cmp_str> ST; //Key의 자료형이 char* 임. char*의 문자열을 비교하고자한다면 cmp_str을 정의해주어야함
						//만약 arg3을 정의하지 않으면, 임의로 저장되는 주소값의 대소를 비교할것임.
void PrintValue(char* name)
{
	map<char*, float, cmp_str>::iterator iter;
	iter = ST.find(name); //map의 find함수는 인자와 해당하는 key를 찾으면, 그 pair를 가르키는 iterator를 return한다. 찾지못하면 ST.end()를 return함.
	if(iter == ST.end()) cout << name << "does not exist" << endl;
	else cout << iter->first << " " << iter->second << endl;
}

void StoreIntoST(char* name, float val)
{
	map<char*, float, cmp_str>::iterator iter;
	iter = ST.find(name);
	if(iter != ST.end() ) ST.erase(name); // name과 같은 이름의 저장된 data를 제거
	ST[name] = vall; // [] 안에 name으로 접근이 가능.... 이는 map<> template안에 [] 연산자 오버로딩이 존재함.ㅑ
			 // 연산자 오버로딩을 사용하지 않겠다면 ... ST.insert( pair<char*, float>(name,val) ) 를 하면 된다.
}

int main()
{
	StoreIntoST("abro", 23);
	StoreIntoST("bb", 34.5);
	StoreIntoST("haha",99);
	StoreIntoST("bb", 1234.5);
	PrintValue("bb");
	PrintValue("haha");
	PrintValue("hoho");

}

