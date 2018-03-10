#ifndef STORAGELIB
#define STORAGELIB
#include "types.h"

class Storage 
{
    public:
        Storage();
        ~Storage();
        void save(matchrecord);
        void read();
        void clear();
        int size();
    private:
        int __sttype;
};
#endif