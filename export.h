#ifndef EXPORT_H
#define EXPORT_H

#include "block.h"
#include <vector>
#include <string>

// Export the blockchain to a CSV file
void exportToCSV(const std::vector<Block>& blockchain, const std::string& filename);

#endif

