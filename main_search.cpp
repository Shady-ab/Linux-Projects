#include "include/block.h"
#include "include/search.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " --hash <hash> OR --height <height>" << endl;
        return 1;
    }

    string option = argv[1];
    string value = argv[2];

    vector<Block> blockchain = readBlocksFromFile("block_data/blocks.txt");

    if (option == "--hash") {
        searchByHash(blockchain, value);
    } else if (option == "--height") {
        searchByHeight(blockchain, value);
    } else {
        cout << "Invalid option. Use --hash or --height." << endl;
        return 1;
    }

    return 0;
}

