#include "include/block.h"
#include "include/print.h"

int main() {
    string filepath = "block_data/blocks.txt";
    vector<Block> blockchain = readBlocksFromFile(filepath);

    printAllBlocks(blockchain);

    return 0;
}

