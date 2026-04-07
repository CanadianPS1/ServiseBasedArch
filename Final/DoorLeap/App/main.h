#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
class Main : public QObject{
    Q_OBJECT
    public:
    explicit Main(QObject *parent = nullptr);
    Q_INVOKABLE void LogIn(const QString &username, const QString &password);
    Q_INVOKABLE void CreateAccount(const QString &username, const QString &password);
    static void ProduceMessage(const std::string& emailType, const char* topic);
};