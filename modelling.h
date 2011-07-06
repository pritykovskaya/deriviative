#ifndef _MODEL_H
#define _MODEL_H 


#include <vector>
using namespace std;

class Model{
private:
	int len;
	double start_point;
	double noise_var;
	double lambda;
	double tang_mean;
	double tang_var;
	vector<double> trend;
	vector<double> noise;
	vector<double> row;

public:
	Model();
	//Model(int my_len, double my_start_point, double my_noise_var, double my_lambda, double my_tang_mean, double my_tang_var);
	vector<int> exp_mod();
	vector<double> norm_mod(double normal_mean, double normal_var, int len);
	vector<double> trend_point_by_point(vector<int> change_time, vector<double> tang);
	vector<double> model_trend();
	vector<double> model_noise();
	vector<double> model_row();
	vector<double> get_trend();
	vector<double> get_row();
	void set_everything(int my_len, double my_start_point, double my_noise_var, double my_lambda, double my_tang_mean, double my_tang_var);
	
	int get_len();
	double get_start_point();
	double get_noise_var();
	double get_lambda();
	double get_tang_mean();
	double get_tang_var();
	
	double profit;
	double drawdown;
};
#endif