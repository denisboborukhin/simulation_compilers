#include "rv32_interpret.hpp"

#include <iostream>

using namespace ELFIO;

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: tutorial <elf_file>" << std::endl;
        return 1;
    }

    // Create an elfio reader
    elfio reader;

    // Load ELF data
    if (!reader.load(argv[1])) {
        std::cout << "Can't find or process ELF file " << argv[1] << std::endl;
        return 2;
    }

    // Print ELF file properties
    std::cout << "ELF file class    : ";
    if (reader.get_class() == ELFCLASS32)
        std::cout << "ELF32" << std::endl;
    else
        std::cout << "ELF64" << std::endl;

    std::cout << "ELF file encoding : ";
    if (reader.get_encoding() == ELFDATA2LSB)
        std::cout << "Little endian" << std::endl;
    else
        std::cout << "Big endian" << std::endl;

    // Print ELF file sections info
    Elf_Half sec_num = reader.sections.size();
    std::cout << "Number of sections: " << sec_num << std::endl;

    std::vector<int32_t> bin_code;
    for (int i = 0; i < sec_num; ++i) {
        section *psec = reader.sections[i];
        std::cout << "  [" << i << "] " << psec->get_name() << "\t" << psec->get_size() << std::endl;
        // Access to section's data
        if (psec->get_name () == ".text")
        {
            int32_t* data = (int32_t *) reader.sections[i]->get_data();
            int size = reader.sections[i]->get_size ();
            for (int count = 0; count != size; count += sizeof (int32_t))
            {
                int32_t elem = *(int32_t*) data;
                printf ("%x\n", elem);
                //bin_code.insert (elem);

                data++;
            }

            break;
        }
    }

    return 0;
}

