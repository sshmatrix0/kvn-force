//
// Created by ksp on 10.04.2026.
//

#include "vpn_service_manager.h"
#include "utils/enum_util.h"

VPNServiceManager::VPNServiceManager(QObject *parent) : QObject(parent) {
    messageServer = new MessageServer(this);
    vpnService = new VPNService(this);
    connect(messageServer, &MessageServer::newMessage, this, &VPNServiceManager::processNewMessage);
    connect(messageServer, &MessageServer::clientDisconnected, this, &VPNServiceManager::processClientOfMessageServerDisconnected);
    connect(vpnService, &AbstractVPN::connecting, this, &VPNServiceManager::processConnecting);
    connect(vpnService, &AbstractVPN::connected, this, &VPNServiceManager::processConnected);
    connect(vpnService, &AbstractVPN::disconnected, this, &VPNServiceManager::processDisconnected);
}

void VPNServiceManager::processNewMessage(QLocalSocket *clientSocket, QString message) {
    try {
        auto json = parseJSON(message);
        auto type = getMessageType(json);
        switch (type) {
            case MessageType::START: {
                processStartMessage(json);
                break;
            }
            case MessageType::STOP: {
                processStopMessage();
                break;
            }
        }
    } catch (std::exception &ex) {
        Logger.error("Error processing message '", ex);
    }
}

void VPNServiceManager::processClientOfMessageServerDisconnected() {
    vpnService->stop();
}

void VPNServiceManager::processConnecting() {
    Logger.debug("send connecting");
    sendSimpleMessageToClients(MessageType::CONNECTING);
}

void VPNServiceManager::processConnected() {
    Logger.debug("send connected");
    sendSimpleMessageToClients(MessageType::CONNECTED);
}

void VPNServiceManager::processFailed() {
    Logger.debug("send failed");
    sendSimpleMessageToClients(MessageType::FAILED);
}

void VPNServiceManager::processDisconnected() {
    Logger.debug("send disconnected");
    sendSimpleMessageToClients(MessageType::DISCONNECTED);
}


QJsonObject VPNServiceManager::parseJSON(const QString &message) {
    QJsonObject jsonObject;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        throw JsonFormatException("Request from GUI should be as Object");
    }
    return doc.object();
}

MessageType VPNServiceManager::getMessageType(const QJsonObject &json) {
    QString typeString = json["type"].toString().toUpper();
    return EnumUtil::fromString<MessageType>(typeString);
}

void VPNServiceManager::sendSimpleMessageToClients(MessageType type) {
    QJsonObject connectingJson;
    connectingJson["type"] = EnumUtil::toString<MessageType>(type).toLower();
    QJsonDocument doc(connectingJson);
    messageServer->sendMessage(doc.toJson(QJsonDocument::Compact));
}

void VPNServiceManager::processStartMessage(QJsonObject json) {
    QJsonValue serverValue = json["server"];
    if (!json.contains("server") || !serverValue.isObject() || serverValue.isNull()) {
        throw JsonFormatException("Missing server property");
    }
    auto server = ServerInfo::fromJson(serverValue.toObject());
    vpnService->start(server);
}

void VPNServiceManager::processStopMessage() {
    vpnService->stop();
}
