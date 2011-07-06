#include <vector>
#include "PROBDIST.H"
using namespace std;

class Strategy {
private:
	double alfa;
	double timeframe;
	vector<double> price_row; 
public:
	Strategy(double alfa, double timeframe);
	void update_timeframe_and_move_to_next_day(double new_price);
	Decision make_decision_for_cur_day();
};

enum Decision {
	SELL, BUY, DO_NOTHING
};