#include "computer.hpp"

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

bool memory::load_code (int64_t address, std::vector<char>& bin_code)
{
    mem = bin_code;

    for (memItr itr = mem.begin (), end = mem.end (); itr != end; ++itr)
    {
        hash_mem[address] = itr;
        address++;
    }

    return true;
}

unsigned char memory::get_byte (int64_t address) 
{
    auto hashItr = hash_mem.find (address);
    if (hashItr != hash_mem.end ())
        return *(hashItr->second);
    else
        return 0;
}

int16_t memory::get_half (int64_t address)
{
    int16_t half = 0;
    half = get_byte (address + 1);
    half <<= 8;
    half |= get_byte (address);

    return half;
}

int32_t memory::get_word (int64_t address)
{
    int32_t word = 0;

    word = get_half (address + 2);
    word <<= 16;
    word |= get_half (address);

    return word;
}

char memory::set_byte (int64_t address, char byte)
{
    auto hashItr = hash_mem.find (address);
    if (hashItr == hash_mem.end ())
    {
        mem.push_back (byte);
        hash_mem[address] = --mem.end();
    }
    else
        *(hashItr->second) = byte;

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
    for (auto itr = hash_mem.begin (), end = hash_mem.end (); itr != end; ++itr)
    {
        std::cout << '\'' << std::hex << itr->first << "\'\t\"" << std::hex << *(itr->second)  <<
            "\"" << std::endl;
    }

    std::cout << "end memory\n";
}

