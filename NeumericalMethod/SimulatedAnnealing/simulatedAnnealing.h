#include <stdio.h>
#include <math.h>
#include<vector>

using namespace std;
enum AnnealingType{ Low, High, User};

class SimulateAnnealing
{
private:
	double initial_temperature, tfactor, neighberRange;
	int n_inner_loop, n_outer_loop;
	double eTolerance;

public:
	SimulateAnnealing();
	void set_default(AnnealingType type = User);
	void find_global_min( double(*func)(vector<double>), vector<double> Wi);
};

