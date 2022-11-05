#include "elf_rv32_parser.hpp"

#include <iostream>
#include <vector>

int interpret_rv32_bin_code (std::vector<int32_t> bin_code);

class cpu final
{
    static constexpr int NUM_REGS = 32;
    int32_t regfile [NUM_REGS];
    int pc = 0;

public:
    int get_pc ();
    int set_pc (int val);
    int32_t get_reg (int num_reg);
    int32_t set_reg (int num_reg, int32_t val_reg);
};
