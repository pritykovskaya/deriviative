#include "StrategyBuilder.h"

StrategyBuilder::StrategyBuilder(vector<double> &_row) {
	for (int i = 0; i < _row.size(); ++i) {
		row.push_back(_row[i]);
	}

	change.push_back(0);
	
	for (int i = 1; i < row.size(); ++i){
		change.push_back(row[i] - row[i - 1]);
 	}
}

pair<int, double> StrategyBuilder::tune(){
	int m = 1;
	double alfa = 0.1;
	int best_m = 0;
	double best_alfa = 0;

	double best_profit = 0;
	double old_criteria = 0;
	double new_criteria = 0;

	pair<double, double> cur_test;
	cur_test.first = 0;
	cur_test.second = 0;
	
	while (m <= row.size() / 4 ) {
		alfa = 0.1;
		while (alfa >= 0.01) {
			make_strategy(m, alfa);
			cur_test = test_strategy();
			double new_criteria = 0;

			// если просадка меньше одного процента то финального капитала, то мы ей пренебрегаем
			if (abs(cur_test.second) < cur_test.first / 100) {
				new_criteria = cur_test.first;
			} else {
				new_criteria = cur_test.first / abs(cur_test.second);
			}
			
			
			if (new_criteria > old_criteria) {
				old_criteria = new_criteria;
				best_m = m;
				best_alfa = alfa;
				best_profit = cur_test.first;
				
				best_behavior.clear();
				for (int i = 0; i < res.size(); ++i) {
					best_behavior.push_back(res[i]);
				}
			}
			alfa -= 0.05;
		}
		m += 1;
	}
	
	res.clear();
	for (int i = 0; i < best_behavior.size(); ++i) {
		res.push_back(best_behavior[i]);
	}

	pair<int, double> tmp;
	tmp.first = best_m;
	tmp.second = best_alfa;
	return (tmp);
};

void StrategyBuilder::pre_calc(int m) {
	moving_sum.clear();
	sqr_moving_sum.clear();

	double tmp_s = 0;
	double sqr_tmp_s = 0;
	for (int i = 0; i < m; ++i) {
		tmp_s += change[i];
		sqr_tmp_s += change[i] * change[i];
	}
	
	moving_sum.push_back(tmp_s / m);
	sqr_moving_sum.push_back(sqr_tmp_s / m);
	int j = 1;
	
	for (int i = m; i < change.size(); ++i){
		// invariant: calc sum from i-m+1 to i
		
		moving_sum.push_back(moving_sum[j - 1] + change[i]/m - change[i - m]/m);
		sqr_moving_sum.push_back(sqr_moving_sum[j - 1] + change[i] * change[i]/m - change[i - m] * change[i - m]/m);
		
		++j;
	}
}

vector<int> StrategyBuilder::what_to_do(int m, double alfa){
	pre_calc(m);

	vector<double> up_b;
	vector<double> down_b;
	vector<int> res;

	double x_gamma = xNormal(0.5 + 0.5 - alfa/2); 
	double sqrt_m = sqrt((double)m);

	int status = 0;

	for (int i = 0; i < moving_sum.size(); ++i) {
		double variance = abs(sqr_moving_sum[i] - moving_sum[i] * moving_sum[i]);
		double tmp = x_gamma * sqrt(variance) / sqrt_m;

		down_b.push_back(moving_sum[i] - tmp);
		up_b.push_back(moving_sum[i] + tmp);
		
		if (status == 1) {
			if (up_b[i] < -0.0001) {
				res.push_back(-1);
				status = 0;
				continue;
			}
		}
		if (status == 0) {
			if (down_b[i] > 0.0001) {
				res.push_back(1);
				status = 1;
				continue;
			}
		}
		res.push_back(0);
	}
	return(res);
}

void StrategyBuilder::make_strategy(int m, double alfa){
	res.clear();
	for (int i = 0; i < m; ++i){
		res.push_back(0);
	}
	vector<int> tmp = what_to_do(m, alfa);
	for (int i = 0; i < tmp.size(); ++i){
		res.push_back(tmp[i]);
	}
};

pair<double, double> StrategyBuilder::test_strategy() {
	double capital = 1000;
	double buy_price = 0;
	double amount_of_derivative = 0;
	double drawdown = 0;
	double diff;
	int status = 0;

	for (int i = 0; i < row.size(); ++i){
		if (res[i] == 1) {
			status = 1;
			capital -= 100;
			buy_price = row[i];
			amount_of_derivative = 100 / buy_price;

		}
		if (res[i] == -1) {
			status = 0;
			capital += row[i] * amount_of_derivative;
			diff = row[i] - buy_price;
			if (diff < drawdown) {
				drawdown = diff;
			}	
		}
	}
	if (status == 1) {
		capital += amount_of_derivative * row[row.size() - 1]; 
		diff = (row[row.size() - 1] - buy_price);
		if (diff < drawdown) {
				drawdown = diff;
		}
	}
	pair<double, double> tmp;
	tmp.first = capital;
	tmp.second = drawdown;
	return(tmp);
}


vector<int> StrategyBuilder::get_res(){
	return(res);
}

vector<double> StrategyBuilder::build_profit_fun(vector<int> best_behavior){
	vector<double> profit_fun;
	double capital = 1000;
	double amount_of_derivative = 0;
	

	for (int i = 0; i < row.size(); ++i) {
		if (best_behavior[i] == 1) {
			capital -= 100;
			amount_of_derivative = 100 / row[i];
		}
		if (best_behavior[i] == -1) {
			capital += amount_of_derivative * row[i];
			amount_of_derivative = 0;
		}
		profit_fun.push_back(capital + amount_of_derivative * row[i]);
	}
	return(profit_fun);
}