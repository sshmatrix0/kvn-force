//
// Created by ksp on 10.04.2026.
//
#pragma once
#include "../message_server/message_type.h"
#include "vpn_service.h"
#include "message_server/message_server.h"

class VPNServiceManager : public QObject {
    Q_OBJECT

public:
    VPNServiceManager(QObject *parent = nullptr);

public slots:
    void processNewMessage(QLocalSocket *clientSocket, QString message);
    void processClientOfMessageServerDisconnected();

    void processConnecting();

    void processConnected();

    void processFailed();

    void processDisconnected();

private:
    QJsonObject parseJSON(const QString &message);

    MessageType getMessageType(const QJsonObject &json);

    void sendSimpleMessageToClients(MessageType type);

    void processStartMessage(QJsonObject json);
    void processStopMessage();

private:
    MessageServer *messageServer;
    VPNService *vpnService;
};
