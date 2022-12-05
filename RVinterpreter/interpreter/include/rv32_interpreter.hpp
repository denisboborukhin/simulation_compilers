#include "elf_rv32_parser.hpp"
#include "computer.hpp"

#include <iostream>
#include <vector>

void interpret_rv32_bin_code (const std::string& elf_file_name);
int execute_instruction (computer::cpu& cpu, computer::memory& memory);
