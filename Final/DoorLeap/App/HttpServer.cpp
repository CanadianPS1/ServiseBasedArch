#include "HttpServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "nlohmann/json.hpp"
#include "main.h"
#include <iostream>
HttpServer::HttpServer(QObject* parent) : QObject(parent){
    server_ = new QTcpServer(this);
    connect(server_, &QTcpServer::newConnection, this, &HttpServer::handleConnection);
    if(!server_->listen(QHostAddress::Any, 9258)) qDebug()<<"Server failed to start!";
    else qDebug()<<"Server listening on port 9258";
}
void HttpServer::handleConnection(){
    QTcpSocket* socket = server_->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, [socket]() {
        QByteArray request = socket->readAll();
        qDebug() << "Request:" << request;
        QList<QByteArray> lines = request.split('\n');
        QByteArray requestLine = lines[0].trimmed();
        QList<QByteArray> parts = requestLine.split(' ');
        if(parts.size() < 2) return;
        QByteArray method = parts[0];
        QByteArray path   = parts[1];
        QByteArray responseBody;
        if(method == "POST" && path == "/logins"){
            QByteArray body = request.mid(request.indexOf("\r\n\r\n") + 4);
            nlohmann::json requestJson = nlohmann::json::parse(body.toStdString());
            std::string type = requestJson["type"];
            std::string success = requestJson["success"];
            std::string message = requestJson["message"];
            std::string username = requestJson["username"];
            std::string authToken = requestJson["jwtToken"];
            nlohmann::json responseJson = {
                {"status","did it big"}
            };
            responseBody = QByteArray::fromStdString(responseJson.dump());
            Main::HandleLogInResponse(type, success, message, username, authToken);
        }else if(method == "POST" && path == "/resterants"){ 
            std::cerr<<"found end point"<<std::endl; 
            int headerEnd = request.indexOf("\r\n\r\n"); 
            if(headerEnd == -1){ 
                std::cerr<<"no header"<<std::endl; 
                return; 
            } 
            QByteArray body = request.mid(headerEnd + 4); 
            if(body.isEmpty()){ 
                std::cerr<<"no body"<<std::endl; 
                return; 
            } 
            std::string bodyStr = body.toStdString(); 
            nlohmann::json requestJson; 
            try{requestJson = nlohmann::json::parse(bodyStr);} 
            catch(const std::exception& e){ 
                std::cerr<<"failed to parse json"<<std::endl; 
                return; 
            } 
            std::cerr<<"parsed json"<<std::endl; 
            std::string type = requestJson["type"]; 
            std::string success = requestJson["success"]; 
            std::string items = requestJson["items"]; 
            std::cerr<<"fetched items"<<std::endl; 
            nlohmann::json responseJson = { {"status", "did it big"} }; 
            responseBody = QByteArray::fromStdString(responseJson.dump()); 
            std::cerr<<"bruh"<<std::endl; 
            Main::HandleGetResterantResponse(items, type, success);
        }else{
            nlohmann::json err = {
                {"error","endpoint not found"}
            };
            responseBody = QByteArray::fromStdString(err.dump());
        }
        QByteArray response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + QByteArray::number(responseBody.size()) + "\r\n"
            "\r\n" + responseBody;
        socket->write(response);
        socket->flush();
        socket->disconnectFromHost();
    });
}