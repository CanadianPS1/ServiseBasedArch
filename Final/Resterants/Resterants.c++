#include <iostream>
#include <map>
#include "Resterants.h"
#include "nlohmann/json.hpp"
#include "httplib/httplib.h"
#include <rdkafka.h>
#include <bits/stdc++.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
Resterant::Resterant(){
    KafkaConnect();
}
void Resterant::KafkaConnect(){
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
    rd_kafka_topic_partition_list_add(topics,"resterant",-1);
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
        if(type.compare("collect") == 0) GetAll(messageJson, Connect());
        else if(type.compare("search") == 0) GetAll(messageJson, Connect());
        else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}
mongocxx::client Resterant::Connect(){
    try{
        mongocxx::client client(mongocxx::uri("mongodb+srv://admit:passw0rd@schoolpilled.10u9kyg.mongodb.net/?appName=SchoolPilled"));
        return client;
    }catch(const std::exception& e){
        mongocxx::instance instance{};
        return Connect();
    }
}
void Resterant::GetAll(nlohmann::json messageJson, mongocxx::client client){
    std::cerr<<"got into the get all method"<<std::endl;
    mongocxx::database db = client["Resterants"];
    mongocxx::collection collection = db["Resterants"];
    nlohmann::json result = nlohmann::json::array();
    mongocxx::cursor cursor cursor = collection.find({});
    for(auto&& doc : cursor){
        std::string bsonStr = bsoncxx::to_json(doc);
        result.push_back(nlohmann::json::parse(bsonStr));
    }
    
}
void Resterant::Search(nlohmann::json messageJson, mongocxx::client client){
    mongocxx::database db = client["Resterants"];
    mongocxx::collection collection = db["Resterants"];
    auto resultMongoObject = collection.find_one(document{}<<"name"<<name<<finalize);
    std::string bsonStr = bsoncxx::to_json(resultMongoObject->view());
    nlohmann::json result =  nlohmann::json::parse(bsonStr);
}
