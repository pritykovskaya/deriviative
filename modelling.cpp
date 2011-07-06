#include "modelling.h"
#include "R32M.H"
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;
const double PI = M_PI;

Model::Model() {}

vector<int> Model::exp_mod() {
	double s = 0;
	double tmp = 0;
	vector<int> res;
	while (s < len) {
		tmp = - log(rnunif()) / lambda;
		s += ceil(tmp);
		res.push_back(s);
	}
	return(res);	
};

vector<double> cum_sum(vector<double> v) {
	vector<double> res;
	double cur = 0;
	for (int i = 0; i < v.size(); i++) {
		cur += v[i];
		res.push_back(cur);
	}
	return res;
}

vector<double> Model::norm_mod(double normal_mean, double normal_var, int len) {
	vector<double> tmp;
	for (int i = 0; i < len; ++i){
		tmp.push_back(normal_var * sqrt(-2 * log(rnunif())) * cos(2 * PI * rnunif()) + normal_mean);
	}
	return(tmp);
};

vector<double> Model::trend_point_by_point(vector<int> change_time, vector<double> tang) {
	vector<double> traj;
	double cur_k = tang[0];
	double cur_b = start_point;
	int j = 0;
	for (int i = 0; i < len; ++i){
		traj.push_back(cur_k * i + cur_b);

		if (i == change_time[j]){
			++j;
			cur_k = tang[j];
			cur_b = traj[i] - i * cur_k ;
		}
	}
	return(traj);
};

vector<double> Model::model_trend() {
	vector<int> change_time = exp_mod();
	// еще нужен угол наклона в первой точке
	vector<double> tang = norm_mod(tang_mean, tang_var, change_time.size() + 1);
	trend = trend_point_by_point(change_time, tang);
	return(trend);
};

vector<double> Model::model_noise() {
	noise = cum_sum(norm_mod(0, noise_var, len));
	return(noise);
};

void Model::set_everything(int my_len, double my_start_point, double my_noise_var, double my_lambda, double my_tang_mean, double my_tang_var) {
	len = my_len;
	start_point = my_start_point;
	noise_var = my_noise_var;
	lambda = my_lambda;
	tang_var = my_tang_var;
	tang_mean = my_tang_mean;	
}

vector<double> Model::model_row(){
	row.clear();
	bool became_negative = false;
	for (int i = 0; i < trend.size(); ++i){
		if (became_negative == false){
			if (trend[i] + noise[i] <= 0){
				became_negative = true;
				row.push_back(0);
				trend[i] = 0;
				continue;
			}
			row.push_back(trend[i] + noise[i]);
			continue;
		}
		row.push_back(0);
		trend[i] = 0;
	}
	return(row);
}

vector<double> Model::get_trend() {
	return trend;
}

vector<double> Model::get_row() {
	return row;
}

int Model::get_len(){
	return len;
};

double Model::get_start_point(){
	return start_point;
};
double Model::get_noise_var(){
	return noise_var;
};
double Model::get_lambda(){
	return lambda;
};
double Model::get_tang_mean(){
	return tang_mean;
};
double Model::get_tang_var(){
	return tang_var;
};