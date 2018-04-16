#include "bt.h"

BT *BT::instance = 0;
std::set<std::string> BT::__discovered_addresses;
bool BT::discovery_complete = false;
const char *BT::__device_name = "AUTHENTICATOR";

BT *BT::getInstance()
{
    if (instance == 0)
    {
        instance = new BT();
    }

    return instance;
};

void BT::bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
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
            discovery_complete = true;
        }
        else if (param->disc_st_chg.state == ESP_BT_GAP_DISCOVERY_STARTED)
        {
            discovery_complete = false;
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

BT::BT()
{
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_dev_set_device_name(__device_name);

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

    if (esp_bt_gap_register_callback(&BT::bt_app_gap_cb) != ESP_OK)
    {
        throw("failed_registering_callback");
    }
};

void BT::discover()
{
    // Each discovered mac is processed at handle_device_found
    esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
};

void BT::handle_device_found(esp_bt_gap_cb_param_t *param)
{
    char bda_str[18];
    bda2str(param->disc_res.bda, bda_str, 18);
    std::string st(bda_str);
    __discovered_addresses.insert(st);
}

bool BT::is_discovery_complete()
{
    return discovery_complete;
};

char *BT::bda2str(esp_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18)
    {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
};

std::set<std::string> BT::get_discovered()
{
    return __discovered_addresses;
};

BT::~BT()
{
    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();
    delete instance;
    instance = nullptr;
}