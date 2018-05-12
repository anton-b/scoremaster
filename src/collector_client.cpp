#include "collector_client.h"

CollectorClient::CollectorClient(const char *wifi_ssid,
                                 const char *wifi_password,
                                 const char *collector_host,
                                 int collector_port,
                                 const char *agent_name,
                                 const char *agent_secret_key)
{
    init_wifi(wifi_ssid, wifi_password);
    __collector_port = collector_port;
    __collector_host = collector_host;
    __agent_name = agent_name;
    __agent_secret_key = agent_secret_key;
};

CollectorClient::~CollectorClient()
{
    deinit_wifi();
};

void CollectorClient::init_wifi(const char *wifi_ssid, const char *wifi_password)
{
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    };
    Serial.print("Wifi connected");
};

void CollectorClient::deinit_wifi()
{
    __tcp_client.stop();
    WiFi.disconnect();
}

void CollectorClient::send_match(matchrecord m)
{
    connect();
    authenticate();
    char *jsonstr = NULL;
    cJSON *json = matchrecord_to_json(m);
    jsonstr = cJSON_Print(json);
    const char * command = "send_match";
    char str[strlen(jsonstr) + strlen(command)];
    sprintf(str, "%s%s", command, json);
    send(str);
    disconnect();
    delete json;
};

void CollectorClient::send(char *payload)
{
    __tcp_client.print(payload);
};

void CollectorClient::disconnect()
{
    __tcp_client.stop();
};

void CollectorClient::connect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (__tcp_client.connect(__collector_host, __collector_port))
        {
            Serial.println("Connected to the collector");
        }
        else
        {
            throw("collector_connection_failed");
        };
    }
    else
    {
        throw("wifi_disconnected");
    }
};

void CollectorClient::authenticate()
{
    cJSON *authjson = cJSON_CreateObject();
    cJSON_AddStringToObject(authjson, "type", "agent");
    cJSON_AddStringToObject(authjson, "agent_name", __agent_name);
    cJSON_AddStringToObject(authjson, "secret_key", __agent_secret_key);
    char * json = cJSON_Print(authjson);
    const char * command = "authenticate";
    char str[strlen(json) + strlen(command)];
    sprintf(str, "%s%s", command, json);
    send(str);
    delete authjson;
};

cJSON *CollectorClient::player_to_json(player p)
{
    cJSON *playerjson = cJSON_CreateObject();
    cJSON_AddItemToObject(playerjson, "id", cJSON_CreateNumber(p.id));
    cJSON_AddItemToObject(playerjson, "name", cJSON_CreateString(p.name));
    return playerjson;
};

cJSON *CollectorClient::matchrecord_to_json(matchrecord m)
{
    cJSON *matchjson = cJSON_CreateObject();
    cJSON_AddItemToObject(matchjson, "id", cJSON_CreateNumber(m.id));
    cJSON_AddItemToObject(matchjson, "playerBlueA", player_to_json(m.playerBlueA));
    cJSON_AddItemToObject(matchjson, "playerBlueB", player_to_json(m.playerBlueB));
    cJSON_AddItemToObject(matchjson, "playerRedA", player_to_json(m.playerRedA));
    cJSON_AddItemToObject(matchjson, "playerRedB", player_to_json(m.playerRedB));
    cJSON_AddItemToObject(matchjson, "win", cJSON_CreateNumber(m.win));
    cJSON_AddItemToObject(matchjson, "blue_score", cJSON_CreateNumber(m.blue_score));
    cJSON_AddItemToObject(matchjson, "red_score", cJSON_CreateNumber(m.red_score));
    cJSON_AddItemToObject(matchjson, "time", cJSON_CreateNumber(m.time));
    cJSON_AddItemToObject(matchjson, "timestart", cJSON_CreateNumber(m.timestart));
    return matchjson;
};