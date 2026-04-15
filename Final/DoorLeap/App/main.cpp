// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "autogen/environment.h"
#include "main.h"
#include <librdkafka/rdkafka.h>
#include "nlohmann/json.hpp"
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "main.moc"
#include "HttpServer.h"
Main::QtItems Main::items;
int main(int argc, char *argv[]){
    set_qt_environment();
    QApplication app(argc, argv);
    HttpServer server;
    QQmlApplicationEngine engine;
    Main mainObject;
    engine.rootContext()->setContextProperty("mainObject", &mainObject);
    const QUrl url(mainQmlFile);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,[url](QObject *obj, const QUrl &objUrl){if(!obj && url == objUrl) QCoreApplication::exit(-1);},Qt::QueuedConnection);
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");
    engine.load(url);
    QObject* rootObject = engine.rootObjects().first();
    if(engine.rootObjects().isEmpty()) return -1;
    Main::items.loginMenu = rootObject->findChild<QObject*>("signIn");
    Main::items.signoutButtonBackground = rootObject->findChild<QObject*>("signoutButtonBackground");
    Main::items.homeButtonBackground = rootObject->findChild<QObject*>("homeButtonBackground");
    Main::items.ordersButtonBackground = rootObject->findChild<QObject*>("ordersButtonBackground");
    Main::items.deliverysButtonBackground = rootObject->findChild<QObject*>("deliverysButtonBackground");
    Main::items.cartButtonBackground = rootObject->findChild<QObject*>("cartButtonBackground");
    Main::items.searchBarBackground = rootObject->findChild<QObject*>("searchBarBackground");
    Main::items.background = rootObject->findChild<QObject*>("background");
    return app.exec();
}
Main::Main(QObject *parent) : QObject(parent) {}
void Main::LogIn(const QString &username, const QString &password){
    std::string pass = password.toStdString();
    std::string use = username.toStdString();
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource(pass, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder( new CryptoPP::StringSink(digest))));
    qDebug()<<"LogIn called with username:"<<use<<"password:"<<digest;
    nlohmann::json messageJson = {
        {"type", "login"},
        {"username", use},
        {"password", digest}
    };
    std::string message = messageJson.dump();
    Main::ProduceMessage(message, "accounts");
}
void Main::CreateAccount(const QString &username, const QString &password) {
    std::string pass = password.toStdString();
    std::string use = username.toStdString();
    CryptoPP::SHA256 hash;
    std::string digest;
    CryptoPP::StringSource(pass, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder( new CryptoPP::StringSink(digest))));
    nlohmann::json messageJson = {
        {"type", "create"},
        {"username", use},
        {"password", digest}
    };
    qDebug()<<"CreateAccount called with username:"<<username<<"password:"<<digest;
    std::string message = messageJson.dump();
    Main::ProduceMessage(message, "accounts");
}
void Main::ProduceMessage(const std::string& message, const char* topic){
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
void Main::HandleLogInResponse(std::string type, std::string success, std::string message, std::string username, std::string authToken){
    if(success.compare("true") == 0){
        if(Main::items.loginMenu && Main::items.signoutButtonBackground && Main::items.homeButtonBackground &&
        Main::items.ordersButtonBackground && Main::items.cartButtonBackground && Main::items.deliverysButtonBackground &&
        Main::items.searchBarBackground && Main::items.background){
            Main::items.loginMenu->setProperty("visible", false);
            Main::items.loginMenu->setProperty("enabled", false);

            Main::items.signoutButtonBackground->setProperty("visible", true);
            Main::items.signoutButtonBackground->setProperty("enabled", true);

            Main::items.homeButtonBackground->setProperty("visible", true);
            Main::items.homeButtonBackground->setProperty("enabled", true);

            Main::items.ordersButtonBackground->setProperty("visible", true);
            Main::items.ordersButtonBackground->setProperty("enabled", true);

            Main::items.cartButtonBackground->setProperty("visible", true);
            Main::items.cartButtonBackground->setProperty("enabled", true);

            Main::items.deliverysButtonBackground->setProperty("visible", true);
            Main::items.deliverysButtonBackground->setProperty("enabled", true);

            Main::items.searchBarBackground->setProperty("visible", true);
            Main::items.searchBarBackground->setProperty("enabled", true);

            Main::items.background->setProperty("visible", true);
            Main::items.background->setProperty("enabled", true);
            nlohmann::json messageJson = {
                {"type", "collect"},
                {"amount", "all"}
            };
            std::string message = messageJson.dump();
            ProduceMessage(message, "resterant");
        }else std::cerr<<"login menu not found!!!!!"<<std::endl;
    }
}
void Main::HandleGetResterantResponse(std::string items, std::string type, std::string success){
    std::cerr<<"entered"<<std::endl;
    //std::cerr<<items<<std::endl;
    //if(success.compare("true") == 0){
        nlohmann::json resterants = items;
        int count = 0;
        for(auto i = resterants.begin(); i != resterants.end(); ++i){
            //std::cerr<<i.key()<<std::endl;
            std::cerr<<count<<std::endl;
            count++;
        }
    //}
}
