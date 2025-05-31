#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>

using namespace std;

struct Block {
    string hash;
    string height;
    string total;
    string time;
    string received_time;
    string relayed_by;
    string prev_block;
};

// Loads blocks from the given text file into a vector
vector<Block> readBlocksFromFile(const string& filename);

#endif

