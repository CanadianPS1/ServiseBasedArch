#include <iostream>
#include "nlohmann/json.hpp"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
class Resterant{
    public:
    Resterant();
    static void KafkaConnect();
    static void GetAll(nlohmann::json messageJson, mongocxx::client client);
    static void Search(nlohmann::json messageJson, mongocxx::client client);
    static mongocxx::client Connect();
};