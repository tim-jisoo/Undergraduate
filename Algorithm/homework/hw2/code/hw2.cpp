#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#define MILL 1000000

using namespace std;

class Sort
{
private:
	int Compares;//정렬 알고리즘 실행시 비교한 횟수를 저장
	double Times;//정렬 알고리즘 실행시 걸리는 시간을 저장 (us 단위)
	int* ptr;//정렬되는 데이터의 주소
	int first;  //정렬이 시작되는 데이터의 인덱스
	int last;  //정렬의 마지막이 되는 데이터의 인덱스
	void swap(int * a, int * b) { int temp = *b; *b = *a; *a = temp; }
	void partition(int* &, int, int, int&);
	void merge(int* &, int, int, int);
	void ExchangeSort(int* &, int, int);
	void QuickSort(int* &, int, int);
	void MergeSort(int * &, int, int);
public:
	Sort() :Compares(0), Times(0), ptr(0), first(0), last(0){}
	int getCompares() { return Compares; }
	double getTimes() { return Times; }
	void ExchangeSort_(int* &v, int a, int b)
	{
		Compares = 0; ptr = v; first = a; last = b;
		double start, end;
		start = clock(); ExchangeSort(v, a, b); end = clock();
		Times = end - start;
	}
	void QuickSort_(int* &v, int a, int b)
	{
		Compares = 0; ptr = v; first = a; last = b;
		double start, end;
		start = clock(); QuickSort(v, a, b); end = clock();
		Times = end - start;
	}
	void MergeSort_(int* &v,int a,int b)
	{ 
		Compares = 0; ptr = v; first = a; last = b;
		double start, end;
		start = clock(); MergeSort(v, a, b); end = clock();
		Times = end - start;
	}
friend ostream& operator<<(ostream& os, Sort& s) { for(int i = s.first; i <= s.last ; i++) cout << s.ptr[i] << " "; return os;}
};

int* generate_sorted_Data(int);
int* generate_random_Data(int);

int main()
{
	int NumOfData;		//정렬되는 데이터의 크기
	int * sorted_data;	//정렬된 데이터의 시작주소
	int * random_data;	//랜덤 데이터의 시작 주소
	Sort s;

	ifstream is("input.txt"); if (!is) { cerr << "input.txt does not exist\n"; return -1; }
	for(int i=1 ; i <= 3 ; i++)	//exchange sort와 quick sort의 정렬된 데이터 정렬 비교
	{	
		is >> NumOfData; cout << "						N = " << NumOfData << endl;
		sorted_data = generate_sorted_Data(NumOfData); //정렬된 데이터 생성 및 데이터의 주소값 반환

		s.ExchangeSort_(sorted_data, 1, NumOfData); //cout << s << endl; 
		cout << "<Exchange Sort>	" << "Key Compares :   " << s.getCompares() << "			Exe. Time:   " << s.getTimes()/MILL << endl;
																//us 단위를 반환. 그래서 1000000 으로 나눔
		s.QuickSort_(sorted_data, 1, NumOfData); //cout << s << endl; 
		cout << "<Quick Sort>	" << "Key Compares :   " << s.getCompares() << "			Exe. Time:   " << s.getTimes()/MILL << endl;

		delete sorted_data;
	}

	for(int i=1; i <=3 ; i++)  //quick sort와 merge sort의 랜덤 데이터 정렬 비교 (평균)
	{
		double time1=0, time2=0; int comp1=0, comp2=0;
		is >> NumOfData; cout << "						N = " << NumOfData << endl;

		for (int k = 0; k < 5 ; k++)
		{
			random_data = generate_random_Data(NumOfData);
			s.QuickSort_(random_data, 1, NumOfData); //cout << s << endl;
			comp1 += s.getCompares(); time1 += s.getTimes(); //평균을 내기위한 누적합
			delete random_data;
		}
		comp1 /= 5.0;  time1 /= 5.0;
		cout << "<Quick Sort>	" << "Key Compares :   " << comp1 << "			Exe. Time:   " << time1/MILL << endl;
		
		for (int j = 0; j < 5 ; j++)
		{
			random_data = generate_random_Data(NumOfData);
			s.MergeSort_(random_data, 1, NumOfData); //cout << s << endl;
			comp2 += s.getCompares(); time2 += s.getTimes();
			delete random_data;
		}
		comp2 /= 5.0; time2 /= 5.0;
		cout << "<Merge Sort>	" << "Key Compares :   " << comp2 << "			Exe. Time:   " << time2/MILL << endl;
	}
	
	is.close();
	return 0;
}

int* generate_sorted_Data(int n) {
	int* arr_ptr = new int[n + 1]; //first element is in 1-index, not using 0-index, 
	for (int i = 1; i <= n; i++) arr_ptr[i] = i;
	return arr_ptr;
}

int* generate_random_Data(int n) {
	srand((unsigned int)time(NULL));
	int* arr_ptr = new int[n + 1]; //first element is in 1-index, not using 0-index, 
	for (int i = 1; i <= n; i++) arr_ptr[i] = rand();
	return arr_ptr;
}

void Sort::ExchangeSort(int* &v, int low, int high) {
	for (int i = low; i <= high - 1; i++)
		for (int j = i + 1; j <= high; j++) {
			if (v[j] < v[i]) swap(&v[j], &v[i]); Compares++;
		}
}

void Sort::partition(int* &v, int low, int high, int& pivot_index) {
	int pivot = v[low];
	int j = low;
	for (int i = low + 1; i <= high; i++) {
		if (v[i] < pivot) swap(&v[i], &v[++j]); Compares++;
	}
	pivot_index = j;
	swap(&v[low], &v[pivot_index]);
}

void Sort::QuickSort(int* &v, int low, int high) {
	int pivot_index;
	if (high > low) {
		partition(v, low, high, pivot_index);
		QuickSort(v, low, pivot_index - 1); 
		QuickSort(v, pivot_index + 1, high);
	}
}

void Sort::merge(int* &v, int low, int mid, int high) {
	int i = low, j = mid + 1, NumOfData = high - low + 1;
	int* sub = new int[NumOfData]; int k = 0; //sub uses 0-index
	while (i <= mid && j <= high) {
		if (v[i] < v[j]) sub[k++] = v[i++];
		else sub[k++] = v[j++];
		Compares++;
	}
	if (i > mid) for (; j <= high; j++) sub[k++] = v[j];
	else for (; i <= mid; i++) sub[k++] = v[i];
	for (int x = 0; x < NumOfData; x++) v[low + x] = sub[x];
	delete sub;
}

void Sort::MergeSort(int * &v, int low, int high) {
	if (low < high) {
		int mid = (low + high) / 2;
		MergeSort(v, low, mid);
		MergeSort(v, mid + 1, high);
		merge(v, low, mid, high);
	}
}
