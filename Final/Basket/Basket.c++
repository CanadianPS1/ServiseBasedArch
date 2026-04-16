#include <iostream>
#include <map>
#include "Basket.h"
#include "nlohmann/json.hpp"
#include <hiredis/hiredis.h>
#include <rdkafka.h>
#include <bits/stdc++.h>
#include <string>
int Basket::id;
Basket::Basket(){
    Basket::id = 0;
    RedisConnect();
    KafkaConnect();
}
redisContext* Basket::RedisConnect(){
    redisContext* context = redisConnect("redis", 6379);
    if(context == nullptr || context->err) {
        if(context) std::cerr << "Redis Connection error: " << context->errstr << std::endl;
        else std::cerr << "Redis Connection error: can't allocate Redis context" << std::endl;
    }else std::cerr << "Connected to Redis" << std::endl;
    return context;
}
void Basket::KafkaConnect(){
    char errstr[512];
    rd_kafka_conf_t* conf = rd_kafka_conf_new();
    rd_kafka_conf_set(conf, "bootstrap.servers", "kafka:9092", errstr, sizeof(errstr));
    rd_kafka_conf_set(conf, "group.id", "DoorLeap", errstr, sizeof(errstr));
    rd_kafka_conf_set(conf, "auto.offset.reset", "earliest", nullptr, 0);
    rd_kafka_t* consumer = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));
    if(!consumer){
        std::cerr<<errstr<<std::endl;
        return;
    }
    rd_kafka_poll_set_consumer(consumer);
    rd_kafka_topic_partition_list_t* topics = rd_kafka_topic_partition_list_new(2);
    rd_kafka_topic_partition_list_add(topics,"cart",-1);
    if(rd_kafka_subscribe(consumer, topics)){
        std::cerr<<"Subscribe failed"<<std::endl;
        return;
    }
    while(true){
        rd_kafka_message_t* msg = rd_kafka_consumer_poll(consumer, 1000);
        if(!msg) continue;
        if(msg->err){
            rd_kafka_message_destroy(msg);
            continue;
        }
        std::string message((char*)msg->payload, msg->len);
        nlohmann::json messageJson = nlohmann::json::parse(message);
        std::string type = messageJson.at("type");
        std::string item = messageJson.at("item");
        if(type.compare("add") == 0){
            int cost = messageJson.at("cost");
            AddToBasket(item, cost);
        }
        else if(type.compare("delete") == 0) DeleteItem(item);
        else if(type.compare("clear") == 0) DeleteAllItems();
        else if(type.compare("get") == 0) std::cout<<GetBasket().dump()<<std::endl;
        else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}
void Basket::AddToBasket(std::string itemToAdd, int cost){
    redisContext* context = RedisConnect();
    std::string key = "basket:" + itemToAdd;
    nlohmann::json item = {
        {"item", itemToAdd},
        {"cost", cost}
    };
    std::string value = item.dump();
    std::cout<<"adding the item to the redis db"<<std::endl;
    redisReply* reply = (redisReply*)redisCommand(context, "SET %s %s", key.c_str(), value.c_str());
    if(!reply) std::cout<<"failed to add the item to the redis db"<<std::endl;
    else std::cout<<"added the item to the cart"<<std::endl;
    if(reply) freeReplyObject(reply);
    redisFree(context);
}
nlohmann::json Basket::GetBasketById(std::string name){
    std::cout<<"starting the search for the item"<<std::endl;
    redisContext* context = RedisConnect();
    nlohmann::json result;
    std::string key = "basket:" + name;
    redisReply* reply = (redisReply*)redisCommand(context, "GET %s", key.c_str());
    if(reply && reply->type == REDIS_REPLY_STRING) result = nlohmann::json::parse(reply->str);
    else std::cout<<"item not in basket: "<<name<<std::endl;
    if(reply) freeReplyObject(reply);
    redisFree(context);
    return result;
}
nlohmann::json Basket::GetBasket(){
    std::cout<<"starting the prosses of gathering all items"<<std::endl;
    redisContext* context = RedisConnect();
    nlohmann::json result = nlohmann::json::array();
    std::cout<<"amout to stazrt the search"<<std::endl;
    redisReply* keys = (redisReply*)redisCommand(context, "KEYS basket:*");
    if(!keys){
        std::cout<<"keys reply is null"<<std::endl;
        redisFree(context);
        return result;
    }
    if(keys->type != REDIS_REPLY_ARRAY){
        std::cout<<"Redis KEYS error: "<<keys->str<<std::endl;
        freeReplyObject(keys);
        redisFree(context);
        return result;
    }
    std::cout<<"grabbed all the keys"<<std::endl;
    for(size_t i = 0; i < keys->elements; i++){
        std::cout<<"searching for the "<<i<<" item"<<std::endl;
        std::string key = keys->element[i]->str;
        redisReply* reply = (redisReply*)redisCommand(context, "GET %s", key.c_str());
        if(reply && reply->type == REDIS_REPLY_STRING) result.push_back(nlohmann::json::parse(reply->str));
        else std::cout<<"failed to get or parse key: "<<key<<std::endl;
        if(reply) freeReplyObject(reply);
    }
    std::cout<<"gathered all the requested items"<<std::endl;
    freeReplyObject(keys);
    redisFree(context);
    return result;
}
void Basket::DeleteItem(std::string name) {
    redisContext* context = RedisConnect();
    std::string key = "basket:" + name;
    redisReply* reply = (redisReply*)redisCommand(context,"DEL %s", key.c_str());
    if(reply && reply->integer > 0) std::cout<<"deleted: "<<name<<std::endl;
    else std::cout << "item not in basket: " << name << std::endl;
    if(reply) freeReplyObject(reply);
    redisFree(context);
}
void Basket::DeleteAllItems() {
    redisContext* context = RedisConnect();
    redisReply* keys = (redisReply*)redisCommand(context, "KEYS basket:*");
    if(keys && keys->type == REDIS_REPLY_ARRAY){
        for(size_t i = 0; i < keys->elements; i++){
            std::string key = keys->element[i]->str;
            redisReply* delReply = (redisReply*)redisCommand(context, "DEL %s", key.c_str());
            if(delReply) freeReplyObject(delReply);
        }
    }
    if(keys) freeReplyObject(keys);
    redisFree(context);
    std::cout<<"basket cleared"<<std::endl;
}