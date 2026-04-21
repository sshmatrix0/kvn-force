//
// Created by ksp on 13.04.2026.
//
#pragma once
#include <QLocalSocket>
#include "message_type.h"
#include <QTimer>
#include "server_info/server_info.h"
#include "vpn/abstract_vpn.h"

class VPNDesktopService : public AbstractVPN {
    Q_OBJECT

public:
    VPNDesktopService(QObject *parent = nullptr);

    void start(ServerInfo server) override;

    void stop() override;
    void runMethod() override;

private slots:
    void onMessageServerConnected();
    void onMessageServerDisconnected();
    void onMessageServerError();
    void processJsonRecieved(const QJsonObject &json);

    void onMessageServerReadyRead();
    void processConfigGenerated() override;
    void reconnectToMessageServer();
private:
    QJsonObject parseJSON(const QString &message);

    MessageType getMessageType(const QJsonObject &json);

private:
    QLocalSocket *socket;
    QByteArray buffer;
    QString serverName = "KVNForceService";
    QTimer *messageServerConnectTimer;
    int RECONNECTION_TIMEOUT = 2000;
signals:
    void jsonReceived(const QJsonObject &json);
};
