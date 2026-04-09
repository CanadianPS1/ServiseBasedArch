#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
class Main : public QObject{
    Q_OBJECT
    public:
    struct QtItems{
        QObject* loginMenu;
        QObject* signoutButtonBackground;
        QObject* homeButtonBackground;
        QObject* ordersButtonBackground;
        QObject* deliverysButtonBackground;
        QObject* cartButtonBackground;
        QObject* searchBarBackground;
        QObject* background;
    };
    static QtItems items;
    explicit Main(QObject *parent = nullptr);
    Q_INVOKABLE void LogIn(const QString &username, const QString &password);
    Q_INVOKABLE void CreateAccount(const QString &username, const QString &password);
    static void ProduceMessage(const std::string& emailType, const char* topic);
    static void HandleLogInResponse(std::string type, std::string success, std::string message, std::string username, std::string authToken);
};