#include "../include/print.h"
#include <iostream>

using namespace std;

void printBlock(const Block& b, bool show_arrow) {
    cout << "hash: " << b.hash << "\n";
    cout << "height: " << b.height << "\n";
    cout << "total: " << b.total << "\n";
    cout << "time: " << b.time << "\n";
    cout << "received_time: " << b.received_time << "\n";
    cout << "relayed_by: " << b.relayed_by << "\n";
    cout << "prev_block: " << b.prev_block << "\n";

    if (show_arrow) {
        cout << "        |\n";
        cout << "        V\n";
    }
}

void printAllBlocks(const vector<Block>& blockchain) {
    for (size_t i = 0; i < blockchain.size(); ++i) {
        printBlock(blockchain[i], i != blockchain.size() - 1);
        cout << "\n";
    }
}

