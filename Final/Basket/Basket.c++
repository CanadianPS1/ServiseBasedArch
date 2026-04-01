#include <iostream>
#include "Basket.h"
#include "nlohmann/json.hpp"
#include <hiredis/hiredis.h>
#include <rdkafka.h>
Basket::Basket(){
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
    std::cerr<<"grinch"<<std::endl;
    while(true){
        rd_kafka_message_t* msg = rd_kafka_consumer_poll(consumer, 1000);
        if(!msg) continue;
        if(msg->err){
            rd_kafka_message_destroy(msg);
            continue;
        }
        //TODO write kafka message logic
        rd_kafka_message_destroy(msg);
    }
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