#include "rv32_interpreter.hpp"

int cpu::get_pc ()
{
    return pc;
}

int cpu:: next_pc ()
{
    return ++pc;
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

void interpret_rv32_bin_code (std::string elf_file_name)
{
    cpu cpu;
    std::pair<int64_t, std::vector<int32_t>> bin_code = get_bin_code (elf_file_name);       

    while (true)
    {
        if (!execute_instruction (cpu, bin_code))
            break;
    }
}

int execute_instruction (cpu& cpu, std::pair<int64_t, std::vector<int32_t>>& bin_code)
{
    int pc = cpu.get_pc ();
    if (pc >= bin_code.second.size ())
        return 0;

    int32_t instruction = bin_code.second[pc];
   
    switch (instruction & 0x7F)
    {
        case 0b0110011:
        {
            std::cout << "add!!!\n";
            int rd = get_bits (instruction, 7, 11);
			int rs1 = get_bits (instruction, 15, 19);
			int imm = get_bits (instruction, 20, 24);      
            
            break;
        }

        case 0b0010011:
        {
            std::cout << "addi!!!";
            int rd = get_bits (instruction, 7, 11);
			int rs1 = get_bits (instruction, 15, 19);
			int imm = get_bits (instruction, 20, 31);

            std::cout << "rd: " << rd << std::endl;
            std::cout << "imm: " << imm << std::endl;

            auto funct3 = get_bits (instruction, 12, 14);

            break;
        }

        default:
        {
            std::cout << bin_code.second.size () << std::endl;
            break;
        }
    }
    
    cpu.next_pc ();
    return 1;
}

int get_bits (int32_t instruction, int young, int old)
{
    return static_cast<int>(((instruction << (31 - old)) >> (31 - old)) >> young);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: ./elf_rv32_parser <elf_file>" << std::endl;
        return 1;
    }

    interpret_rv32_bin_code (argv[1]);        

    return 0;
}
