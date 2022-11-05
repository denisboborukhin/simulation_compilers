#include <iostream>
#include <vector>
#include <elfio/elfio.hpp>
#include <iomanip>

std::pair<int64_t, std::vector<int32_t>> get_bin_code (std::string file_name);
