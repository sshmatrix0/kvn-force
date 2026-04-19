//
// Created by ksp on 23.03.2026.
//

#include "servers_manager.h"

#include "exceptions/wrong_args_exception.h"
#include "logger/logger.h"
#include <QQmlEngine>
#include <QLocalSocket>
ServersManager::ServersManager(QObject *parent) : QAbstractListModel(parent) {
    loadFromSettings();
}

void ServersManager::loadFromSettings() {
    beginResetModel();
    servers.clear();
    QByteArray savedData = settings.get("servers").toByteArray();
    bool foundDefault = false;
    if (!savedData.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(savedData);
        if (doc.isArray()) {
            QJsonArray serversArray = doc.array();
            for (const auto &value: serversArray) {
                if (value.isObject()) {
                    ServerInfo server = ServerInfo::fromJson(value.toObject());
                    if (server.isActive()) {
                        foundDefault = true;
                        currentServerId = server.getId();
                        currentServerIpChanged(server.getIp());
                        currentServerIdChanged(server.getId());
                        currentServerNameChanged(server.getName());
                    }
                    servers.append(server);
                }
            }
        }
    }
    if (!foundDefault && !servers.isEmpty()) {
        currentServerId = servers[0].getId();
        currentServerIpChanged(servers[0].getIp());
        currentServerIdChanged(servers[0].getId());
        currentServerNameChanged(servers[0].getName());
    }
    endResetModel();
}

void ServersManager::setNewCurrentServer(QString serverId) {
    try {
        auto server = getServerById(serverId);
        for (auto &serverInfo: servers) {
            if (serverInfo.getId() == server.getId()) {
                serverInfo.setActive(true);
            } else {
                serverInfo.setActive(false);
            }
        }
        saveServers();
        currentServerId = server.getId();
        currentServerIpChanged(server.getIp());
        currentServerIdChanged(server.getId());
        currentServerNameChanged(server.getName());
    } catch (const WrongArgsException &e) {
        Logger.error("Can't set server with id: " + serverId.toStdString(), e);
    }
}

void ServersManager::addNewServer(QString vlessUrl) {
    try {
        vlessUrl = vlessUrl.trimmed();
        auto serverInfo = ServerInfo::fromVlessUrl(vlessUrl, getDefaultServerNumber());
        beginResetModel();
        if (servers.isEmpty()) {
            serverInfo.setActive(true);
            servers.append(serverInfo);
            setNewCurrentServer(serverInfo.getId());
            emit withoutServersChanged(false);
        } else {
            servers.append(serverInfo);
            emit withoutServersChanged(false);
        }
        saveServers();
        endResetModel();
    } catch (std::exception &e) {
        qmlEngine(this)->throwError(tr("Exception: %1").arg(e.what()));
    }
}

QString ServersManager::getCurrentServerIp() {
    auto server = getServerById(currentServerId);
    return server.getIp();
}

QString ServersManager::getCurrentServerId() {
    return currentServerId;
}

QString ServersManager::getCurrentServerName() {
    auto server = getServerById(currentServerId);
    return server.getName();
}

int ServersManager::rowCount(const QModelIndex &parent) const {
    return servers.size();
}

QVariant ServersManager::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= servers.size())return QVariant();
    const ServerInfo &server = servers[index.row()];
    switch (role) {
        case IdRole:
            return server.getId();
        case IpRole:
            return server.getIp();
        case NameRole:
            return server.getName();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ServersManager::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[IpRole] = "ip";
    roles[NameRole] = "name";
    return roles;
}

bool ServersManager::isEmpty() const {
    return servers.empty();
}

void ServersManager::processRequestCurrentServer() {
    auto server = getServerById(currentServerId);
    emit sendResponseCurrentServer(server);
}

ServerInfo ServersManager::getServerById(const QString &id) {
    for (auto &server: servers) {
        if (server.getId() == id) {
            return server;
        }
    }
    qmlEngine(this)->throwError(tr("Server id not found: %1").arg(id));
    return emptyServer;
}

int ServersManager::getServerIndexById(const QString &id) {
    for (int i = 0; i < servers.size(); i++) {
        if (servers[i].getId() == id) {
            return i;
        }
    }
    qmlEngine(this)->throwError(tr("Server id not found: %1").arg(id));
    return -1;
}

QVariantMap ServersManager::getServerAsMapById(const QString &id) {
    auto server = getServerById(id);
    QVariantMap serverAsMap;
    serverAsMap["id"] = server.getId();
    serverAsMap["ip"] = server.getIp();
    serverAsMap["port"] = server.getPort();
    serverAsMap["name"] = server.getName();
    serverAsMap["xHttpPath"] = server.getStreamSettings().xhttpSettings->path;
    return serverAsMap;
}

void ServersManager::updateServer(const QString &id, QVariantMap serverAsMap) {
    auto server = getServerById(id);
    server.setIp(serverAsMap["ip"].toString());
    auto xHttpPath = serverAsMap["xHttpPath"].toString();
    server.setName(serverAsMap["name"].toString());
    server.setPort(serverAsMap["port"].toInt());
    auto streamSettings = server.getStreamSettings();
    if (streamSettings.xhttpSettings != nullptr) {
        streamSettings.xhttpSettings->path = xHttpPath;
    } else {
        Logger.warn("Can't save xHTTP path. Unsupported transport protocol");
    }
    server.setStreamSettings(streamSettings);
    updateServer(server);
    saveServers();
}

void ServersManager::deleteServer(const QString &id) {
    beginResetModel();
    auto indexForDelete = getServerIndexById(id);
    servers.removeAt(indexForDelete);
    endResetModel();
    saveServers();
    if (servers.isEmpty()) {
        emit withoutServersChanged(true);
    }
}

QString ServersManager::getDefaultServerNumber() {
    int nextNumber = servers.size() + 1;
    bool foundNext = false;
    while (!foundNext) {
        for (auto &server: servers) {
            if (server.getName() == QString::number(nextNumber)) {
                nextNumber++;
                break;
            }
        }
        foundNext = true;
    }
    return QString::number(nextNumber);
}

void ServersManager::saveServers() {
    QJsonArray serversArray;
    for (const auto &server: servers) {
        serversArray.append(server.toJson());
    }
    QJsonDocument doc(serversArray);
    settings.set("servers", doc.toJson());
}

void ServersManager::updateServer(ServerInfo &serverInfo) {
    for (auto i = 0; i < servers.size(); i++) {
        if (servers[i].getId() == serverInfo.getId()) {
            servers[i] = serverInfo;
        }
    }
}
