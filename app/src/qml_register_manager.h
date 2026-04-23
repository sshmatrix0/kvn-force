#pragma  once

#include "system_tray/system_tray.h"
#include <QQmlApplicationEngine>

#include "servers/servers_manager.h"
#include "vpn/vpn_manager.h"

class QMLRegisterManager {
public:
    void initialize() {
        //connect(singboxProcess, &QProcess::errorOccurred,
        QObject::connect(&vpnManager, &VPNManager::sendRequestCurrentServer, &serversManager,
                         &ServersManager::processRequestCurrentServer);
        QObject::connect(&serversManager, &ServersManager::sendResponseCurrentServer, &vpnManager,
                         &VPNManager::getResponseCurrentServer);
        qmlRegisterSingletonInstance("MyTray", 1, 0, "SystemTray", &systemTray);
        qmlRegisterSingletonInstance("ServersManager", 1, 0, "ServersManager", &serversManager);
        qmlRegisterSingletonInstance("VPNManager", 1, 0, "VPNManager", &vpnManager);
        qmlRegisterSingletonInstance("Settings", 1, 0, "Settings", &settings);
        QObject::connect(&vpnManager, &VPNManager::connected, &systemTray, &SystemTray::onVPNConnected);
        QObject::connect(&vpnManager, &VPNManager::disconnected, &systemTray, &SystemTray::onVPNDisconnected);
        QObject::connect(&systemTray, &SystemTray::vpnConnect, &vpnManager, &VPNManager::start);
        QObject::connect(&systemTray, &SystemTray::vpnDisconnect, &vpnManager, &VPNManager::stop);
    }

private:
    SystemTray systemTray;
    ServersManager serversManager;
    VPNManager vpnManager;
    Settings settings;
};
