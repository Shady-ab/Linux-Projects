#include "include/block.h"
#include "include/export.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<Block> blockchain = readBlocksFromFile("block_data/blocks.txt");

    exportToCSV(blockchain, "a.csv");

    cout << "Exported to a.csv" << endl;
    return 0;
}

