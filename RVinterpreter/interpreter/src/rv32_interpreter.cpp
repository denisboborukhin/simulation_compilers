#include "rv32_interpreter.hpp"

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: ./elf_rv32_parser <elf_file>" << std::endl;
        return 1;
    }

    std::vector<int32_t> bin_code = get_bin_code (argv[1]);        

    return 0;
}
