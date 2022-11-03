#include <iostream>

#include <elfio/elfio.hpp>
#include <string.h>
#include <vector>
/*
struct bin_code final
{
    int32_t* data = nullptr;
    int32_t pc = 0;

    bin_code ();
    bin_code (const int32_t* reader_data, int size)
    {
        this->get_data (reader_data, size);
    }

    void get_data (const int32_t* reader_data, int size)
    {
        data = new int32_t [size];
        memcpy (data, reader_data, size); 
    }

    ~bin_code ()
    {
        delete [] data;
    }
};*/
