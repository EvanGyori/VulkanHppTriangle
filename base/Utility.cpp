#include "Config.h"
#include "Utility.h"

#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>

bool isSubset(
    const std::vector<const char*> subset,
    const std::vector<const char*> superset)
{
    for (const char* a : subset) {
	bool isInSuperset = false;
	for (const char* b : superset) {
	    if (strcmp(a, b) == 0) {
		isInSuperset = true;
		break;
	    }
	}

	if (!isInSuperset) {
	    return false;
	}
    }

    return true;
}

std::vector<char> loadFile(const char* filename)
{
    std::ifstream file(filename, std::ios_base::in | std::ios_base::ate | std::ios_base::binary);
    if (!file.is_open()) {
	std::string errorMessage = "Failed to open file: ";
	errorMessage += filename;
	throw std::runtime_error(errorMessage);
    }

    size_t size = file.tellg();
    file.seekg(0);
    std::vector<char> buffer(size);
    file.read(&buffer[0], size);
    
    return buffer;
}
