#include "rv32_interpreter.hpp"

int cpu::get_pc ()
{
    return pc;
}

int cpu::set_pc (int val)
{
    pc = val;

    return pc;
}

int32_t cpu::get_reg (int num_reg)
{
    if (num_reg < NUM_REGS)
        return regfile[num_reg];
    else 
        exit (0);
}

int32_t cpu::set_reg (int num_reg, int32_t val_reg)
{
    if (num_reg < NUM_REGS)
        return regfile[num_reg] = val_reg;
    else 
        exit (0);
}

int interpret_rv32_bin_code (std::vector<int32_t> bin_code)
{
    cpu cpu;

}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: ./elf_rv32_parser <elf_file>" << std::endl;
        return 1;
    }

    std::pair<int64_t, std::vector<int32_t>> bin_code = get_bin_code (argv[1]);        
    int64_t address = bin_code.first;
    std::vector<int32_t> instructions = bin_code.second;

    return 0;
}
