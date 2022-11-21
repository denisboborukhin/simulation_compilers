#include "elf_rv32_parser.hpp"

std::pair<std::pair<uint32_t, uint32_t>, std::vector<char>> get_bin_code (std::string file_name)
{
    uint32_t address = 0; 
    uint32_t entry_point = 0;
    std::vector<char> instructions;

    // Create an elfio reader
    ELFIO::elfio reader;

    std::ofstream output ("../dump/pars_info.txt");

    // Load ELF data
    if (!reader.load(file_name.c_str ())) {
        std::cout << "Can't find or process ELF file " << file_name << std::endl;
        return {{address, entry_point}, instructions};
    }

    // Print ELF file properties
    output << "ELF file class: ";
    if (reader.get_class() != ELFIO::ELFCLASS32)
    {
        std::cout << "Error ELF file class: ELF64, but expected ELF32" << std::endl;
        exit (1);
    }
    else
        output << "ELF32" << std::endl;

    output << "ELF file encoding: ";
    if (reader.get_encoding() != ELFIO::ELFDATA2LSB)
    {
        std::cout << "Error ELF file encoding: \"Big endian\", expected \"Little endian\"" << std::endl;
        exit (1);
    }
    else
        output << "Little endian" << std::endl;

    entry_point = reader.get_entry ();

    output << "Entry point: " << std::hex << std::setw(8) << std::setfill ('0') 
        << entry_point << std::endl;

    //read data from segment
    ELFIO::segment *pseg = reader.segments[1];
    ELFIO::Elf_Half seg_num = reader.segments.size();
    char* data  = (char*) pseg->get_data();
    int size = pseg->get_file_size ();

    address = pseg->get_virtual_address ();

    output << "Physical address: " << std::hex << std::setw (8)  << std::setfill ('0')
        << address << std::endl;
    output << "Binary code from ./text:\n";
 
    for (int count = 0; count != size; count++)
    {
        instructions.push_back (*data);
        if (!(count % 4))
        {
            output << '<' << std::hex << std::setw (8)  << std::setfill ('0') << address + count
                << ">:" << '\t' << std::hex << std::setw (8)  << std::setfill ('0') 
                << *reinterpret_cast<uint32_t*> (data) << std::endl;
        }

        data++;
    }            

#if 0
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
            //int32_t* data = (int32_t*) psec->get_data();
            int size = psec->get_size ();
            address = psec->get_address ();

            output << "Physical address: " << std::hex << address << std::endl;
            output << "Binary code from ./text:\n";

            char* data = (char*) psec->get_data ();
            for (int count = 0; count != size; count++)
            {
                instructions.push_back (*data);
                if (!(count % 4))
                {
                    output << '<' << std::hex << std::setw (8)  << std::setfill ('0') << address + count
                        << ">:" << '\t' << std::hex << std::setw (8)  << std::setfill ('0') 
                        << *reinterpret_cast<uint32_t*> (data) << std::endl;
                }

                data++;
            }            

            break;
        }
    }
#endif

    output << "Success parsing\n";
    output.close ();

    return {{address, entry_point}, instructions};
}

