#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>

class cpu final
{
    static constexpr int NUM_REGS = 32;
    int32_t regfile [NUM_REGS] {0, 0, 8196};
    int pc = 0;

public:
    int get_pc () const;
    int next_pc ();
    int set_pc (int val);
    int32_t get_reg (const int num_reg) const;
    int32_t set_reg (const int num_reg, const int32_t val_reg);
};

struct memory final
{
    std::vector<char> mem;

    using memItr = typename std::vector<char>::iterator;
    std::unordered_map<int64_t, memItr> hash_mem;

    bool load_code (const int64_t start_address, const std::vector<char>& bin_code);

    unsigned char get_byte (const int64_t address) const;
    uint16_t get_half (const int64_t address) const;
    uint32_t get_word (const int64_t address) const;

    char set_byte (int64_t address, char byte);
    int16_t set_half (int64_t address, int16_t half);
    int32_t set_word (int64_t address, int32_t word);

    void dump ();
    void dump_mem ();
    uint32_t dump_word (memItr itr);
};

