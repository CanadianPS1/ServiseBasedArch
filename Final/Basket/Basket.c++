#include <iostream>
#include <map>
#include "Basket.h"
#include "nlohmann/json.hpp"
#include <hiredis/hiredis.h>
#include <rdkafka.h>
#include <bits/stdc++.h>
int Basket::id;
Basket::Basket(){
    RedisConnect();
    KafkaConnect();
    Basket::id = 0;
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
        if(message.compare("add") == 0) AddToBasket(message);
        else if(message.compare(0,4, "get1") == 0){
            GetBasketById(1);
        }else if(message.compare("get") == 0) GetBasket();
        else if(message.compare(0, 7, "delete1") == 0){
            DeleteItem(1);
        }else if(message.compare("delete") == 0) DeleteAllItems();
        else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}
void Basket::AddToBasket(std::string msg){
    float cost = 0;
    std::string resterant = "green";
    std::string item = "green";
    redisContext* context = RedisConnect();
    nlohmann::json Item = {{resterant},{item},{std::to_string(cost)}};
    std::string ItemString = Item.dump();
    redisReply* reply = (redisReply*)redisCommand(context, "SET %d %s", id, ItemString.c_str());
    id++;
    freeReplyObject(reply);
    redisFree(context);
}
nlohmann::json Basket::GetBasketById(int id){
    redisContext* context = RedisConnect();
    nlohmann::json result;
    redisReply* reply = (redisReply*)redisCommand(context, "GET %s", id);
    if(reply->type == REDIS_REPLY_STRING){
        result = nlohmann::json::parse(reply->str);;
        std::cout<<"Stored string in Redis: "<<reply->str<<std::endl;
    }else std::cout<<"Failed to retrieve the value."<<std::endl;
    freeReplyObject(reply);
    redisFree(context);
    return result; 
}
nlohmann::json Basket::GetBasket(){
    nlohmann::json result;
    redisContext* context = RedisConnect();
    std::map<int, std::string> json;
    for(int i = 0; i < id; i++){
        redisReply* reply = (redisReply*)redisCommand(context, "GET %s", id);
        if(reply->type == REDIS_REPLY_STRING){
            json.insert({i, reply->str});
            std::cout<<"Stored string in Redis: "<<reply->str<<std::endl;
        }else std::cout<<"Failed to retrieve the value."<<std::endl;
        freeReplyObject(reply);
    }
    redisFree(context);
    return result; 
}
void Basket::DeleteItem(int id){
    redisContext* context = RedisConnect();
    redisReply* reply = (redisReply*)redisCommand(context, "DEL %s", id);
    if(reply->type == REDIS_REPLY_STRING) std::cout<<"Deleted"<<reply->str<<std::endl;
    else std::cout<<"Failed to retrieve the value."<<std::endl;
    freeReplyObject(reply);
    redisFree(context);
}
void Basket::DeleteAllItems(){
    nlohmann::json result;
    redisContext* context = RedisConnect();
    for(int i = 0; i < id; i++){
        redisReply* reply = (redisReply*)redisCommand(context, "DEL %s", id);
        if(reply->type == REDIS_REPLY_STRING) std::cout<<"Deleted: "<<reply->str<<std::endl;
        else std::cout<<"Failed to retrieve the value."<<std::endl;
        freeReplyObject(reply);
    }
    id = 0;
    redisFree(context);
}