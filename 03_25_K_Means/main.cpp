#include <iostream>
#include <vector>
#include <random>
#include <map>


using namespace std;

map<double, vector<int>> get_K_Means(const vector<int>& data, int K) {
	vector<int> K_means_assign;
	vector<double> centroids;
	map<int, vector<int>> ret;
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(0, K - 1);
	bool same = false;

	centroids = vector<double>(K, 0);

	for (int i = 0; i < data.size(); i++) {
		K_means_assign.push_back(distrib(gen));
	}

	map<int, vector<int>> new_map = {};

	//build up blank map
	for (int i = 0; i < K; i++) {
		new_map[i] = vector<int>();
	}

	//loop through items, add to map based on centroid
	for (int i = 0; i < data.size(); i++) {
		new_map[K_means_assign[i]].push_back(data[i]);
	}

	while (!same) {

		//parallelizeable part: compute centroids
		//stored values in map, not indexes
		for (int i = 0; i < K; i++) {
			const vector<int>& lis = new_map[i];
			int sum = 0;
			for (const int li : lis) {
				sum += li;
			}
			double avg = sum / lis.size();
			centroids[i] = avg;
		}

		//reassign maps
		ret = new_map;
		for (int i = 0; i < K; ++i) {
			new_map[i].clear();
		}



		//reassign centroids
		for (int i : data) {
			int nearest_centroid = -1;
			double nearest_dist = std::numeric_limits<double>::max();
			for (int cent = 0; cent < centroids.size(); ++cent) {
				if (abs(centroids[cent] - i) < nearest_dist) {
					nearest_centroid = cent;
					nearest_dist = abs(centroids[cent] - i);
				}
			}
			new_map[nearest_centroid].push_back(i);
		}

		if (ret == new_map) {
			same = true;
		}

	}

	map<double, vector<int>> final_ret = {};

	for (int i = 0; i < K; ++i) {
		final_ret[centroids[i]] = ret[i];
	}

	return final_ret;
}

int main() {
	vector<int> v = {1, 8, 23, 2, 76, 14, -1, 100, 15};

	int K = 4;
	map<double, vector<int>> K_means = get_K_Means(v, K);

	for (const auto& pair : K_means) {
		cout << "Centroid: " << std::to_string(pair.first) << endl;
		cout << "Values: " << endl;
		string out = "\t";
		for (int j : pair.second) {
			out += std::to_string(j) + ", ";
		}
		cout << out << endl;
	}
}