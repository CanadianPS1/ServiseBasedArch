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
#include <bsoncxx/builder/basic/document.hpp>
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
        else if(type.compare("search") == 0) Search(messageJson, Connect());
        else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}
mongocxx::instance instance{};
httplib::Client httpClient("http://doorleap:9258");
mongocxx::client Resterant::Connect(){
    try{
        return mongocxx::uri("mongodb+srv://admit:passw0rd@schoolpilled.10u9kyg.mongodb.net/?appName=SchoolPilled");
    }catch(const std::exception& e){
        return Connect();
    }
}
void Resterant::GetAll(nlohmann::json messageJson, mongocxx::client client){
    auto db = client["Resterants"];
    auto collection = db["Resterants"];
    nlohmann::json result = nlohmann::json::array();
    auto cursor = collection.find({});
    for(auto&& doc : cursor){
        std::string bsonStr = bsoncxx::to_json(doc);
        result.push_back(nlohmann::json::parse(bsonStr));
    }
    nlohmann::json packet = {
            {"type", "collect"},
            {"success", "true"},
            {"items", result}
        };
    auto res = httpClient.Post("/resterants", packet.dump(), "application/json");
    if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
    else{
        std::cerr<<"failed to send packet"<<std::endl;
        if(res){
            std::cerr << "Status Code: " << res->status << std::endl;
            std::cerr << "Error Details: " << res->body << std::endl;
        }
    }
    
}
void Resterant::Search(nlohmann::json messageJson, mongocxx::client client){
    auto db = client["Resterants"];
    auto collection = db["Resterants"];
    std::string name = messageJson.at("amount");
    auto resultMongoObject = collection.find_one(bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("name", name)));
    if(!resultMongoObject){
        std::cerr<<"No Results"<<std::endl;
        return;
    }
    std::string bsonStr = bsoncxx::to_json(resultMongoObject->view());
    nlohmann::json result =  nlohmann::json::parse(bsonStr);    
    nlohmann::json packet = {
            {"type", "search"},
            {"success", "true"},
            {"items", result}
        };
    auto res = httpClient.Post("/resterants", packet.dump(), "application/json");
    if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
    else{
        std::cerr<<"failed to send packet"<<std::endl;
        if(res){
            std::cerr << "Status Code: " << res->status << std::endl;
            std::cerr << "Error Details: " << res->body << std::endl;
        }
    }
}
