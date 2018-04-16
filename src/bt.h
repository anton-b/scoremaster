#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include <set>
#include <string>
#include <Arduino.h>

class BT
{
public:
  static BT *getInstance();

private:
  static BT *instance;
  static std::set<std::string> __discovered_addresses;
  static bool discovery_complete;
  static const char *__device_name;
  static char *bda2str(esp_bd_addr_t bda, char *str, size_t size);
  static void handle_device_found(esp_bt_gap_cb_param_t *param);
  BT();
  ~BT();

public:
  static void bt_app_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
  static std::set<std::string> get_discovered();
  static void discover();
  static bool is_discovery_complete();
};
