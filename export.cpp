#include "../include/export.h"
#include <fstream>
#include <iostream>

using namespace std;

void exportToCSV(const vector<Block>& blockchain, const string& filename) {
    ofstream out(filename);
    if (!out) {
        cerr << "Could not open file for writing: " << filename << endl;
        return;
    }

    // Write CSV header
    out << "hash,height,total,time,received_time,relayed_by,prev_block\n";

    for (const auto& b : blockchain) {
        out << b.hash << ','
            << b.height << ','
            << b.total << ','
            << b.time << ','
            << b.received_time << ','
            << b.relayed_by << ','
            << b.prev_block << '\n';
    }

    out.close();
}

