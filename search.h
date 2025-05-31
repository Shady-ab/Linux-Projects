#ifndef SEARCH_H
#define SEARCH_H

#include "block.h"
#include <vector>
#include <string>

// Search and print a block by hash
void searchByHash(const std::vector<Block>& blockchain, const std::string& hash);

// Search and print a block by height
void searchByHeight(const std::vector<Block>& blockchain, const std::string& height);

#endif

