//
// Created by ksp on 13.04.2026.
//

#include "vpn_desktop_service.h"

#include "utils/enum_util.h"

VPNDesktopService::VPNDesktopService(QObject *parent) {
    socket = new QLocalSocket(this);
    socket->connectToServer(serverName);
    messageServerConnectTimer = new QTimer(this);
    connect(messageServerConnectTimer, &QTimer::timeout, this, &VPNDesktopService::reconnectToMessageServer);
    connect(socket, &QLocalSocket::connected, this, &VPNDesktopService::onMessageServerConnected);
    connect(socket, &QLocalSocket::disconnected, this, &VPNDesktopService::onMessageServerDisconnected);
    connect(socket, &QLocalSocket::errorOccurred, this, &VPNDesktopService::onMessageServerError);
    connect(socket, &QLocalSocket::readyRead, this, &VPNDesktopService::onMessageServerReadyRead);
    connect(this, &VPNDesktopService::jsonReceived, this, &VPNDesktopService::processJsonRecieved);
    messageServerConnectTimer->start(RECONNECTION_TIMEOUT);
}

void VPNDesktopService::start(ServerInfo server) {
    connectionState = ConnectionState::CONNECTING;
    auto serverJson = server.toJson();
    QJsonObject json;
    json["type"] = EnumUtil::toString(MessageType::START).toLower();
    json["server"] = serverJson;
    QJsonDocument doc(json);
    socket->write(doc.toJson());
    socket->flush();
}

void VPNDesktopService::stop() {
    Logger.info("Stopping service");
    QJsonObject json;
    json["type"] = EnumUtil::toString(MessageType::STOP).toLower();
    QJsonDocument doc(json);
    socket->write(doc.toJson());
    socket->flush();
}

void VPNDesktopService::runMethod() {
    Logger.warn("Unimplemented");
}

void VPNDesktopService::onMessageServerConnected() {
    Logger.debug("Connected to service");
    if (messageServerConnectTimer->isActive()) {
        messageServerConnectTimer->stop();
    }
}

void VPNDesktopService::onMessageServerDisconnected() {
    Logger.debug("Disconnected to service.");
    if (!messageServerConnectTimer->isActive()) {
        messageServerConnectTimer->start();
    }
}

void VPNDesktopService::onMessageServerError() {
    Logger.warn("Service error: " + socket->errorString().toStdString());
    emit failed();
    connectionState = ConnectionState::DISCONNECTED;
    emit disconnected();
    if (!messageServerConnectTimer->isActive()) {
        messageServerConnectTimer->start(RECONNECTION_TIMEOUT);
    }
}

void VPNDesktopService::onMessageServerReadyRead() {
    // Добавляем новые данные в буфер
    buffer.append(socket->readAll());

    // Разбираем буфер по разделителю '\n'
    while (true) {
        int newlinePos = buffer.indexOf('\n');
        if (newlinePos == -1)
            break; // Нет сообщения

        // Извлекаем одно сообщение (без '\n')
        QByteArray message = buffer.left(newlinePos);
        buffer.remove(0, newlinePos + 1);

        // Пропускаем пустые строки
        if (message.trimmed().isEmpty())
            continue;
        try {
            auto json = parseJSON(message);
            emit jsonReceived(json);
        } catch (std::exception &ex) {
            Logger.error("Error parse message", ex);
        }
    }
}

void VPNDesktopService::processJsonRecieved(const QJsonObject &json)
{
    try {
        auto type = getMessageType(json);
        switch (type) {
        case MessageType::CONNECTING: {
            Logger.debug("connecting");
            connectionState = ConnectionState::CONNECTING;
            emit connecting();
            break;
        }
        case MessageType::CONNECTED: {
            Logger.debug("connected");
            connectionState = ConnectionState::CONNECTED;
            emit connected();
            break;
        }
        case MessageType::DISCONNECTED: {
            Logger.debug("disconnected");
            connectionState = ConnectionState::DISCONNECTED;
            emit disconnected();
            break;
        }
        case MessageType::FAILED: {
            Logger.debug("failes");
            connectionState = ConnectionState::FAILED;
            emit failed();
            break;
        }
        }
    } catch (std::exception &ex) {
        Logger.error("Error", ex);
    }
}

void VPNDesktopService::processConfigGenerated() {
    Logger.warn("Not implemented");
}

void VPNDesktopService::reconnectToMessageServer() {
    if (socket->state() == QLocalSocket::UnconnectedState) {
        Logger.warn("Try to reconnect to service");
        socket->connectToServer(serverName);
    }
}

QJsonObject VPNDesktopService::parseJSON(const QString &message) {
    QJsonParseError parseError;
    auto message1 = message.trimmed();
    QJsonDocument doc = QJsonDocument::fromJson(message1.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        throw JsonFormatException("Response from service should be as Object: "
                                  + message.toStdString()
                                  + ", ex: " + parseError.errorString().toStdString());
    }
    return doc.object();
}

MessageType VPNDesktopService::getMessageType(const QJsonObject &json) {
    QString typeString = json["type"].toString().toUpper();
    return EnumUtil::fromString<MessageType>(typeString);
}
