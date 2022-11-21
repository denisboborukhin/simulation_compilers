#include "rv32_interpreter.hpp"

using computer::cpu;
using computer::memory;

static bool check_and_execute_shifts (cpu& cpu, const int rd, const int rs1, 
        const int imm, const int funct3);
static int get_bits (const int32_t instruction, const int left, const int right);

void interpret_rv32_bin_code (std::string elf_file_name)
{
    cpu cpu;
    memory memory;

    std::pair<std::pair<uint32_t, uint32_t>, std::vector<char>> bin_code = 
        get_bin_code (elf_file_name);       
    
    memory.load_code (bin_code.first.first, bin_code.second);
    cpu.set_pc (bin_code.first.second);

    cpu.dump_regs ();
    memory.dump_words ();

    std::cout << "asm code: " << std::endl;
    for (;;)
    {
        if (!execute_instruction (cpu, memory))
            break;
    }
    std::cout << "end of asm code" << std::endl;

    cpu.dump_regs ();
    memory.dump_words ();
}

int execute_instruction (cpu& cpu, memory& memory)
{
    uint32_t pc = cpu.get_pc ();

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

            #if 0
            std::cout << "rd: " << rd << "\t ";
            std::cout << "rs1: " << rs1 << "\t ";
            std::cout << "imm: " << imm << std::endl;
            #endif
            
            auto funct3 = get_bits (instruction, 12, 14);
            switch (funct3)
            {   
                case 0b000:                     
                    std::cout << "addi\n";
                    cpu.set_reg (rd, cpu.get_reg (rs1) + imm);
                    break;

               case 0b010:
                    std::cout << "slti\n";
                    if (cpu.get_reg (rs1) < imm)
                        cpu.set_reg (rd, 1);
                    break;
                
                case 0b100:
                    std::cout << "xori\n";
                    cpu.set_reg (rd, cpu.get_reg (rs1) ^ imm);
                    break;

                case 0b110:
                    std::cout << "ori\n";
                    cpu.set_reg (rd, cpu.get_reg (rs1) | imm);
                    break;

                case 0b111:
                    std::cout << "andri\n";
                    cpu.set_reg (rd, cpu.get_reg (rs1) & imm);
                    break;

                default:
                    check_and_execute_shifts (cpu, rd, rs1, imm, funct3);
                    break;
            }

            break;
        }

        case 0b0100011:
        {
            int rs1 = get_bits (instruction, 15, 19);
            int rs2 = get_bits (instruction, 20, 24);
            int imm = (get_bits (instruction, 25, 31) << 5) + get_bits (instruction, 7, 11);
        
            #if 0
            std::cout << "rs1: " << rs1 << "\t ";
            std::cout << "rs2: " << rs2 << "\t ";
            std::cout << "imm: " << imm << std::endl;
            #endif

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
            int imm = ((get_bits(instruction, 31, 31) << 19) + (get_bits(instruction, 12, 19) << 11) +
                (get_bits(instruction, 20, 20) << 10) + get_bits(instruction, 21, 30)) << 1;

            cpu.set_reg (rd, pc + 4);
            cpu.set_pc (imm + pc);

            return 1;
        }
        
        case 0b1100111:
        {
            std::cout << "jalr\n";

            int rd = get_bits (instruction, 7, 11);
            int rs1 = get_bits (instruction, 15, 19);
            int imm = get_bits(instruction, 20, 31); 

            #if 0
            std::cout << "rd: " << rd << "\t ";
            std::cout << "rs1: " << rs1 << "\t ";
            std::cout << "imm: " << imm << std::endl;
            #endif

            auto funct3 = get_bits (instruction, 12, 14);
            if (funct3 == 0b000)
            {
                cpu.set_reg (rd, pc + 4);
                cpu.set_pc (imm + cpu.get_reg(rs1));
            }
            else
            {
                std::cout << "Unknown funct3 for jalr\n";
                exit (0);
            }

            return 1;
        }

        default:
        {
            std::cout << "Unknown code of operation. PC: " << std::hex << pc << std::endl;
            break;
        }
    }
   
    cpu.set_pc (pc + 4);
 
    return 1;
}

static bool check_and_execute_shifts (cpu& cpu, const int rd, const int rs1, 
        const int imm, const int funct3)
{
    auto funct7 = get_bits (imm, 5, 11);
    int new_imm = static_cast<unsigned> (get_bits (imm, 0, 4));
                    
    if (funct7 == 0b0000000)
    {
        if (funct3 == 0b001)
        {
            std::cout << "slli\n";
            cpu.set_reg (rd, cpu.get_reg (rs1) << new_imm);

            return true;
        }
        else if (funct3 == 0b101)
        {
            std::cout << "slri\n";
            cpu.set_reg (rd, cpu.get_reg (rs1) >> new_imm);

            return true;
        }
    }
    else if (funct7 == 0b0100000)
    {
        if (funct3 == 0b101)
        {
            std::cout << "srai\n";

            return true;
        }
    }
    
    std::cout << "Incorrect funct3 for 0b0010011 operation\n";
    return false;
}

static int get_bits (const int32_t instruction, const int young, const int old)
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
