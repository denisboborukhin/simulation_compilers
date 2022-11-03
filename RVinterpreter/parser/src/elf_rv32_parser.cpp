#include "elf_rv32_parser.hpp"

std::vector<int32_t> get_bin_code (std::string file_name)
{
    std::vector<int32_t> bin_code;

    // Create an elfio reader
    ELFIO::elfio reader;

    std::ofstream output ("../dump/parser_dump.txt");

    // Load ELF data
    if (!reader.load(file_name.c_str ())) {
        output << "Can't find or process ELF file " << file_name << std::endl;
        return bin_code;
    }

    // Print ELF file properties
    output << "ELF file class: ";
    if (reader.get_class() == ELFIO::ELFCLASS32)
        output << "ELF32" << std::endl;
    else
        output << "ELF64" << std::endl;

    output << "ELF file encoding: ";
    if (reader.get_encoding() == ELFIO::ELFDATA2LSB)
        output << "Little endian" << std::endl;
    else
        output << "Big endian" << std::endl;

    // Print ELF file sections info
    ELFIO::Elf_Half sec_num = reader.sections.size();
    output << "Number of sections: " << sec_num << std::endl;

    for (int i = 0; i < sec_num; ++i) 
    {
        ELFIO::section *psec = reader.sections[i];
        
        output << "  [" << i << "] " << psec->get_name() << "\t" << psec->get_size() << std::endl;
        // Access to section's data
        if (psec->get_name () == ".text")
        {
            int32_t* data = (int32_t *) reader.sections[i]->get_data();
            int size = reader.sections[i]->get_size ();
            
            output << "Binary code from ./text:\n";
            for (int count = 0; count != size; count += sizeof (int32_t))
            {
                int32_t elem = *(int32_t*) data;
                output << std::setw(8) << std::setfill('0') << std::hex << elem << std::endl;
                
                bin_code.insert (bin_code.end(), elem);
                data++;
            }

            break;
        }
    }

    output << "Success parsing\n";
    return bin_code;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: ./elf_rv32_parser <elf_file>" << std::endl;
        return 1;
    }

    std::vector<int32_t> bin_code = get_bin_code (argv[1]);        

    return 0;
}

