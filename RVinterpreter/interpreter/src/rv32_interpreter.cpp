#include "rv32_interpreter.hpp"

void interpret_rv32_bin_code (std::string elf_file_name)
{
    cpu cpu;
    memory memory;

    std::pair<int64_t, std::vector<char>> bin_code = get_bin_code (elf_file_name);       
    
    memory.load_code (bin_code.first, bin_code.second);
    cpu.set_pc (bin_code.first);
    
    for (int i = 0; i != 30; ++i)
    {
        if (!execute_instruction (cpu, memory))
            break;
    }
}

int execute_instruction (cpu& cpu, memory& memory)
{
    int64_t pc = cpu.get_pc ();
    //if (pc >= memory.mem.size ())
    //    return 0;

    int32_t instruction = memory.get_word (pc);
    switch (instruction & 0x7F)
    {
        case 0b0110011:
        {
            int rd = get_bits (instruction, 7, 11);
			int rs1 = get_bits (instruction, 15, 19);
			int rs2 = get_bits (instruction, 20, 24);      
           
            auto funct3 = get_bits (instruction, 12, 14);
            switch (funct3)
            {
                case 0b000:
                    std::cout << "add\n";
                    cpu.set_reg (rd, cpu.get_reg (rs1) + cpu.get_reg (rs2));
                    break;

                default:
                    std::cout << "don't known from add\n";
            }

            break;
        }

        case 0b0010011:
        {
            int rd = get_bits (instruction, 7, 11);
			int rs1 = get_bits (instruction, 15, 19);
			int imm = get_bits (instruction, 20, 31);

            auto funct3 = get_bits (instruction, 12, 14);
            switch (funct3)
            {   
                case 0b000:                     //addi
                    std::cout << "addi\n";
                    cpu.set_reg (rd, cpu.get_reg (rs1) + imm);
                    break;

                default:
                    std::cout << "don't known from addi\n";
            }

            break;
        }

        case 0b0100011:
        {
            int rs1 = get_bits (instruction, 15, 19);
            int rs2 = get_bits (instruction, 20, 24);
            int imm = (get_bits (instruction, 25, 31) << 5) + get_bits (instruction, 7, 11);

            auto funct3 = get_bits (instruction, 12, 14);
            switch (funct3)
            {
                case 0b000:
                    std::cout << "sb\n";
                    memory.set_byte (rs1 + imm, rs2);
                    break;

                case 0b001:
                    std::cout << "sh\n";
                    break;

                case 0b010:
                    std::cout << "sw\n";
                    break;
            }

            break;
        }

        default:
        {
            std::cout << memory.mem.size () << std::endl;
            break;
        }
    }
    
    cpu.set_pc (pc + 4);
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
