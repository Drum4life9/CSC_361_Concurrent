#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<int> generate_random_list(const int size) {
	vector<int> list;

	default_random_engine rand_engine;
	uniform_real_distribution dist(-100.0, 100.0);

	for (int i = 0; i < size; ++i)
		list.push_back(dist(rand_engine));

	return list;
}

void dotProduct(const vector<int>& l1, const vector<int>& l2,
	const int start, const int end,
	int& globalSum) {
	for (int i = start; i < end; ++i) {
		globalSum += l1[i] * l2[i];
	}
}

int main() {
    vector<int> v1 = generate_random_list(500000);
	vector<int> v2 = generate_random_list(500000);

	//Synchronously
	auto start = high_resolution_clock::now();

	int globalSum = 0;
	dotProduct(v1, v2, 0, v1.size(), globalSum);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Dot product: " << globalSum << endl;
	cout << "Duration of synchronous execution: " << endl << duration << endl;

	cout << endl;

	//Asynchronously
	start = high_resolution_clock::now();

	globalSum = 0;
	thread t1(dotProduct,v1, v2, 0, 100000, std::ref(globalSum));
	thread t2(dotProduct,v1, v2, 100000, 200000, std::ref(globalSum));
	thread t3(dotProduct,v1, v2, 200000, 300000, std::ref(globalSum));
	thread t4(dotProduct,v1, v2, 300000, 400000, std::ref(globalSum));
	thread t5(dotProduct,v1, v2, 400000, 500000, std::ref(globalSum));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	cout << "Dot product: " << globalSum << endl;
	cout << "Duration of asynchronous execution: " << endl << duration << endl;

	//Well, the result in this case seems to be between 2-3 times slower by
	//using threads. (shrug)

}






/*
 * HW Questions:
 *		1) I assume the "system" is talking about language here, so I'll be
 *	referring to C++ for this question. A list of instructions executed by a
 *	thread in c++ typically takes the form of some sort of function, or small
 *	packaged set of instructions to be followed. I'll be using functions for
 *	the part 4 of this homework
 *		2) Starting a thread in c++ uses the std::thread class. As we'll see,
 *	we write
 *		thread t1(function, arguments);
 *	which creates the thread and runs it. To wait for the thread to finish
 *	executing from another one (in c++), we use t1.join() to make the current
 *	thread wait for the new thread to finish completing.
 *		3) Mutexes can be created and shared among threads, and then locked
 *	from any particular thread that needs access to the shared resource. Once
 *	it is done using the resource, it then is unlocked, allowing other threads
 *	to continue using the shared resource. The mutex object pauses all other
 *	threads until it is unlocked and another thread can continue using and
 *	updating the value
 */