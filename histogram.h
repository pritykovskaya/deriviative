#pragma once

#include <vector>
#include <string>

using namespace std;

class Histogram {
private:
	vector<int> columns;
	int column_max;
	double column_width;
	double start_point;
	double final_point;
	string name;
public:
	Histogram(vector<double> data, int columns_count, string _name);
	vector<int> get_columns();
	int get_column_max();
	double get_column_width();
	double get_start_point();
	double get_final_point();
	string get_name();
};