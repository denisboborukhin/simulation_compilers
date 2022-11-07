#include "elf_rv32_parser.hpp"
#include "computer.hpp"

#include <iostream>
#include <vector>

void interpret_rv32_bin_code (std::string elf_file_name);
int execute_instruction (cpu& cpu, memory& memory);

int get_bits (int32_t instruction, int left, int right);
