#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
class HttpServer : public QObject{
    Q_OBJECT
    public:
    void Register(const QString& name, const QString& id, const QString& address, int port);
    explicit HttpServer(QObject *parent = nullptr);
    private slots:
    void handleConnection();
    private:
    QTcpServer* server_;
};