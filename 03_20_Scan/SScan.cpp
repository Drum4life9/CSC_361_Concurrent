//
// Created by blanda on 3/17/2025.
//

#include "SScan.h"
#include <cmath>

using namespace std;

std::vector<double> SScan::getScans() {
    return scans;
}

std::vector<double> SScan::plainScan() const {
    std::vector<double> ss(data);
    for(int i = 1; i < data.size(); ++i) {
        ss[i] = slow(ss[i]) + ss[i-1];
    }

    return ss;
}

void SScan::recursiveScan() {
    //this is the number of tiles; the last tile might be short
    const int m = 1 + (data.size() - 1)/tileSize;

    r.resize(m, 0);
    scans = data;  //we could overwrite the original, or make a copy
    upSweep(0,m);
    downSweep(0,m,0);
}

void SScan::upSweep(int start, int numTiles) {
    if(numTiles == 1) {
        //base case; compute a reduction over the ith tile
        r[start] = tileReduce(start*tileSize);
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

void SScan::downSweep(int start, int numTiles, double initial) {
    if(numTiles == 1) {
        //base case; compute the scan over the ith tile
        tileScan(start*tileSize, initial);
    }else {
        int k = split(numTiles);
        downSweep(start, k, initial);
        initial = initial + r[start+k-1];
        downSweep(start+k, numTiles-k, initial);
    }
}

int SScan::split(int m) {
    return 1 << static_cast<int>(std::log2(m - 1));
}

double SScan::tileReduce(int start) {
    int end = (start + tileSize <= data.size()) ? start+tileSize : data.size();
    double num = 0.0;
#pragma omp parallel for reduction(+:num)
    for(int i = start; i < end; ++i) {
        num += slow(data[i]);
    }
    return num;
}

void SScan::tileScan(int start, double initial) {
    int end = (start + tileSize <= data.size()) ? start+tileSize : data.size();
    scans[start] = slow(scans[start]) + initial;
    for(int i = start + 1; i < end; ++i) {
        scans[i] = slow(scans[i]) + scans[i - 1];
    }
}


//artificially increase the computational intensity
double SScan::slow(double d) {
    double d2 = d;
    while(d2 > 0) {
        d2 -= .00001;
    }
    return d;
}

std::vector<double> SScan::myParallelScan(std::vector<double> v, int numCuts) {
    vector<double> ret(v.size());
    int numElemsPerCut = static_cast<int>(v.size())/numCuts;


    for (int c = 0; c < numCuts; ++c) {
        for (int i = 0; i < numElemsPerCut; ++i) {
            if (i == 0) {
                ret[numCuts * c + i] = v[numCuts * c + i];
            }

        }
    }
}