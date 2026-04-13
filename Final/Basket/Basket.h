#include <iostream>
#include "nlohmann/json.hpp"
#include <hiredis/hiredis.h>
class Basket{
    public:
    Basket();
    static int id;
    static redisContext* RedisConnect();
    static void KafkaConnect();
    static void AddToBasket(std::string msg);
    static nlohmann::json GetBasketById(int id);
    static nlohmann::json GetBasket();
    static void DeleteItem(int id);
    static void DeleteAllItems();
};