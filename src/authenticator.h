#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "storage.h"
#include "types.h"
#include <functional>

struct user
{
    const char *bt_mac;
    player pl;
};

class Authenticator
{
    Storage *__storage_users;
    std::vector<user> __discovered_users;
    const char *__dev_name = "AUTHENTICATOR";

  public:
    Authenticator()
    {

        Serial.begin(115200);
        __storage_users = new Storage("users");
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
        esp_bt_dev_set_device_name(__dev_name);

        /* set discoverable and connectable mode, wait to be connected */
        esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);

        if (esp_bt_controller_init(&bt_cfg) != ESP_OK)
        {
            throw("initialize_controller_failed");
        }

        if (esp_bt_controller_enable(ESP_BT_MODE_BTDM) != ESP_OK)
        {
            throw("enable_controller_failed");
        }

        if (esp_bluedroid_init() != ESP_OK)
        {
            throw("initialize_bluedroid_failed");
        }

        if (esp_bluedroid_enable() != ESP_OK)
        {
            throw("enable_bluedroid_failed");
        }

        /* register GAP callback function */
        /* NOTE: to be called after esp_bluedroid_enable() */


        if (esp_bt_gap_register_callback(&bt_app_gap_cb) != ESP_OK)
        {
            throw("failed_registering_callback");
        }
    };
    player *db_add_user(player *p, const char *bt_mac)
    {
        if (!db_user_exists(bt_mac))
        {
            user u;
            u.bt_mac = bt_mac;
            u.pl = *p;
            int id = __storage_users->add_element(&u, sizeof(user));
            p->id = id;
            return p;
        };
    };

    int db_user_exists(const char *bt_mac)
    {
        user usr;
        std::vector<int> list = __storage_users->list_elements();
        for (std::vector<int>::iterator i = list.begin(); i != list.end(); ++i)
        {
            __storage_users->get_element(*i, &usr);
            if (usr.bt_mac == bt_mac)
            {
                return *i;
            }
        };
        return -1;
    };

    std::vector<int> db_list_users()
    {
        std::vector<int> list = __storage_users->list_elements();
        return list;
    };

    user db_get_user(int id)
    {
        user usr;
        __storage_users->get_element(id, &usr);
        return usr;
    };

    void db_delete_user(int id)
    {
        __storage_users->remove_element(id);
    };

    void discover_users()
    {
        // Each discovered mac is processed at handle_device_found
        esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
    }

    std::vector<user> get_discovered_users()
    {
        return __discovered_users;
    }

    static char *bda2str(esp_bd_addr_t bda, char *str, size_t size)
    {
        if (bda == NULL || str == NULL || size < 18)
        {
            return NULL;
        }

        uint8_t *p = bda;
        sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
                p[0], p[1], p[2], p[3], p[4], p[5]);
        return str;
    }

    void handle_device_found(esp_bt_gap_cb_param_t *param)
    {
        char bda_str[18];
        bda2str(param->disc_res.bda, bda_str, 18);
        user u;
        int id = db_user_exists(bda_str);
        if (id > -1)
        {
            u = db_get_user(id);
            __discovered_users.push_back(u);
        };
    }

    void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
    {
        char bda_str[18];

        switch (event)
        {
        case ESP_BT_GAP_DISC_RES_EVT:
        {
            handle_device_found(param);
            break;
        }
        case ESP_BT_GAP_DISC_STATE_CHANGED_EVT:
        {
            if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STOPPED)
            {
                Serial.println("Device discovery stopped.");
            }
            else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED)
            {
                Serial.println("Discovery started.");
            }
            break;
        }
        default:
        {
            Serial.printf("event: %d\n", event);
            break;
        }
        }
        return;
    }

    ~Authenticator()
    {
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        esp_bt_controller_disable();
        esp_bt_controller_deinit();
    };
};