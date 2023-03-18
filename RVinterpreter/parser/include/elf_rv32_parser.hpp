#include <iostream>
#include <vector>
#include <elfio/elfio.hpp>
#include <iomanip>

std::pair<std::pair<uint32_t, uint32_t>, std::vector<char>> get_bin_code (std::string &file_name);
const uint32_t get_point_of_entry ();
