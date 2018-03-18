#include "storage.h"
#include <Arduino.h>
#include <string.h>




Storage::Storage(const char *nspace)
{
    handle_nvs_error(nvs_flash_init());
    handle_nvs_error(nvs_open(nspace, NVS_READWRITE, &__st_handle));
    ensure_index();
};

Storage::~Storage()
{
    nvs_close(__st_handle);
};

void Storage::add_element(int id, void *data, size_t size)
{
    if (element_exists(id) == false)
    {
        write(make_key(id), data, size);
        add_to_index(id);
        commit_index();
    } else 
    {
        throw "element_exists";
    };
};

void Storage::get_element(int id, void * result)
{
    if (element_exists(id))
    {
        read(make_key(id), result);
    };
}

std::vector<int> Storage::list_elements()
{
    return __index;
};

void Storage::remove_element(int id)
{
    clear(make_key(id));
    remove_from_index(id);
    commit_index();
};

void Storage::remove_all()
{
    // Just clear all namespace keys including index
    clear_all();
};

void Storage::write(const char *key, void *data, size_t size)
{
    handle_nvs_error(nvs_set_blob(__st_handle, key, data, size));
    handle_nvs_error(nvs_commit(__st_handle));
};

void Storage::read(const char *key, void *result)
{
    size_t size = key_size(key);
    handle_nvs_error(nvs_get_blob(__st_handle, key, result, &size));
};

size_t Storage::key_size(const char *key)
{
    size_t size = 0;
    handle_nvs_error(nvs_get_blob(__st_handle, key, NULL, &size));
    return size;
};

void Storage::clear_all()
{
    handle_nvs_error(nvs_erase_all(__st_handle));
    handle_nvs_error(nvs_commit(__st_handle));
};

void Storage::clear(const char *key)
{
    handle_nvs_error(nvs_erase_key(__st_handle, key));
    handle_nvs_error(nvs_commit(__st_handle));
};

size_t Storage::length()
{
    return __index.size();
};

void Storage::add_to_index(int id)
{
    __index.push_back(id);
};

void Storage::remove_from_index(int id)
{
    for (std::vector<int>::iterator i = __index.begin(); i != __index.end(); ++i)
    {
        if (*i = id)
        {
            __index.erase(i);
        };
    };
    commit_index();
};

bool Storage::element_exists(int id)
{
    return (find(__index.begin(), __index.end(), id) != __index.end());
};
 
void Storage::commit_index()
{
    if (__index.size() > 0)
    {
        write("__index__", &__index[0], sizeof(int) * __index.size());
    } else
    {
        clear("__index__");
    };
};

void Storage::ensure_index()
{
    try
    {
        int index_size = key_size("__index__") / sizeof(int);
        int *index = new int[index_size];
        read("__index__", index);
        __index = std::vector<int>(index, index + index_size);
        delete[] index;
    }
    catch (const char *e)
    {
        return;
    }
};

const char *Storage::make_key(int id)
{
    String key = String(id, 10);
    return key.c_str();
};

void Storage::handle_nvs_error(esp_err_t error)
{
    switch (error)
    {
    case ESP_OK:
        break;
    //init errors
    case ESP_ERR_NVS_NO_FREE_PAGES:
        throw "no_free_pages";
    //open errors
    case ESP_ERR_NVS_NOT_INITIALIZED:
        throw "not_initialized";
    case ESP_ERR_NVS_PART_NOT_FOUND:
        throw "part_not_found";
    // read and common errors
    case ESP_ERR_NVS_INVALID_NAME:
        throw "invalid_name";
    case ESP_ERR_NVS_INVALID_LENGTH:
        throw "invalid_length";
    case ESP_ERR_NVS_NOT_FOUND:
        throw "not_found";
    case ESP_ERR_NVS_INVALID_HANDLE:
        throw "invalid_handle";
    // write errors
    case ESP_ERR_NVS_READ_ONLY:
        throw "nvs_readonly";
    case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
        throw "not_enough_space";
    case ESP_ERR_NVS_REMOVE_FAILED:
        throw "nvs_remove_failed";
    case ESP_ERR_NVS_KEY_TOO_LONG:
        throw "key_too_long";
    case ESP_ERR_NVS_VALUE_TOO_LONG:
        throw "value_too_long";
    default:
        throw "unknown_error";
    };
};