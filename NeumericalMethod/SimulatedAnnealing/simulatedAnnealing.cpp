#include "simulatedAnnealing.h"

SimulateAnnealing::SimulateAnnealing(){ set_default(); }

void SimulateAnnealing::set_default(AnnealingType type)
{
	if (type == User)
	{
		initial_temperature = 1.0;		//�ʱ� �µ��� 1��
		tfactor           = 0.99;		//��Ҹ���Ű�� ����. �µ� ���̴� ���ӵ��� ������
		neighberRange = 1.0;		    //neighberhood�� ���� ����
		n_inner_loop = 1000;			//�־��� �µ����� try�غ��� ����
		n_outer_loop = 250;				//�µ��� ���ӽ�Ű�� Ƚ��
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
	
	//T���� ���δ�. outer Loop�� �ݺ� �Ҽ���, global minimal�� ����� ���ٰ� �����Ѵ�.
	//global minimal �� ���������, BadCase�� �����ϴ� Ȯ���� �ٿ����Ѵ�.
	do
	{
		iter_inner = 0;
		CurrentPoint = BestPoint;

		//�־��� T�� ���ؼ�, 1000ȸ �ݺ��Ͽ� Best ���� �����Ѵ�.
		do
		{
			////////////////////////////////////////////// �������� �������� �̿����� �����Ѵ�. //////////////////////////////////////////////////////////////
			for (int i = 0; i < Dimension-1; i++)
			{
				// 0 < x < 1 ������ random num �߻�
				x = (double)rand() / RAND_MAX;
				// 0.5 < x < 1.5 ������ random num�� int index�� ���� => 50������ Ȯ���� 0 �Ǵ� 1����
				idx = (int)(x + 0.5);
				// -0.5 < x < 0.5 ������ random num �߻�
				x = (double)rand() / RAND_MAX - 0.5;
				//�������� �ϳ��� ��Ҹ� �ٲ㼭 �̿����� �����Ѵ�. 
				NeighberPoint[idx] = CurrentPoint[idx] + neighberRange * x;
			}
			NeighberVal = function(NeighberPoint);
			deltaE = NeighberVal - CurrentVal;
			///////////////////////////////////////////// ������ �̿����� �������� �׽�Ʈ �Ѵ�. //////////////////////////////////////////////////////////////////
			
			//�̿����� ����Ʈ�� ���� ���� ��, ������ Best�� �Ѿư���.
			if (NeighberVal < BestVal)
			{
				BestPoint = NeighberPoint; BestVal = NeighberVal;				//Best Update
				CurrentPoint = BestPoint;  CurrentVal = BestVal;				//Best is Current, Now.
			}
		
			//�̿����� ����Ʈ�� ��������, ���簪���ٴ� ���� ��, Good Movement�� �Ѿư���.
			//�̷��� ���� ������ ���� �� ���� �� �ִ�.
			//else���� Ȯ�������� ������ ���� �����ϰ� ������, ���� ���� ������ ����̴�.
			//���� ��� Minimal���� �Ѿư�����, ������ Minimal���� �־����� Maximum���� ���ٰ�, �ٽ� Minimal �� ������ ����̴�. => global minimum �غ��� ���̽�
			//NeighberVal < CurrentVal
			else if (deltaE < 0)
			{
				CurrentPoint = NeighberPoint; CurrentVal = NeighberVal;			//Neighbor is Current, Now.
			}
			
			//�̿����� (����Ʈ���� ����) ���簪���ٵ� ������ �� Ȯ�������� bad case�� try�غ���.
			//NeighberVal > CurrentVal (deltaE > 0)
			else
			{
				//0 < threshold < 1. ����� exp(0) = 1�̴�.
				//T(�µ�)�� ��������, exp���� �ް��ϰ� 0�� ���������.
				//outer loop�� �ѹ� �ŵ��ϸ� T���� 0.99�� �ȴ�. (�۾�����.)
				//����, threashold���� outer loop�� �ŵ��� ������ �پ�� ���̰�
				//x < thresholdBadcase �� ���� ���� �پ�� ���̴�.
				threshold = exp(-deltaE / T);
				
				//0 < x < 1 ������ ������ �� �߻�
				x = (double)rand() / RAND_MAX;

				//Ȯ�������� bad case�� try�� �غ��ڴ�.
				if (x < threshold){ CurrentPoint = NeighberPoint; CurrentVal = NeighberVal; } 
			}

			iter_inner++;
		} while (iter_inner < n_inner_loop);
		
		printf(" temperature = %f,  Best = %f  Wb = (%f,%f) \n", T, BestVal, BestPoint[0], BestPoint[1]);
		//�µ��� 0.99�� �Ѵ� (�µ��� ���δ� : annealing)
		T = tfactor*T;
		iter_outer++;
	} while (iter_outer < n_outer_loop);
}
	







