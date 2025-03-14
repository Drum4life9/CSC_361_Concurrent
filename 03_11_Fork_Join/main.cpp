#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

vector v = {1, 2, 3, 3, 5, 3, 7, 8, 9, 3};
int length = 0;
int count = 0;

int serial_count3s(const vector<int>& v) {
	int num3 = 0;
	for (const int i : v) {
		if (i == 3)
		num3++;
	}
	return num3;
}

int count3s() {
	int count_th;
	length = v.size();
	count = 0;
	int i;

#pragma omp parallel shared(v, length, count) private(count_th)
	{
		count_th = 0;
#pragma omp parallel for private(i)
		for (i = 0; i < length; ++i) {
			if (v[i] == 3)
				count_th++;
		}

#pragma omp atomic

			count += count_th;

	}
	return count;
}



int main() {
#pragma omp parallel
{
	cout << count3s() << endl;
	// cout << omp_get_num_threads() << endl;
}
	return 0;
}