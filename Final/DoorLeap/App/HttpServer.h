#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
class HttpServer : public QObject{
    Q_OBJECT
    public:
    explicit HttpServer(QObject *parent = nullptr);
    private slots:
    void handleConnection();
    private:
    QTcpServer* server_;
};