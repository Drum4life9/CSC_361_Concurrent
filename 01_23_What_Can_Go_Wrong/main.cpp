#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

/* Question 1
 *	A deadlock occurs when there are two process that are each waiting on the other
 *		one to release a resource that they need. There are (according to GeeksForGeeks)
 *		4 things that need to be true simultaneously for a deadlock to occur:
 *
 *	1) Mutual exclusion is in use: the resources are not sharable
 *	2) Hold & Wait: the process holds one resource and is waiting for another resource
 *		held by another process
 *	3) No Preemption: The process holding the resource must release it itself, it
 *		cannot be "taken" by another process
 *	4) Circular wait: There must be a cycle in the graph of dependencies and resources
 *
 *	Below is an example of a function that spawns 2 threads that enter a deadlock.
 *	We have all these necessary pieces for a deadlock, especially the cyclical wait
 *	for the opposing thread to finish with work.
 */

// --------------------------------DEADLOCKING------------------------------------

mutex resource1;
mutex resource2;

void threadFunction1() {
	lock_guard lock1(resource1);
	this_thread::sleep_for(chrono::milliseconds(100)); // Simulating work
	lock_guard lock2(resource2); // This will wait for resource2

	cout << "Thread 1 has both mutexes" << endl;
}

void threadFunction2() {
	lock_guard lock2(resource2);
	this_thread::sleep_for(chrono::milliseconds(100)); // Simulating work
	lock_guard lock1(resource1); // This will wait for resource1

	cout << "Thread 2 has both mutexes" << endl;
}

void enterDeadlock() {
	thread t1(threadFunction1);
	thread t2(threadFunction2);

	t1.join();
	t2.join();
}

// ----------------------------------------------------------------------

void dotProductThread(int index, int start, int end, const vector<double>& v1, const vector<double>& v2, vector<double>& results) {
	double sum = 0;
	for (int i = start; i < end; ++i) {
		sum += v1[i] * v2[i];
	}
	results[index] = sum;
}

vector<double> generate_random_list(const int size) {
		vector<double> list;

		default_random_engine rand_engine;
		uniform_real_distribution dist(-10.0, 10.0);

		for (int i = 0; i < size; ++i)
			list.push_back(dist(rand_engine));

		return list;
	}


int main() {
	vector<double> v1 = generate_random_list(1000000);
	vector<double> v2 = generate_random_list(1000000);
	vector<double> results;
	results.reserve(1);
	results.push_back(0);
	//Synchronously
	auto start = high_resolution_clock::now();

	dotProductThread(0, 0, v1.size(), v1, v2, results);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Dot product: " << results[0] << endl;
	cout << "Duration of synchronous execution: " << endl << duration << endl;

	//Async threads
	const int M = 5;
	const int size = v1.size();
	results.clear();
	results.reserve(M);

	start = high_resolution_clock::now();

	for (int m = 0; m < M; ++m) {
		results.push_back(0.0);
		int startIndex = m * (size / M);
		int endIndex = startIndex + (size / M);
		jthread thread1(dotProductThread, m, startIndex, endIndex,
			std::ref(v1), std::ref(v2), std::ref(results));
	}

	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);

	double dot = 0;
	for (const double d : results) {
		dot += d;
	}
	cout << "--------" << endl;
	cout << "Dot product: " << dot << endl;
	cout << "Duration of asynchronous threaded execution: " << endl << duration << endl;

	return 0;
}
