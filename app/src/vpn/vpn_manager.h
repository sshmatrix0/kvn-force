//
// Created by ksp on 25.03.2026.
//
#pragma once
#include <QObject>

#include "abstract_vpn.h"
#include "vpn_factory.h"
#include "logger/logger.h"
#include "utils/enum_util.h"

class VPNManager : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QString connectionState READ getConnectionState NOTIFY connectionStateChanged)

    VPNManager(QObject *parent = nullptr) : QObject(parent) {
        vpn = VPNFactory::createVPN();
        connect(vpn.data(), &AbstractVPN::connecting, this, [=] {
            emit connectionStateChanged(getConnectionState());
        });
        connect(vpn.data(), &AbstractVPN::connected, this, [=] {
            emit connected();
            emit connectionStateChanged(getConnectionState());
        });
        connect(vpn.data(), &AbstractVPN::disconnected, this, [=] {
            emit disconnected();
            emit connectionStateChanged(getConnectionState());
        });
        connect(vpn.data(), &AbstractVPN::failed, this, [=] {
            emit failed();
            emit connectionStateChanged(getConnectionState());
        });
    }

    QString getConnectionState() {
        return EnumUtil::toString<ConnectionState>(vpn->getConnectionState());
    }

signals:
    void sendRequestCurrentServer();

    void connected();

    void disconnected();

    void failed();

    void connectionStateChanged(QString state);

public slots:
    void start() {
        emit sendRequestCurrentServer();
    }

    void stop() {
        vpn->stop();
    }

    void getResponseCurrentServer(ServerInfo server);

private:
    QSharedPointer<AbstractVPN> vpn;
};

inline void VPNManager::getResponseCurrentServer(ServerInfo server) {
    vpn->start(server);
    Logger.info("VPNManager::getResponseCurrentServer: " + server.getIp().toStdString());
}
