#include "HttpServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "nlohmann/json.hpp"
#include "main.h"
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