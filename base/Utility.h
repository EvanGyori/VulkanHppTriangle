#pragma once

#include <vector>

bool isSubset(
    const std::vector<const char*> subset,
    const std::vector<const char*> superset);

std::vector<char> loadFile(const char* filename);
