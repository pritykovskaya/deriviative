#include "PROBDIST.H"
#include <math.h>
#include <vector>
using namespace std;

class StrategyBuilder {
private:
	vector<double> change;
	vector<double> row;
	vector<int> res;
	vector<double> moving_sum;
	vector<double> sqr_moving_sum;
	vector<int> best_behavior;

	void pre_calc(int m);
	vector<int> what_to_do(int m, double alfa);

public:
	pair<double, double> test_strategy();
	StrategyBuilder(vector<double> &_row);
	pair<int, double> tune();
	vector<int> get_res();
	void make_strategy(int m, double alfa);
	vector<double> build_profit_fun(vector<int> best_behavior);

};