#ifndef SSCAN_H
#define SSCAN_H

#include <vector>


class SScan {
public:
    SScan(const std::vector<double>& data, int tileSize) : data(data), tileSize(tileSize){}

    std::vector<double> getScans();

    void recursiveScan();

    std::vector<double> plainScan() const;

    std::vector<double> myParallelScan(std::vector<double> v, int numCuts);

private:
    void upSweep(int start, int numTiles);
    void downSweep(int start, int numTiles, double initial);
    int split(int m);
    double tileReduce(int start);
    void tileScan(int start, double initial);

    static double slow(double d);


    std::vector<double> data;
    int tileSize;
    std::vector<double> r;
    std::vector<double> scans;

};



#endif //SSCAN_H
