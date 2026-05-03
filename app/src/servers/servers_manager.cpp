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
                        currentServerNetworkChanged(server.getStreamSettings().network);
                        currentServerSecurityChanged(server.getStreamSettings().security);
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
        currentServerNetworkChanged(servers[0].getStreamSettings().network);
        currentServerSecurityChanged(servers[0].getStreamSettings().security);
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
        currentServerNetworkChanged(server.getStreamSettings().network);
        currentServerSecurityChanged(server.getStreamSettings().security);
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

QString ServersManager::getCurrentServerNetwork() {
    auto server = getServerById(currentServerId);
    return server.getStreamSettings().network;
}

QString ServersManager::getCurrentServerSecurity() {
    auto server = getServerById(currentServerId);
    return server.getStreamSettings().security;
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
        case NetworkRole:
            return server.getStreamSettings().network;
        case SecurityRole:
            return server.getStreamSettings().security;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> ServersManager::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[IpRole] = "ip";
    roles[NameRole] = "name";
    roles[NetworkRole] = "network";
    roles[SecurityRole] = "security";
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
    serverAsMap["network"] = server.getStreamSettings().network;
    serverAsMap["security"] = server.getStreamSettings().security;
    if (serverAsMap["network"] == "xhttp") {
        serverAsMap["xHttpPath"] = server.getStreamSettings().xhttpSettings->path;
    } else if (serverAsMap["network"] == "xhttp") {
        serverAsMap["grpcServiceName"] = server.getStreamSettings().grpcSettings->serviceName;
    }
    if (serverAsMap["security"] == "tls") {
        serverAsMap["tlsFingerprint"] = server.getStreamSettings().tlsSettings->fingerprint;
    } else if (serverAsMap["security"] == "reality") {
        serverAsMap["realityServerName"] = server.getStreamSettings().realitySettings->serverName;
        serverAsMap["realityFingerprint"] = server.getStreamSettings().realitySettings->fingerprint;
        serverAsMap["realityPublicKey"] = server.getStreamSettings().realitySettings->publicKey;
        serverAsMap["realityShortId"] = server.getStreamSettings().realitySettings->shortId;
        serverAsMap["realitySpiderX"] = server.getStreamSettings().realitySettings->spiderX;
    }
    return serverAsMap;
}

void ServersManager::updateServer(const QString &id, QVariantMap serverAsMap) {
    auto server = getServerById(id);
    server.setIp(serverAsMap["ip"].toString());
    server.setName(serverAsMap["name"].toString());
    server.setPort(serverAsMap["port"].toInt());
    auto lastNetwork = server.getStreamSettings().network;
    auto lastSecurity = server.getStreamSettings().security;
    auto newNetwork = serverAsMap["network"].toString();
    auto newSecurity = serverAsMap["security"].toString();
    if (lastNetwork != newNetwork) {
        if (lastNetwork == "xhttp") {
            server.getStreamSettings().xhttpSettings = nullptr;
        } else if (lastNetwork == "grpc") {
            server.getStreamSettings().grpcSettings = nullptr;
        }
    }
    if (lastSecurity != newSecurity) {
        if (lastSecurity == "tls") {
            server.getStreamSettings().tlsSettings = nullptr;
        } else if (lastSecurity == "reality") {
            server.getStreamSettings().realitySettings = nullptr;
        }
    }
    auto streamSettings = server.getStreamSettings();
    if (newNetwork == "xhttp") {
        auto xHttpPath = serverAsMap["xHttpPath"].toString();
        if (streamSettings.xhttpSettings != nullptr) {
            streamSettings.xhttpSettings->path = xHttpPath;
        } else {
            streamSettings.xhttpSettings = QSharedPointer<XhttpSettings>(new XhttpSettings{.path = xHttpPath});
        }
    } else if (newNetwork == "grpc") {
        if (streamSettings.grpcSettings != nullptr) {
            streamSettings.grpcSettings->serviceName = serverAsMap["grpcServiceName"].toString();
        } else {
            streamSettings.grpcSettings = QSharedPointer<GrpcSettings>(new GrpcSettings{
                .serviceName = serverAsMap["grpcServiceName"].toString()
            });
        }
    }
    if (newSecurity == "tls") {
        if (streamSettings.tlsSettings != nullptr) {
            streamSettings.tlsSettings->fingerprint = serverAsMap["tlsFingerprint"].toString();
        } else {
            streamSettings.tlsSettings = QSharedPointer<TlsSettings>(new TlsSettings{
                .fingerprint = serverAsMap["tlsFingerprint"].toString()
            });
        }
    } else if (newSecurity == "reality") {
        if (streamSettings.realitySettings != nullptr) {
            streamSettings.realitySettings->fingerprint = serverAsMap["realityFingerprint"].toString();
            streamSettings.realitySettings->serverName = serverAsMap["realityServerName"].toString();
            streamSettings.realitySettings->publicKey = serverAsMap["realityPublicKey"].toString();
            streamSettings.realitySettings->shortId = serverAsMap["realityShortId"].toString();
            streamSettings.realitySettings->spiderX = serverAsMap["realitySpiderX"].toString();
        }else {
            streamSettings.realitySettings = QSharedPointer<RealitySettings>(new RealitySettings{
                .serverName = serverAsMap["realityServerName"].toString(),
                .fingerprint = serverAsMap["realityFingerprint"].toString(),
                .publicKey = serverAsMap["realityPublicKey"].toString(),
                .shortId = serverAsMap["realityShortId"].toString(),
                .spiderX = serverAsMap["realitySpiderX"].toString()
            });
        }
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
