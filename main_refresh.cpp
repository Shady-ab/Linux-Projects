#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_blocks>\n";
        return 1;
    }

    std::string numBlocks = argv[1];
    std::string command = "bash fetch_blocks.sh " + numBlocks;

    std::cout << "Refreshing data with " << numBlocks << " blocks...\n";

    int result = system(command.c_str());

    if (result == 0) {
        std::cout << "Database reloaded successfully.\n";
    } else {
        std::cerr << "Failed to reload database. Exit code: " << result << std::endl;
    }

    return result;
}

