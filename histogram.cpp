#include "histogram.h"
#include <string.h>

Histogram::Histogram(vector<double> data, int columns_count, string _name) {
	name = _name;
	double min = data[0];
	double max = data[0];

	for (int i = 0; i < data.size(); ++i) {
		min = min > data[i] ? data[i] : min;
		max = max < data[i] ? data[i] : max;
	}

	column_width = ((double) max - min) / columns_count + 0.0001;

	for (int i = 0; i < columns_count; ++i) {
		columns.push_back(0);
	}

	for (int i = 0; i < data.size(); ++i) {
		double dInd = (data[i] - min) / column_width;
		columns[(int) dInd]++;
	}

	column_max = columns[0];
	for (int i = 0; i < columns_count; ++i) {
		column_max = column_max < columns[i] ? columns[i] : column_max;
	}

	start_point = min;
	final_point = max;
}

vector<int> Histogram::get_columns() {
	return columns;
}

int Histogram::get_column_max() {
	return column_max;
}

double Histogram::get_column_width() {
	return column_width;
}

double Histogram::get_start_point() {
	return start_point;
}

string Histogram::get_name() {
	return name;
}

double Histogram::get_final_point() {
	return final_point;
}