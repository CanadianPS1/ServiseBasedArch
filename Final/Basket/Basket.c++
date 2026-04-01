#include <iostream>
#include "Basket.h"
#include "nlohmann/json.hpp"
Basket::Basket(){
    RedisConnect();
    KafkaConnect();
}
void Basket::RedisConnect(){

}
void Basket::KafkaConnect(){

}
void Basket::AddToBasket(){

}
nlohmann::json Basket::GetBasketById(int id){
    nlohmann::json result;
    return result; 
}
nlohmann::json Basket::GetBasket(){
    nlohmann::json result;
    return result; 
}
void Basket::DeleteItem(int id){

}
void Basket::DeleteAllItems(){

}