#ifndef STORAGELIB
#define STORAGELIB
#include "types.h"
#include "nvs.h"
#include "nvs_flash.h"

class Storage 
{
    public:
        Storage();
        ~Storage();
        void save(const char * key, matchrecord * mr);
        matchrecord * read(const char * key);
        void clear(const char * key);
        void clear_all();
        int size();
    private:
        nvs_handle __st_handle;
};
#endif