#include "storage.h"

Storage::Storage()
{
    nvs_open("matches", NVS_READWRITE, &__st_handle);
    // nvs_
};

Storage::~Storage()
{
    nvs_close(__st_handle);
};

void Storage::save(const char * key, matchrecord * mr) 
{
    char * data = new char;
    size_t size = sizeof(matchrecord);
    memcpy(data, mr, size); 
    nvs_set_blob(__st_handle, key, data, size);
};

void Storage::clear()
{
    return;
};

matchrecord Storage::read(const char * key)
{
    char * data;
    matchrecord mr;
    size_t size = sizeof(matchrecord);
    nvs_get_blob(__st_handle, key, data, &size);
    memcpy(&mr, data, size);
    return mr;
};

int Storage::size()
{
    return 0;
};


