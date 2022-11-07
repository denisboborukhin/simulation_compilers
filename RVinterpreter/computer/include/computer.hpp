#include <iostream>
#include <vector>
#include <unordered_map>

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

struct memory final
{
    std::vector<char> mem;

    using memItr = typename std::vector<char>::iterator;
    std::unordered_map<int64_t, memItr> hash_mem;

    bool load_code (int64_t address, std::vector<char>& bin_code);

    unsigned char get_byte (int64_t address);
    int32_t get_word (int64_t address);

    char set_byte (int64_t address, char byte);
    int set_half (int64_t address, int half);
};

