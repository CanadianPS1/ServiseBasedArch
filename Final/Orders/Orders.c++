#include <iostream>
#include <map>
#include "Orders.h"
#include "nlohmann/json.hpp"
#include <rdkafka.h>
#include <bits/stdc++.h>
#include <string>
#include <thread>
#include <curl/curl.h>
#include <chrono>
#include <cstring>
Orders::Orders(){
    KafkaConnect();
}
void Orders::KafkaConnect(){
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
    rd_kafka_topic_partition_list_add(topics,"orders",-1);
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
        if(type.compare("email") == 0) Email(messageJson);
        else if(type.compare("update") == 0){
            nlohmann::json message = {
                {"type", "clear"},
                {"item", "null"}
            };
            Produce(message.dump(), "cart");
            UpdateOrders();
        }
        else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}
size_t Orders::payload_source(void *ptr, size_t size, size_t nmemb, void *userp){
        const char **payload_text = (const char **)userp;
        if(*payload_text){
            size_t len = strlen(*payload_text);
            if(len > size * nmemb) len = size * nmemb;
            memcpy(ptr, *payload_text, len);
            *payload_text += len;
            return len;
        }
        return 0;
    }
void Orders::Email(nlohmann::json messageJson){
    std::string body = messageJson["body"];
    std::string to = messageJson["to"];
    std::string subject = messageJson["subject"];
    CURL *curl = curl_easy_init();
    if(!curl) return;
    std::string from = "rogelio.kertzmann37@ethereal.email";
    std::string payload = "To: " + to + "\r\n" "From: " + from + "\r\n" "Subject: " + subject + "\r\n" "\r\n" + body + "\r\n";
    struct curl_slist *recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());
    const char* payload_ptr = payload.c_str();
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.ethereal.email:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "rogelio.kertzmann37@ethereal.email");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "PwDCnV8HWgvzZRnyhK");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &payload_ptr);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) std::cerr<<"Email failed: "<< curl_easy_strerror(res)<< std::endl;
    else std::cout<<"Email sent to "<<to<<std::endl;
    curl_slist_free_all(recipients);
    curl_easy_cleanup(curl);
}
void Orders::UpdateOrders(){
    const auto now = std::chrono::system_clock::now();
    std::thread orderProsses([](){
        bool didit = false;
        while(true){     
            if(!didit){
                Produce("{{\"type\", \"deliveryUpdate\"}, {\"update\", \"order started\"}}", "gui");
                Produce("{{\"type\", \"email\"}, {\"to\", \"user@gmail.com\"}, {\"subject\", \"delivery update\"}, {\"body\", \"Your Order has been started!\"}}", "order");
                didit = true;
            }
            std::this_thread::sleep_for(std::chrono::seconds(30));
            Produce("{{\"type\", \"deliveryUpdate\"}, {\"update\", \"delivery driver arived at resterant\"}}", "gui");
            Produce("{{\"type\", \"email\"}, {\"to\", \"user@gmail.com\"}, {\"subject\", \"delivery update\"}, {\"body\", \"The Delivery Driver has arived at the target resterant\"}}", "order");
            std::this_thread::sleep_for(std::chrono::seconds(30));
            Produce("{{\"type\", \"deliveryUpdate\"}, {\"update\", \"delivery driver arived at resterant\"}}", "gui");
            Produce("{{\"type\", \"email\"}, {\"to\", \"user@gmail.com\"}, {\"subject\", \"delivery update\"}, {\"body\", \"The Delivery Driver has arived at the target resterant\"}}", "order");
            std::this_thread::sleep_for(std::chrono::seconds(30));
            Produce("{{\"type\", \"deliveryUpdate\"}, {\"update\", \"delivery driver has picked up order\"}}", "gui");
            Produce("{{\"type\", \"email\"}, {\"to\", \"user@gmail.com\"}, {\"subject\", \"delivery update\"}, {\"body\", \"Your Delivery Driver has picked up your order and is now on the way to your target destination\"}}", "order");
            std::this_thread::sleep_for(std::chrono::seconds(30));
            Produce("{{\"type\", \"deliveryUpdate\"}, {\"update\", \"order delivered\"}}", "gui");
            Produce("{{\"type\", \"email\"}, {\"to\", \"user@gmail.com\"}, {\"subject\", \"delivery update\"}, {\"body\", \"Your Order has been delivered!!!\"}}", "order");
            break;
        }
    });
    if(orderProsses.joinable()) orderProsses.join();
    
}   

void Orders::Produce(const std::string& message, const char* topic){
    const char* brokers = "kafka:9092";
    char errstr[512];
    rd_kafka_conf_t* conf = rd_kafka_conf_new();
    if(rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK){
        std::cerr << errstr << std::endl;
        return;
    }
    rd_kafka_t* producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if(!producer){
        std::cerr << errstr << std::endl;
        return;
    }
    const char* msg = message.c_str();
    rd_kafka_resp_err_t err = rd_kafka_producev(
        producer,
        RD_KAFKA_V_TOPIC(topic),
        RD_KAFKA_V_VALUE((void*)msg, strlen(msg)),
        RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
        RD_KAFKA_V_END
    );
    if(err) std::cerr << "Send failed\n";
    else std::cout << "Sent\n";
    rd_kafka_flush(producer, 5000);
    rd_kafka_destroy(producer);
}