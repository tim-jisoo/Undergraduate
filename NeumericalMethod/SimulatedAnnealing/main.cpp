#include "simulatedAnnealing.h"

double performance(vector<double> param)
{
	double x = param[0];
	double y = param[1];
	double cost = fmin(1.0, x*x + y*y) + fmin(1.0, (x - 1)*(x - 1) + (y + 1)*(y + 1) - 0.75) + fmin(1.0, (x + 1)*(x + 1) + (y + 1)*(y + 1) - 0.25) + fmin(1.0, (x + 1)*(x + 1) + y*y - 0.25);
	return cost;
}

void main(void)
{
	vector<double> initialPoint(2);
	double cost;
	SimulateAnnealing SA;

	double mc=1000, mx, my;
	for (double x = -2; x <= 2; x += 0.01)
		for (double y = -2; y <= 2; y += 0.01)
		{
			initialPoint[0] = x; initialPoint[1] = y;
			cost = performance(initialPoint);
			if (cost < mc)
			{
				mc = cost;
				mx = x;
				my = y;
			}
		}
	

	SA.find_global_min(performance, initialPoint);
}