#include <fstream>
#include <random>
#include <vector>
#include <iostream>
#include <thread>

using namespace std;

vector<string> files = {"users.txt", "text1.txt", "text2.txt"};

vector<string> contents;

void serial_concat() {
	contents.resize(files.size());
	for (int i = 0; i < files.size(); ++i) {
		string file_name = files[i];
		file_name = "../" + file_name;
		ifstream ifs(file_name);
		string line;
		string total_file_contents;
		while (getline(ifs, line)) {
			total_file_contents += line;
			total_file_contents += "\n";
		}
		contents[i] = total_file_contents;
	}

	ofstream outFile("../serial_concat.txt");
	for (const auto & content : contents) {
		outFile << content;
	}
	outFile.close();
}

void parallel_concat() {
	contents.resize(files.size());
	string total_file_contents;
	string line;
#pragma omp parallel shared(contents, files) private(total_file_contents, line)
	{
#pragma omp for schedule(static)
		for (int i = 0; i < files.size(); ++i) {
			string file_name = files[i];
			file_name = "../" + file_name;
			ifstream ifs(file_name);
			while (getline(ifs, line)) {
				total_file_contents += line;
				total_file_contents += "\n";
			}

			contents[i] = total_file_contents;
		}

#pragma omp critical
		{
			ofstream outFile("../parallel_concat.txt");
			for (const auto & content : contents)
				outFile << content;
			outFile.close();
		}
	}
}

int min_value_array_serial(const vector<int>& v) {
	int min = INT_MAX;

	for (int i = 0; i < v.size(); ++i) {
		if (v[i] < min) {
			this_thread::sleep_for(1ms);
			min = v[i];
		}
	}

	return min;
}

int min_value_array_parallel(const vector<int>& v) {
	int min = INT_MAX;
#pragma omp parallel for reduction(min:min)
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] < min) {
			this_thread::sleep_for(1ms);
			min = v[i];
		}
	}

	return min;
}



int main() {
	// serial_concat();
	// parallel_concat();"
	int size = 2000;
	vector<int> v(size);
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> distrib(1, 100);
	uniform_int_distribution<> distrib2(0, 1);
	for (int& i : v) {
		i = distrib(gen) * (distrib2(gen) == 0 ? 1 : -1);
	}

	// int min = min_value_array_serial(v);
	// cout << "Min serial is: " << min << endl;

	// int min2 = min_value_array_parallel(v);
	// cout << "Min Parallel is: " << min2 << endl;


}

/*
 * Question 1:
 *	 Yes, it certainly seems like reduce needs to be commutative. If one thread
 *		does a computation that's later in the array of whatever is being worked on
 *		than what the current thread is working on, we would hope that the result is
 *		the same whichever thread is worked on first (say, in our dot product example).
 *		With the dot product, both multiplication and addition are commutative, if they
 *		were not, we would run into several troubles from running the program one time
 *		versus another.
 *
 *		An example of where this would matter is in number 2 for the homework. The
 *		files need to be concatenated in the order that they appear in the list. This
 *		is certainly not a commutative operation, as 1 needs to always come before 2 for
 *		the result to be correct according to the problem definition.
 */