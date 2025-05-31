#include "../include/search.h"
#include "../include/print.h"
#include <iostream>

using namespace std;

void searchByHash(const vector<Block>& blockchain, const string& hash) {
    for (const auto& b : blockchain) {
        if (b.hash == hash) {
            printBlock(b, false);
            return;
        }
    }
    cout << "Block with hash not found." << endl;
}

void searchByHeight(const vector<Block>& blockchain, const string& height) {
    for (const auto& b : blockchain) {
        if (b.height == height) {
            printBlock(b, false);
            return;
        }
    }
    cout << "Block with height not found." << endl;
}

