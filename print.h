#ifndef PRINT_H
#define PRINT_H

#include "block.h"
#include <vector>

// Prints a single block (with arrow if not last)
void printBlock(const Block& b, bool show_arrow);

// Prints all blocks in the chain
void printAllBlocks(const std::vector<Block>& blockchain);

#endif

