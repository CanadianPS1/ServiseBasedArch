#include <iostream>
#include <map>
#include "Account.h"
#include "nlohmann/json.hpp"
#include <rdkafka.h>
#include <bits/stdc++.h>
int Account::id;
Account::Account(){
    KafkaConnect();
    Account::id = 0;
}
void Account::KafkaConnect(){
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
    rd_kafka_topic_partition_list_add(topics,"account",-1);
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
        // if(message.compare("add") == 0) AddToBasket(message);
        // else if(message.compare(0,4, "get1") == 0){
        //     GetBasketById(1);
        // }else if(message.compare("get") == 0) GetBasket();
        // else if(message.compare(0, 7, "delete1") == 0){
        //     DeleteItem(1);
        // }else if(message.compare("delete") == 0) DeleteAllItems();
        // else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}