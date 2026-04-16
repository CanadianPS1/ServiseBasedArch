#include <iostream>
#include "nlohmann/json.hpp"
#include <hiredis/hiredis.h>
class Basket{
    public:
    Basket();
    static int id;
    static redisContext* RedisConnect();
    static void KafkaConnect();
    static void AddToBasket(std::string item, int cost);
    static nlohmann::json GetBasketById(std::string name);
    static nlohmann::json GetBasket();
    static void DeleteItem(std::string name);
    static void DeleteAllItems();
};