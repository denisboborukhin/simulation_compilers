#include "rv32_interpret.hpp"

#include <iostream>

#include <elfio/elfio.hpp>

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
    for (int i = 0; i < sec_num; ++i) {
        section *psec = reader.sections[i];
        std::cout << "  [" << i << "] " << psec->get_name() << "\t" << psec->get_size() << std::endl;
        // Access to section's data
        // const char* p = reader.sections[i]->get_data()
    }

    // Print ELF file segments info
    Elf_Half seg_num = reader.segments.size();
    std::cout << "Number of segments: " << seg_num << std::endl;
    for (int i = 0; i < seg_num; ++i) {
        const segment *pseg = reader.segments[i];
        std::cout << "  [" << i << "] 0x" << std::hex << pseg->get_flags() << "\t0x" << pseg->get_virtual_address()
                  << "\t0x" << pseg->get_file_size() << "\t0x" << pseg->get_memory_size() << std::endl;
        // Access to segments's data
        const char* p = reader.segments[i]->get_data();

        //std::cout << "read!!:: " << p << std::endl;
    }
    
    for (int i = 0; i < sec_num; ++i) {
        section *psec = reader.sections[i];
        // Check section type
        if (psec->get_type() == SHT_SYMTAB) {
            const symbol_section_accessor symbols(reader, psec);
            for (unsigned int j = 0; j < symbols.get_symbols_num(); ++j) {
                std::string name;
                Elf64_Addr value;
                Elf_Xword size;
                unsigned char bind;
                unsigned char type;
                Elf_Half section_index;
                unsigned char other;

                // Read symbol properties
                symbols.get_symbol(j, name, value, size, bind, type, section_index, other);
                std::cout << j << " " << name << " " << value << std::endl;
            }
        }
    }
    
    return 0;
}

/*
int fd;
Elf *e;
char *name, *p, pc[4 * sizeof(char)];
Elf_Scn *scn;
Elf_Data *data;
GElf_Shdr shdr;
size_t n, shstrndx, sz;

if (argc != 2)
    errx(EX_USAGE, "usage: %s file-name", argv[0]);

if (elf_version (EV_CURRENT) == EV_NONE)
    errx (EX_SOFTWARE , "ELF library initialization failed : %s", elf_errmsg (-1));

if ((fd = open(argv[1], O_RDONLY, 0)) < 0)
    err(EX_NOINPUT, "open \"%s\" failed", argv[1]);

if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
    errx(EX_SOFTWARE, "elf_begin() failed: %s.", elf_errmsg(-1));

if (elf_kind(e) != ELF_K_ELF)
    errx(EX_DATAERR, "%s is not an ELF object.", argv[1]);

if (elf_getshdrstrndx(e, &shstrndx) != 0)
    errx(EX_SOFTWARE, "elf_getshdrstrndx() failed: %s.", elf_errmsg(-1));

scn = elf_nextscn(e,scn);
gelf_getshdr(scn, &shdr);
name = elf_strptr(e, shstrndx, shdr.sh_name);
printf("!!Section %-4.4jd %s \n", (uintmax_t)elf_ndxscn(scn), name);

Elf_Cmd cmd = elf_next(e);
cmd = elf_next(e);

printf ("cmd: %d\n", cmd);

while ((scn = elf_nextscn(e, scn)) != NULL)
{
    if (gelf_getshdr(scn, &shdr) != &shdr)
        errx(EX_SOFTWARE, "getshdr() failed: %s.", elf_errmsg(-1));

    if ((name = elf_strptr(e, shstrndx, shdr.sh_name)) == NULL)
        errx(EX_SOFTWARE, "elf_strptr() failed: %s.", elf_errmsg(-1));

    printf("Section %-4.4jd %s \n", (uintmax_t)elf_ndxscn(scn), name);
}

if ((scn = elf_getscn(e, shstrndx)) == NULL)
    errx(EX_SOFTWARE, "getscn() failed: %s.", elf_errmsg(-1));
if (gelf_getshdr(scn, &shdr) != &shdr)
    errx(EX_SOFTWARE, "getshdr(shstrndx) failed: %s.", elf_errmsg(-1));

printf(".shstrab: size=%jd\n", (uintmax_t)shdr.sh_size);
data = NULL;
n = 0;

putchar ('\n');
elf_end (e);
close (fd);
exit (EX_OK);
*/
