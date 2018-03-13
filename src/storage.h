#ifndef STORAGELIB
#define STORAGELIB
#include "types.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "string.h"

class Storage 
{
    public:
        Storage();
        ~Storage();
        void save(const char * key, matchrecord * );
        matchrecord read(const char * key);
        void clear();
        int size();
    private:
        nvs_handle __st_handle;
};
#endif