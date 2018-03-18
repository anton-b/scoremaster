#ifndef STORAGELIB
#define STORAGELIB
#include "types.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <vector>
#include <algorithm>

class Storage
{
  public:
    Storage(const char *nspace);
    ~Storage();
    void add_element(int id, void *data, size_t size);
    void get_element(int id, void * result);
    std::vector<int> list_elements();
    void remove_element(int id);
    void remove_all();
    bool element_exists(int id);
  
    size_t length();
  protected:
    /*Underlyng memory manipulation*/
    void handle_nvs_error(esp_err_t error);
    void write(const char *key, void *data, size_t size);
    void read(const char *key, void *result);
    void clear(const char *key);
    void clear_all();
    /* Index manipulation */
    void ensure_index();
    void add_to_index(int id);
    void remove_from_index(int id);
    void commit_index();
    size_t key_size(const char *key);
    const char *make_key(int id);
    nvs_handle __st_handle;
    std::vector<int> __index;
};
#endif