#include "HttpServer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
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
        QByteArray response ="HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"status\":\"ok\"}";
        socket->write(response);
        socket->flush();
        socket->disconnectFromHost();
    });
}