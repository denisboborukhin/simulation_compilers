#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>

class cpu final
{
    static constexpr int NUM_REGS = 32;
    static constexpr int XLEN = 32;
    int32_t regfile [NUM_REGS] {0, 0, 8192};
    uint32_t pc = 0;

public:
    uint32_t get_pc () const;
    uint32_t next_pc ();
    uint32_t set_pc (int val);
    
    int32_t get_reg (const int num_reg) const;
    int32_t set_reg (const int num_reg, const int32_t val_reg);

    void dump_regs () const; 
};

struct memory final
{
    std::unordered_map<uint32_t, char> mem;

    bool load_code (const uint32_t start_address, const std::vector<char>& bin_code);

    unsigned char get_byte (const uint32_t address) const;
    uint16_t get_half (const uint32_t address) const;
    uint32_t get_word (const uint32_t address) const;

    char set_byte (uint32_t address, char byte);
    int16_t set_half (uint32_t address, int16_t half);
    int32_t set_word (uint32_t address, int32_t word);

    void dump_words ();
};
