#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#include "omp.h"

using namespace std;

vector<tuple<int, int, int>> serial_image(const vector<tuple<int, int, int>>& v) {
	vector<tuple<int, int, int>> adjusted;
	adjusted.resize(v.size());

	for (int i = 0; i < v.size(); i++) {
		int r = get<0>(v[i]);
		int g = get<1>(v[i]);
		int b = get<2>(v[i]);
		double I = sqrt(r * r + g * g + b * b);
		double s = sqrt(I * I / 3);
		int s2 = static_cast<int>(s);
		adjusted[i] = make_tuple(s2, s2, s2);
	}
	return adjusted;
}

vector<tuple<int, int, int>> parallel_image(const vector<tuple<int, int, int>>& v) {
	vector<tuple<int, int, int>> adjusted;
	adjusted.resize(v.size());


#pragma omp parallel
	{
#pragma omp for schedule(static)
	for (int i = 0; i < v.size(); i++) {
		int r = get<0>(v[i]);
		int g = get<1>(v[i]);
		int b = get<2>(v[i]);
		double I = sqrt(r * r + g * g + b * b);
		double s = sqrt(I * I / 3);
		int s2 = static_cast<int>(s);
		adjusted[i] = make_tuple(s2, s2, s2);
	}
}
	return adjusted;
}

int main() {
	vector<tuple<int, int, int>> image;
	image.emplace_back(255, 100, 50);
	image.emplace_back(13, 112, 67);
	image.emplace_back(1, 2, 3);
	image.emplace_back(1, 3, 123);
	image.emplace_back(1, 34, 95);
	image.emplace_back(1, 15, 12);
	image.emplace_back(1, 176, 18);
	image.emplace_back(1, 37, 73);
	image.emplace_back(1, 86, 200);

	const vector<tuple<int, int, int>> image2 = serial_image(image);
	for (const auto & i : image2) {
		cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;
	}

	cout << "--------------" << endl;

	const vector<tuple<int, int, int>> image3 = parallel_image(image);
	for (const auto & i : image3) {
		cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;
	}

}