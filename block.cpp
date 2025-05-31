#include "../include/block.h"
#include <fstream>
#include <iostream>

using namespace std;

vector<Block> readBlocksFromFile(const string& filename) {
    ifstream in(filename);
    if (!in) {
        cerr << "❌ Could not open " << filename << endl;
        exit(1);
    }

    vector<Block> blockchain;
    string line;
    Block current;

    while (getline(in, line)) {
        if (line.find("hash: ") == 0) {
            current.hash = line.substr(6);
        } else if (line.find("height: ") == 0) {
            current.height = line.substr(8);
        } else if (line.find("total: ") == 0) {
            current.total = line.substr(7);
        } else if (line.find("time: ") == 0) {
            current.time = line.substr(6);
        } else if (line.find("received_time: ") == 0) {
            current.received_time = line.substr(16);
        } else if (line.find("relayed_by: ") == 0) {
            current.relayed_by = line.substr(12);
        } else if (line.find("prev_block: ") == 0) {
            current.prev_block = line.substr(13);
        } else if (line.find("}") != string::npos) {
            blockchain.push_back(current);
            current = Block(); // Reset for next block
        }
    }

    in.close();
    return blockchain;
}

