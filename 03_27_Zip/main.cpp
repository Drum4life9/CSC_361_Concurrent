#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

template <typename T>
vector<T> zip_serial(const vector<T>& v1, const vector<T>& v2) {
	vector<T> res;
	res.resize(v1.size() + v2.size());
	int idx = 0;
	while (idx < res.size()) {
		res[idx] = v1[idx/2];
		idx++;
		res[idx] = v2[idx/2];
		idx++;
	}

	return res;
}


template <typename T>
vector<T> zip_parallel(const vector<T>& v1, const vector<T>& v2) {
	vector<T> res;
	res.resize(v1.size() + v2.size());
	int minSize = min(v1.size(), v2.size());
	bool is_V1_smaller = v1.size() < v2.size();

#pragma omp parallel for
	for (int i = 0; i < minSize * 2; i += 2) {
		res[i] = v1[i/2];
	}
#pragma omp parallel for
	for (int j = 1; j < minSize * 2; j += 2) {
		res[j] = v2[j/2];
	}
#pragma omp parallel for
	for (int i = minSize * 2; i < res.size(); ++i) {
		res[i] = is_V1_smaller ? v2[i - minSize] : v1[i - minSize];
	}

	return res;
}

int main() {
int size = 10000000;
	vector<int> v(size);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, 100);


	vector<int> v1 = {};
	vector<int> v2 = {};
	for (int i = 0; i < size; i++) {
		v1.push_back(distrib(gen));
		v2.push_back(distrib(gen));
	}

    auto start = high_resolution_clock::now();
	//Sequential scan of files
	vector<int> v3 = zip_serial<int>(v1, v2);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "------------------- Duration of synchronous execution: -----------------" << endl;
	cout << duration << endl;

	start = high_resolution_clock::now();
	vector<int> v4 = zip_parallel<int>(v1, v2);
	stop = high_resolution_clock::now();

	bool same = v3 == v4;

	duration = duration_cast<microseconds>(stop - start);

	cout << "------------------- Duration of asynchronous execution: -----------------" << endl;
	cout << duration << endl;

	string out = same ? "Yes" : "No";

	cout << "Are results the same: " << out << endl;


	// Just need to throw a bunch more numbers at it to get a speedup!
	// Small lists perform significantly worse lol
};