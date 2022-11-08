#include "computer.hpp"

int cpu::get_pc () const
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

int32_t cpu::get_reg (const int num_reg) const
{
    if (num_reg >= NUM_REGS)
    {
        std::cout << "invalid number of register\n";
        exit (0);
    }

    return regfile[num_reg];
}

int32_t cpu::set_reg (const int num_reg, const int32_t val_reg)
{
    if (num_reg >= NUM_REGS)
    {
        std::cout << "invalid number of register\n";
        exit (0);
    }
    
    return regfile[num_reg] = val_reg;
}

bool memory::load_code (const int64_t start_address, const std::vector<char>& bin_code)
{
    mem = bin_code;

    int64_t address = start_address;
       
    for (memItr itr = mem.begin (), end = mem.end (); itr != end; ++itr)  
    {

        hash_mem[address] = itr;
        address++;
    }

    return true;
}

unsigned char memory::get_byte (const int64_t address) const
{
    auto hashItr = hash_mem.find (address);
    if (hashItr != hash_mem.end ())
        return *(hashItr->second);
    else
        return 0;
}

uint16_t memory::get_half (const int64_t address) const
{
    uint16_t half = 0;
    half = get_byte (address + 1);
    half <<= 8;
    half |= get_byte (address);

    return half;
}

uint32_t memory::get_word (const int64_t address) const
{
    uint32_t word = 0;

    word = get_half (address + 2);
    word <<= 16;
    word |= get_half (address);

    return word;
}

char memory::set_byte (int64_t address, char byte)
{
    //std::cout << "set: " << std::hex << address << std::endl;
    auto hashItr = hash_mem.find (address);
    if (hashItr == hash_mem.end ())
    {
        memItr itr = mem.end ();
        mem.insert (itr, byte);
        hash_mem[address] = itr;
    }
    else
    {
        std::cout << "I'am imposter!!!!\n";
        *(hashItr->second) = byte;
    }

    return byte;
}

int16_t memory::set_half (int64_t address, int16_t half)
{   
    set_byte (address, half & 0xFF);
    set_byte (address + 1, (half >> 8) & 0xFF);

    return half;
}

int32_t memory::set_word (int64_t address, int32_t word)
{
    set_half (address, word & 0xFFFF);
    set_half (address + 2, (word >> 16) & 0xFFFF);

    return word;
}

void memory::dump ()
{
    if (hash_mem.size () != mem.size ())
    {
        std::cout << "Memory error\n";
        exit (1);
    }

    std::cout << "memory:\n";
    int count = 0;
    
    for (auto itr = hash_mem.begin (), end = hash_mem.end (); itr != end; ++itr)
    {
        int64_t address = itr->first;
        if (!(itr->first % 4))
        {
            std::cout << std::dec << std::setw(3) << count << ". ";
            std::cout << '<' << std::hex << std::setw (8) << std::setfill ('0') << address
            << ">:\t\"" << std::hex << std::setw (8) << std::setfill ('0') << get_word (address)
            << "\"" << std::endl;

            count++;
        }
    }

    std::cout << "end memory\n";
}

