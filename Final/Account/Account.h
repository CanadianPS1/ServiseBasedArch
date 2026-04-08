#include <iostream>
#include "nlohmann/json.hpp"
#include <mysql.h>
class Account{
    public:
    Account();
    static int id;
    static void KafkaConnect();
    static void Login(nlohmann::json messageJson, MYSQL* connection);
    static void Create(nlohmann::json messageJson, MYSQL* connection);
    static MYSQL* Connect();
};