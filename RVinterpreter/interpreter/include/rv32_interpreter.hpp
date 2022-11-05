#include "elf_rv32_parser.hpp"

#include <iostream>
#include <vector>

class cpu final
{
    static constexpr int NUM_REGS = 32;
    int32_t regfile [NUM_REGS] {};
    int pc = 0;

public:
    int get_pc ();
    int next_pc ();
    int set_pc (int val);
    int32_t get_reg (int num_reg);
    int32_t set_reg (int num_reg, int32_t val_reg);
};

void interpret_rv32_bin_code (std::string elf_file_name);
int execute_instruction (cpu& cpu, std::pair<int64_t, std::vector<int32_t>>& bin_code);

int get_bits (int32_t instruction, int left, int right);
