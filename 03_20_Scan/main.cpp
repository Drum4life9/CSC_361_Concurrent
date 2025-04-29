#include <iostream>
#include <vector>
#include "SScan.h"

using namespace std;

int main() {
  vector<double> v;

  v = vector<double>{1,2,3,4,5,6,7,8,9};

  SScan scan = SScan(v, 3);

  vector<double> v2 = scan.plainScan();
  for (double x : v2) {
    cout << x << endl;
  }
  cout << "-----------------" << endl;
  scan.recursiveScan();
  vector<double> v3 = scan.getScans();
  for (const double x : v3) {
    cout << x << endl;
  }
}