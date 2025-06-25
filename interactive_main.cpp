#include "include/block.h"
#include "include/print.h"
#include "include/search.h"
#include "include/export.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>    // usleep
#include <cstdio>      // FILE*, popen, fgets
#include <string>

using namespace std;

// Run the Bash script and show its output in real-time
bool runScriptWithOutput(const string& scriptCommand) {
    cout << "Running: " << scriptCommand << endl;

    FILE* pipe = popen(scriptCommand.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to start script." << endl;
        return false;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        cout << buffer; // Print script output line by line
    }

    int exitCode = pclose(pipe);
    if (exitCode != 0) {
        cerr << "Script exited with code: " << exitCode << endl;
        return false;
    }

    return true;
}

void showMenu() {
    cout << "Choose an option:\n";
    cout << "1. Print db\n";
    cout << "2. Print block by hash\n";
    cout << "3. Print block by height\n";
    cout << "4. Export data to csv\n";
    cout << "5. Refresh data\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    vector<Block> blockchain = readBlocksFromFile("block_data/blocks.txt");

    int choice;
    string input;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1:
                printAllBlocks(blockchain);
                break;

            case 2:
                cout << "Enter block hash: ";
                getline(cin, input);
                searchByHash(blockchain, input);
                break;

            case 3:
                cout << "Enter block height: ";
                getline(cin, input);
                searchByHeight(blockchain, input);
                break;

            case 4:
                exportToCSV(blockchain, "a.csv");
                cout << "Exported to a.csv" << endl;
                break;

            case 5: {
                int blockCount;
                cout << "Enter number of blocks to fetch: ";
                cin >> blockCount;
                cin.ignore(); // Clear newline

                string command = "./fetch_blocks.sh " + to_string(blockCount);
                if (runScriptWithOutput(command)) {
                    system("sync");
                    usleep(500000);
                    blockchain = readBlocksFromFile("block_data/blocks.txt");
                    cout << "Reloaded " << blockchain.size() << " blocks." << endl;
                } else {
                    cout << "Failed to refresh data." << endl;
                }
                break;
            }

            case 0:
                cout << "Goodbye! and dont use Linux again!" << endl;
                break;

            default:
                cout << "Invalid option." << endl;
        }

    } while (choice != 0);

    return 0;
}

