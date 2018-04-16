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
        BT bt = BT::getInstance();
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

    std::vector<user> get_discovered_users()
    {
        return __discovered_users;
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