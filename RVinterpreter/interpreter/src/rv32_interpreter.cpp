#include "rv32_interpreter.hpp"

void interpret_rv32_bin_code (std::string elf_file_name)
{
    cpu cpu;
    memory memory;

    std::pair<int64_t, std::vector<char>> bin_code = get_bin_code (elf_file_name);       
    
    memory.load_code (bin_code.first, bin_code.second);
    cpu.set_pc (bin_code.first);

    //memory.dump ();
    for (;;)
    {
        if (!execute_instruction (cpu, memory))
            break;
    }

    //memory.dump ();
}

int execute_instruction (cpu& cpu, memory& memory)
{
    int64_t pc = cpu.get_pc ();

    uint32_t instruction = memory.get_word (pc);
    if (!instruction)
        return 0;

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
                case 0b000:                     
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
                    memory.set_byte (cpu.get_reg (rs1) + imm, cpu.get_reg (rs2));
                    break;

                case 0b001:
                    std::cout << "sh\n";
                    memory.set_half (cpu.get_reg (rs1) + imm, cpu.get_reg (rs2));
                    break;

                case 0b010: 
                    std::cout << "sw\n"; 
                    memory.set_word (cpu.get_reg (rs1) + imm, cpu.get_reg (rs2)); 
                    break;
            }

            break;
        }

        case 0b0000011:
        {
            int rd = get_bits (instruction, 7, 11);
            int rs1 = get_bits (instruction, 15, 19);
            int imm = get_bits (instruction, 20, 31);

            auto funct3 = get_bits (instruction, 12, 14);
            switch (funct3)
            {
                case 0b000:
                    std::cout << "lb\n";
                    cpu.set_reg (rd, memory.get_byte (cpu.get_reg (rs1) + imm));
                    break;

                case 0b001:
                    std::cout << "lh\n";
                    cpu.set_reg (rd, memory.get_half (cpu.get_reg (rs1) + imm));
                    break;

                case 0b010:
                    std::cout << "lw\n";
                    cpu.set_reg (rd, memory.get_word (cpu.get_reg (rs1) + imm));
                    break;
            }

            break;
        }

        case 0b1101111:
        {
            std::cout << "jal\n";

            int rd = get_bits (instruction, 7, 11);
            int imm = (get_bits(instruction, 31, 31) << 19) + (get_bits(instruction, 12, 19) << 11) +
                (get_bits(instruction, 20, 20) << 10) + get_bits(instruction, 21, 30);

            cpu.set_reg (rd, pc + 4);
            cpu.set_pc (imm + pc);

            break;
        }
        
        case 0b1100111:
        {
            std::cout << "jalr\n";

            int rd = get_bits (instruction, 7, 11);
            int rs1 = get_bits (instruction, 15, 19);
            int imm = get_bits(instruction, 20, 31); 

            auto funct3 = get_bits (instruction, 12, 14);
            if (funct3 == 0b000)
            {
                cpu.set_reg (rd, pc + 4);
                cpu.set_pc (imm + rs1);
            }
            else
            {
                std::cout << "Unknown funct3 for jalr\n";
                exit (0);
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
        std::cout << "Usage: ./elf_rv32_interpreter <elf_file>" << std::endl;
        return 1;
    }

    interpret_rv32_bin_code (argv[1]);               

    return 0;
}
