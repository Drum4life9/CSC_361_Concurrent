#include <functional>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

int tileSize = 5;
vector<int> r;
vector<int> scans;

int split(int m) {
	return 1 << static_cast<int>(std::log2(m - 1));
}

double tileReduce(int start, const vector<int> &data) {
	int end = (start + tileSize <= data.size()) ? start+tileSize : data.size();
	double num = 0.0;
#pragma omp parallel for reduction(+:num)
	for(int i = start; i < end; ++i) {
		num += data[i];
	}
	return num;
}

void tileScan(int start, double initial, const vector<int> &data) {
	int end = (start + tileSize <= data.size()) ? start+tileSize : data.size();
	scans[start] = scans[start] + initial;
	for(int i = start + 1; i < end; ++i) {
		scans[i] = scans[i] + scans[i - 1];
	}
}

void upSweep(int start, int numTiles) {
	if(numTiles == 1) {
		//base case; compute a reduction over the ith tile
		r[start] = tileReduce(start*tileSize, scans);
	}
	else {
		int k = split(numTiles);

		upSweep(start, k);
		upSweep(start+k, numTiles-k);


		if(numTiles == 2*k) {
			r[start + numTiles - 1] = r[start + k-1] + r[start+numTiles-1];
		}
	}
}

void downSweep(int start, int numTiles, double initial) {
	if(numTiles == 1) {
		//base case; compute the scan over the ith tile
		tileScan(start*tileSize, initial, scans);
	}else {
		int k = split(numTiles);
		downSweep(start, k, initial);
		initial = initial + r[start+k-1];
		downSweep(start+k, numTiles-k, initial);
	}
}



template <typename T>
vector<T> serial_pack(const vector<T>& inp, const function<bool(T)> & func) {
	vector<T> res;

	for (const T& item : inp) {
		if (func(item))
			res.push_back(item);
	}
	return res;
}

template <typename T>
vector<T> parallel_pack(const vector<T>& inp, const function<bool(T)> & func) {
	vector<T> res;
	vector<int> pred = vector<int>(inp.size(), 0);

	int count_elems = 0;

#pragma omp parallel for schedule(static) shared(count_elems)
	for (int i = 0; i < inp.size(); ++i) {
		if (func(inp[i])) {
			pred[i] = 1;
			count_elems++;
		}
	}

	const int m = 1 + (pred.size() - 1)/tileSize;

	r.resize(m, 0);
	scans = pred;  //we could overwrite the original, or make a copy
	upSweep(0,m);
	downSweep(0,m,0);

	res.resize(count_elems);
#pragma omp parallel for schedule(static) shared(res)
	for (int i = 1; i < scans.size(); ++i) {
		if (scans[i] > scans[i - 1]) {
			res[scans[i-1]] = inp[i];
		}
	}

	return pred;
}

int main() {
	int size = 100;
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
	vector<int> res = serial_pack<int>(v1, [] (const int i) { return i % 2 == 0; });
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << "------------------- Duration of synchronous execution: -----------------" << endl;
	cout << duration << endl;

	start = high_resolution_clock::now();
	vector<int> res2 = parallel_pack<int>(v2, [] (const int i) { return i % 2 == 0; });
	stop = high_resolution_clock::now();

	bool same = res == res2;

	duration = duration_cast<microseconds>(stop - start);

	cout << "------------------- Duration of asynchronous execution: -----------------" << endl;
	cout << duration << endl;

	string out = same ? "Yes" : "No";

	cout << "Are results the same: " << out << endl;

	// yeaaaaa so my results aren't great.... not sure what I did, but I
	// think my head's in the right place
}

