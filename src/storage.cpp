#include "storage.h"
#include <Arduino.h>

Storage::Storage()
{
    if (nvs_flash_init() == ESP_OK)
    {
        Serial.println("NVS init ok");
        if (nvs_open("matches", NVS_READWRITE, &__st_handle) == ESP_OK)
        {
            Serial.println("NVS open ok");
        } else
        {
            Serial.println("NVS open failed");
        }
    } else
    {
      Serial.println("NVS init failed");
    }

};

Storage::~Storage()
{
    nvs_close(__st_handle);
};

void Storage::save(const char * key, matchrecord * mr) 
{
    size_t size = sizeof(matchrecord);
    esp_err_t t = nvs_set_blob(__st_handle, key, mr, size);
    t = nvs_commit(__st_handle);
    Serial.print(t);
};

matchrecord * Storage::read(const char * key)
{
    size_t size = 0;
    matchrecord * mr = new matchrecord;
    esp_err_t err;
    err = nvs_get_blob(__st_handle, key, NULL, &size);
    if (err != ESP_OK) Serial.println("Error");
    Serial.println(size);
    err = nvs_get_blob(__st_handle, key, mr, &size);
    if (err != ESP_OK) Serial.println("Error");
    return mr;
};

int Storage::size()
{
    return 0;
};

void Storage::clear_all()
{
    nvs_erase_all(__st_handle);
    nvs_commit(__st_handle);
};

void Storage::clear(const char * key)
{
    nvs_erase_key(__st_handle, key);
    nvs_commit(__st_handle);
}

