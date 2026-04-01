#include <iostream>
#include "nlohmann/json.hpp"
class Basket{
    public:
    Basket();
    static void RedisConnect();
    static void KafkaConnect();
    static void AddToBasket();
    static nlohmann::json GetBasketById(int id);
    static nlohmann::json GetBasket();
    static void DeleteItem(int id);
    static void DeleteAllItems();
};