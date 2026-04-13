#include <iostream>
#include <map>
#include "Account.h"
#include "nlohmann/json.hpp"
#include "jwt/include/jwt-cpp/jwt.h"
#include "httplib/httplib.h"
#include <rdkafka.h>
#include <bits/stdc++.h>
#include <mysql.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
Account::Account(){
    KafkaConnect();
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
    rd_kafka_topic_partition_list_add(topics,"accounts",-1);
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
        if(type.compare("login") == 0) Login(messageJson, Connect());
        else if(type.compare("create") == 0) Create(messageJson, Connect());
        else std::cerr<<"WHAT THE HECK"<<std::endl;
        rd_kafka_message_destroy(msg);
    }
}
MYSQL* Account::Connect(){
    const char* host = std::getenv("DB_HOST");
    const char* user = std::getenv("DB_USER");
    const char* password = std::getenv("DB_PASSWORD");
    const char* database = std::getenv("DB_NAME");
    MYSQL* connection = mysql_init(nullptr);
    if(!connection){
        std::cerr << "MySQL init error" << std::endl;
        return nullptr;
    }
    const char* hostStr = host ? host : "mariadb";
    const char* userStr = user ? user : "user";
    const char* passwordStr = password ? password : "abc";
    const char* dbStr = database ? database : "accounts";
    if(!mysql_real_connect(connection, hostStr, userStr, passwordStr, dbStr, 3306, nullptr, 0)){
        std::cerr << "MySQL connection error: " << mysql_error(connection) << std::endl;
        mysql_close(connection);
        return nullptr;
    }
    return connection;
}
void Account::Login(nlohmann::json messageJson, MYSQL* connection){
    std::string username = messageJson.at("username");
    std::string password = messageJson.at("password");
    std::string query = "SELECT id, password FROM users WHERE username='" + username + "'";
    if(mysql_query(connection, query.c_str())) {
        std::cerr << mysql_error(connection) << std::endl;
        return;
    }
    MYSQL_RES* result = mysql_store_result(connection);
    if(!result){
        std::cerr << "No result from query" << std::endl;
        return;
    }
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row){
        std::string dbPassword = row[1];
        if(dbPassword == password){
            std::cout << "User logged in!" << std::endl;
            nlohmann::json packet = {
                {"type", "login"},
                {"success", "true"},
                {"jwtToken", CreateToken(username, "grinchGrinchYellowGrinch")},
                {"username", username},
                {"message", "user loged in succesfuly"}
            };
            const std::string baseURL = "http://doorleap:9258";
            httplib::Client client(baseURL.c_str());
            auto res = client.Post("/logins", packet.dump(), "application/json");
            if(res && res->status == 201) std::cerr<<"packet sent"<<std::endl;
            else{
                std::cerr<<"failed to send packet"<<std::endl;
                if(res){
                    std::cerr << "Status Code: " << res->status << std::endl;
                    std::cerr << "Error Details: " << res->body << std::endl;
                }
            }
        }else{
            std::cout << "Incorrect Password" << std::endl;
            nlohmann::json packet = {
                {"type", "login"},
                {"success", "false"},
                {"jwtToken", CreateToken(username, "grinchGrinchYellowGrinch")},
                {"username", username},
                {"message", "user failed to log in"}
            };
            const std::string baseURL = "http://doorleap:9258";
            httplib::Client client(baseURL.c_str());
            auto res = client.Post("/logins", packet.dump(), "application/json");
            if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
            else{
                std::cerr<<"failed to send packet"<<std::endl;
                if(res){
                    std::cerr << "Status Code: " << res->status << std::endl;
                    std::cerr << "Error Details: " << res->body << std::endl;
                }
            }
        }
    }else{
        std::cout << "Username not found" << std::endl;
        nlohmann::json packet = {
            {"type", "login"},
            {"success", "false"},
            {"jwtToken", CreateToken(username, "grinchGrinchYellowGrinch")},
            {"username", username},
            {"message", "user failed to log in"}
        };
        const std::string baseURL = "http://doorleap:9258";
        httplib::Client client(baseURL.c_str());
        auto res = client.Post("/logins", packet.dump(), "application/json");
        if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
        else{
            std::cerr<<"failed to send packet"<<std::endl;
            if(res){
                std::cerr << "Status Code: " << res->status << std::endl;
                std::cerr << "Error Details: " << res->body << std::endl;
            }
        }
    }
    mysql_free_result(result);
    mysql_close(connection);
}
void Account::Create(nlohmann::json messageJson, MYSQL* connection){
    std::string username = messageJson.at("username");
    std::string query = "SELECT id, username FROM users WHERE username='" + username + "'";
    if(mysql_query(connection, query.c_str())) {
        std::cerr << mysql_error(connection) << std::endl;
        return;
    }
    MYSQL_RES* result = mysql_store_result(connection);
    MYSQL_ROW row;
    if(mysql_fetch_row(result) == 0){
        std::string password = messageJson.at("password");
        //TODO add the email to the GUI
        std::string query = "INSERT INTO users (username, password, email) VALUES ('" + username + "', '" + password + "', 'grok@gmail.beans')";
        if(mysql_query(connection, query.c_str())){
            std::cerr<<"Insert failed: "<<mysql_error(connection)<<std::endl;
            nlohmann::json packet = {
                {"type", "create"},
                {"success", "false"},
                {"jwtToken", CreateToken(username, "grinchGrinchYellowGrinch")},
                {"username", username},
                {"message", "db failed to insert"}
            };
            const std::string baseURL = "http://doorleap:9258";
            httplib::Client client(baseURL.c_str());
            auto res = client.Post("/logins", packet.dump(), "application/json");
            if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
            else{
                std::cerr<<"failed to send packet"<<std::endl;
                if(res){
                    std::cerr << "Status Code: " << res->status << std::endl;
                    std::cerr << "Error Details: " << res->body << std::endl;
                }
            }
        }else{
            std::cout<<"User inserted successfully."<<std::endl;
            nlohmann::json packet = {
                {"type", "create"},
                {"success", "true"},
                {"jwtToken", CreateToken(username, "grinchGrinchYellowGrinch")},
                {"username", username},
                {"message", "user made an account"}
            };
            const std::string baseURL = "http://doorleap:9258";
            httplib::Client client(baseURL.c_str());
            auto res = client.Post("/logins", packet.dump(), "application/json");
            if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
            else{
                std::cerr<<"failed to send packet"<<std::endl;
                if(res){
                    std::cerr << "Status Code: " << res->status << std::endl;
                    std::cerr << "Error Details: " << res->body << std::endl;
                }
            }
        }
    }else{
        std::cerr<<"username is taken"<<std::endl;
        nlohmann::json packet = {
            {"type", "create"},
            {"success", "false"},
            {"jwtToken", CreateToken(username, "grinchGrinchYellowGrinch")},
            {"username", username},
            {"message", "username was taken"}
        };
        const std::string baseURL = "http://doorleap:9258";
        httplib::Client client(baseURL.c_str());
        auto res = client.Post("/logins", packet.dump(), "application/json");
        if(res && res->status == 200) std::cerr<<"packet sent"<<std::endl;
        else{
            std::cerr<<"failed to send packet"<<std::endl;
            if(res){
                std::cerr << "Status Code: " << res->status << std::endl;
                std::cerr << "Error Details: " << res->body << std::endl;
            }
        }
    }
    mysql_free_result(result);
    mysql_close(connection);
}
std::string Account::CreateToken(std::string user, const std::string& secret){
    auto token = jwt::create()
        .set_issuer("DoorLeap")
        .set_type("JWT")
        .set_payload_claim("user", jwt::claim(user))
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::minutes{60})
        .sign(jwt::algorithm::hs256{secret});
    return token;
}