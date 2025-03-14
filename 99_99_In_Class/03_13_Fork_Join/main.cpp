#include <functional>

#include "omp.h"
#include <iostream>
#include <vector>

using namespace std;

int add_five(string s) {
	return s.length();
}

template <typename I, typename O>
vector<O> map_parallel(vector<I>& arr, function<O(I)> const& f) {
	vector<O> ret;
	ret.reserve(arr.size());
#pragma omp parallel for shared(arr, f, ret)
		for (int i = 0; i < arr.size(); ++i) {
			printf("\ti = %d on thread %d\n", i, omp_get_thread_num());
			ret[i] = f(arr[i]);
		}
	return ret;
}

void nested_loop(vector<vector<int>>& arr) {
#pragma omp parallel for collapse(2) shared(arr)
	for (int i = 0; i < arr.size(); ++i) {
		for (int j = 0; j < arr.size(); ++j) {
			printf("\ti = %d and j = %d on thread %d\n", i, j, omp_get_thread_num());
			arr[i][j] = arr[i][j] + 10;
		}
	}
}

int main() {
// 	vector<string> arr;
// 	int size = 20;
// 	arr.reserve(size);
// for (int i = 0; i < size; ++i) {
// 		arr.push_back((std::string(i, '*')));
// 	}
//
// 	vector<int> vec = map_parallel<string, int>(arr, add_five);

	// cout << "--------" << endl;
	//
	// cout << "after map: ";
	// for (int i = 0; i < arr.size(); ++i) {
	// 	cout << vec[i] << " " << endl;
	// }

	int size2 = 5;

	vector<vector<int>> image;
	for (int i = 0; i < size2; ++i) {
		vector<int> vec;
		vec.reserve(size2);
		for (int j = 0; j < size2; ++j) {
			vec.push_back(i + j);
		}
		image.push_back(vec);
	}

	nested_loop(image);

	for (int i = 0; i < size2; ++i) {
		for (int j = 0; j < size2; ++j) {
			cout << image[i][j] << " ";
		}
		cout << endl;
	}
}