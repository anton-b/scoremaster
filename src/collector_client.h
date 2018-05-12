#include "types.h"
#include <WiFi.h>
#include "cJSON.h"
#include <cstdio>
#include <cstring>

class CollectorClient
{
  public:
    CollectorClient(const char *wifi_ssid,
                    const char *wifi_password,
                    const char *collector_host,
                    int collector_port,
                    const char *agent_name,
                    const char *agent_secret_key);
    ~CollectorClient();
    void authenticate();
    void send_match(matchrecord m);
    void init_wifi(const char *wifi_ssid, const char *wifi_password);
    void deinit_wifi();
    void send(char *payload);
    void connect();
    void disconnect();
    cJSON *matchrecord_to_json(matchrecord m);
    cJSON *player_to_json(player p);

  private:
    const char *__collector_host;
    int __collector_port;
    const char *__agent_name;
    const char *__agent_secret_key;
    WiFiClient __tcp_client;
};