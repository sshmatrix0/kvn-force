//
// Created by ksp on 10.04.2026.
//

#include "message_server.h"
#include "logger/logger.h"

MessageServer::MessageServer(QObject *parent) : QObject(parent) {
    server = new QLocalServer(this);
    server->removeServer(serverName);
    server->setSocketOptions(QLocalServer::WorldAccessOption);
    if (!server->listen(serverName)) {
        Logger.warn("Server failed to start:" + server->errorString().toStdString());
    } else {
        Logger.info("Started: " + serverName.toStdString());
    }

    connect(server, &QLocalServer::newConnection, this, &MessageServer::onNewConnection);
}

MessageServer::~MessageServer() {
    server->close();
}

void MessageServer::sendMessage(const QByteArray &message) {
    for (QLocalSocket *client: clients) {
        if (client && client->state() == QLocalSocket::ConnectedState) {
            client->write(message + '\n');
            client->flush();
        }
    }
}

void MessageServer::onNewConnection() {
    QLocalSocket *clientSocket = server->nextPendingConnection();
    clients.append(clientSocket);
    connect(clientSocket, &QLocalSocket::readyRead, [clientSocket,this]() {
        auto message = clientSocket->readAll();
        Logger.debug("New client message: " + message.toStdString());
        emit newMessage(clientSocket, message);
    });
    connect(clientSocket, &QLocalSocket::connected, this, &MessageServer::onClientConnected);
    connect(clientSocket, &QLocalSocket::disconnected, this, &MessageServer::onClientDisconnected);
    connect(clientSocket, &QLocalSocket::errorOccurred, this, &MessageServer::onErrorOccurred);
}

void MessageServer::onClientConnected() const {
    Logger.info("Client connected. Num of clients: " + std::to_string(clients.count()));
}

void MessageServer::onClientDisconnected() {
    // Определяем, какой сокет отключился
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (!socket) return;

    // Удаляем из списка
    clients.removeOne(socket);

    // Планируем удаление объекта (безопасно, если в данный момент нет событий)
    socket->deleteLater();
    emit clientDisconnected();
}

void MessageServer::onErrorOccurred() {
    QLocalSocket *socket = qobject_cast<QLocalSocket *>(sender());
    if (!socket) return;
    Logger.warn("Socket error:" + socket->errorString().toStdString());
    // Удаляем из списка и очищаем
    clients.removeOne(socket);
    socket->deleteLater();
    emit clientDisconnected();
}
