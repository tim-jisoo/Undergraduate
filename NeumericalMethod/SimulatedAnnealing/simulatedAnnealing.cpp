#include "simulatedAnnealing.h"

SimulateAnnealing::SimulateAnnealing(){ set_default(); }

void SimulateAnnealing::set_default(AnnealingType type)
{
	if (type == User)
	{
		initial_temperature = 1.0;		//초기 온도는 1로
		tfactor           = 0.99;		//어닐링시키는 정도. 온도 줄이는 가속도라 생각해
		neighberRange = 1.0;		    //neighberhood의 범위 설정
		n_inner_loop = 1000;			//주어진 온도에서 try해보는 정도
		n_outer_loop = 250;				//온도를 감속시키는 횟수
	}
}

void SimulateAnnealing :: find_global_min(double(*function)(vector<double>), vector<double> initialPoint)
{	
	//NeighberPoint : NewPoint
	//CurrentPoint : OldPoint
	vector <double> BestPoint(2), CurrentPoint(2), NeighberPoint(2);
	double BestVal, CurrentVal, NeighberVal;
	double T, threshold, x;
	int iter_inner, iter_outer,Dimension,idx, deltaE;

	//////////////////////////////////////// initialization //////////////////////////////////////////////////////////
	Dimension = 2;
	T = initial_temperature;
	iter_inner = 0; iter_outter = 0;
	CurrentPoint = initialPoint; CurrentVal = function(initialPoint);
	BestPoint = CurrentPoint; BestVal = CurrentVal;
	srand(12345671);
	/////////////////////////////////////// Execution ///////////////////////////////////////////////////////////////
	
	//T값을 줄인다. outer Loop를 반복 할수록, global minimal에 가까워 진다고 가정한다.
	//global minimal 에 가까워지면, BadCase를 선택하는 확률을 줄여야한다.
	do
	{
		iter_inner = 0;
		CurrentPoint = BestPoint;

		//주어진 T에 대해서, 1000회 반복하여 Best 값을 갱신한다.
		do
		{
			////////////////////////////////////////////// 현재점을 기준으로 이웃점을 결정한다. //////////////////////////////////////////////////////////////
			for (int i = 0; i < Dimension-1; i++)
			{
				// 0 < x < 1 사이의 random num 발생
				x = (double)rand() / RAND_MAX;
				// 0.5 < x < 1.5 사이의 random num을 int index에 저장 => 50프로의 확률로 0 또는 1선택
				idx = (int)(x + 0.5);
				// -0.5 < x < 0.5 사이의 random num 발생
				x = (double)rand() / RAND_MAX - 0.5;
				//현재점의 하나의 요소만 바꿔서 이웃점을 선택한다. 
				NeighberPoint[idx] = CurrentPoint[idx] + neighberRange * x;
			}
			NeighberVal = function(NeighberPoint);
			deltaE = NeighberVal - CurrentVal;
			///////////////////////////////////////////// 생성한 이웃점을 기준으로 테스트 한다. //////////////////////////////////////////////////////////////////
			
			//이웃점이 베스트값 보다 좋을 때, 무조건 Best를 쫓아간다.
			if (NeighberVal < BestVal)
			{
				BestPoint = NeighberPoint; BestVal = NeighberVal;				//Best Update
				CurrentPoint = BestPoint;  CurrentVal = BestVal;				//Best is Current, Now.
			}
		
			//이웃값이 베스트값 나쁘지만, 현재값보다는 좋을 때, Good Movement를 쫓아간다.
			//이러한 경우는 다음과 같을 때 생길 수 있다.
			//else에서 확률적으로 안좋은 값을 선택하고 다음에, 좋은 값을 선택한 경우이다.
			//예를 들어 Minimal값을 쫓아가지만, 가끔은 Minimal에서 멀어지는 Maximum으로 갔다가, 다시 Minimal 로 빠지는 경우이다. => global minimum 극복한 케이스
			//NeighberVal < CurrentVal
			else if (deltaE < 0)
			{
				CurrentPoint = NeighberPoint; CurrentVal = NeighberVal;			//Neighbor is Current, Now.
			}
			
			//이웃값이 (베스트값은 물론) 현재값보다도 안좋을 때 확률적으로 bad case를 try해본다.
			//NeighberVal > CurrentVal (deltaE > 0)
			else
			{
				//0 < threshold < 1. 참고로 exp(0) = 1이다.
				//T(온도)가 작을수록, exp값은 급격하게 0에 가까워진다.
				//outer loop를 한번 거듭하면 T값은 0.99배 된다. (작아진다.)
				//따라서, threashold값은 outer loop를 거듭할 때마다 줄어들 것이고
				//x < thresholdBadcase 일 경우는 점점 줄어들 것이다.
				threshold = exp(-deltaE / T);
				
				//0 < x < 1 사이의 임의의 수 발생
				x = (double)rand() / RAND_MAX;

				//확률적으로 bad case로 try를 해보겠다.
				if (x < threshold){ CurrentPoint = NeighberPoint; CurrentVal = NeighberVal; } 
			}

			iter_inner++;
		} while (iter_inner < n_inner_loop);
		
		printf(" temperature = %f,  Best = %f  Wb = (%f,%f) \n", T, BestVal, BestPoint[0], BestPoint[1]);
		//온도를 0.99배 한다 (온도를 줄인다 : annealing)
		T = tfactor*T;
		iter_outer++;
	} while (iter_outer < n_outer_loop);
}
	







