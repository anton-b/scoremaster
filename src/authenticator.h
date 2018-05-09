#include "storage.h"
#include "types.h"
#include "bt.h"
#include <cstring>

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
    BT * bt;

  public:
    Authenticator()
    {
        __storage_users = new Storage("users");
        bt = BT::getInstance();
    };

    void discover_users()
    {
        bt -> discover();
        user u;
        Serial.println("Started discovery.");
        while (!bt->is_discovery_complete())
        {
            delay(1000);
        };
        Serial.println("Finished discovery");
        std::set<std::string> addr_found = bt->get_discovered();

        for (std::set<std::string>::iterator i = addr_found.begin(); i != addr_found.end(); ++i)
        {
            int id = db_user_exists(i->c_str());
            Serial.println(i->c_str());
            Serial.println(id);
            if (id > -1)
            {
                u = db_get_user(id);
                __discovered_users.push_back(u);
            };
        }
    };

    void db_remove_all()
    {
        __storage_users -> remove_all();
    };

    player *db_add_user(player *p, const char *bt_mac)
    {
        if (db_user_exists(bt_mac) == -1)
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
        return __storage_users->list_elements();
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

    ~Authenticator()
    {
        delete bt;
    };
};