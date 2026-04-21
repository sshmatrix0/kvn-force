//
// Created by ksp on 24.03.2026.
//


#include "server_info.h"

#include "logger/logger.h"
#include <QUrlQuery>
#include <QUuid>

#include "exceptions/bad_url_format_exception.h"
#include "exceptions/json_format_exception.h"
#include "exceptions/unsupported_protocol_exception.h"


ServerInfo ServerInfo::fromJson(const QJsonObject &serverJSON) {
    if (serverJSON.isEmpty()) {
        throw JsonFormatException("serverJSON undefined");
    }
    if (!serverJSON.contains("id")) {
        throw JsonFormatException("serverJSON.id undefined");
    }

    if (!serverJSON.contains("settings")) {
        throw JsonFormatException("serverJSON.settings undefined");
    }
    auto settingsJson = serverJSON["settings"].toObject();
    if (!settingsJson.contains("vnext") && !settingsJson["vnex"].isArray()) {
        throw JsonFormatException("vnext undefined");
    }
    auto vNextArrayJson = settingsJson["vnext"].toArray();
    if (vNextArrayJson.isEmpty()) {
        throw JsonFormatException("vNext array is empty");
    }
    auto vNextJson = vNextArrayJson[0].toObject();
    if (!vNextJson.contains("address")) {
        throw JsonFormatException("address undefined");
    }
    if (!vNextJson.contains("port")) {
        throw JsonFormatException("port undefined");
    }


    if (!serverJSON.contains("streamSettings")) {
        throw JsonFormatException("streamSettings undefined");
    }

    QJsonObject streamSettingsJson = serverJSON["streamSettings"].toObject();
    StreamSettings streamSettings = getStreamSettingsFromJson(streamSettingsJson);
    QString serverId = serverJSON["id"].toString();
    QString ip = vNextJson["address"].toString();
    int port = vNextJson["port"].toInt();
    QString name = "-";
    if (serverJSON.contains("name")) {
        name = serverJSON["name"].toString();
    }
    auto userInfo = getUserFromJson(vNextJson);
    bool active = true;

    if (serverJSON.contains("active")) {
        active = serverJSON["active"].toBool();
    }
    auto serverInfo = ServerInfo(serverId, ip, port, name,
                                 userInfo, streamSettings
    );
    serverInfo.setActive(active);
    return serverInfo;
}

ServerInfo ServerInfo::fromVlessUrl(const QString &vlessUrl, const QString &defaultServerNumber) {
    Logger.info("Try to add new server: " + vlessUrl.toStdString());
    auto url = QUrl(vlessUrl);
    if (!url.isValid()) {
        throw BadUrlFormatException("Bad server url: " + vlessUrl.toStdString());
    }
    if (url.host().isEmpty() || url.port() == 0 || vlessUrl.isEmpty()) {
        throw BadUrlFormatException("Bad server url: " + vlessUrl.toStdString());
    }
    Logger.info(
        "Server host: " + url.host().toStdString() + ", server port: " + std::to_string(url.port()) + " " + url.
        userInfo().toStdString());

    UserInfo userInfo = {.id = url.userInfo()};
    QUrlQuery query(url);
    StreamSettings streamSettings = {
        .xhttpSettings = QSharedPointer<XhttpSettings>(new XhttpSettings{
            .path = QUrl::fromPercentEncoding(query.queryItemValue("path").toUtf8())
        })
    };
    QString serverName = url.fragment();
    if (serverName.isEmpty()) {
        serverName = defaultServerNumber;
    }
    QUuid uuid = QUuid::createUuid();
    ServerInfo serverInfo(uuid.toString(), url.host(), url.port(), serverName, userInfo, streamSettings);
    return serverInfo;
}

UserInfo ServerInfo::getUserFromJson(const QJsonObject &vNextJson) {
    if (!vNextJson.contains("users") && vNextJson["users"].isArray()) {
        throw JsonFormatException("users undefined");
    }
    QJsonArray usersJsonArray = vNextJson["users"].toArray();
    if (usersJsonArray.isEmpty()) {
        throw JsonFormatException("users array is empty");
    }
    auto userJson = usersJsonArray[0].toObject();
    if (!userJson.contains("id")) {
        throw JsonFormatException("users id undefined");
    }
    if (!userJson.contains("encryption")) {
        throw JsonFormatException("users encryption undefined");
    }
    QString userId = userJson["id"].toString();
    QString encryption = userJson["encryption"].toString();
    QString security = userJson["security"].toString();
    if (security.isEmpty()) {
        return UserInfo{.id = userId, .encryption = encryption,};
    }
    return UserInfo{.id = userId, .security = security, .encryption = encryption};
}

StreamSettings ServerInfo::getStreamSettingsFromJson(const QJsonObject &streamSettingsJson) {
    if (!streamSettingsJson.contains("network")
        || !streamSettingsJson.contains("security")
        || !streamSettingsJson.contains("tlsSettings")) {
        throw JsonFormatException("streamSettings bad format");
    }
    QJsonObject tlsSettingsJson = streamSettingsJson["tlsSettings"].toObject();
    if (!tlsSettingsJson.contains("fingerprint")) {
        throw JsonFormatException("fingerprint undefined");
    }


    QString ssNetwork = streamSettingsJson["network"].toString();
    QString ssSecurity = streamSettingsJson["security"].toString();
    QString fingerprint = tlsSettingsJson["fingerprint"].toString();
    if (ssNetwork == "xhttp") {
        QJsonObject xhttpSettingsJson = streamSettingsJson["xhttpSettings"].toObject();
        if (!xhttpSettingsJson.contains("path")) {
            throw JsonFormatException("path undefined");
        }
        if (!xhttpSettingsJson.contains("mode")) {
            throw JsonFormatException("mode undefined");
        }
        QString path = xhttpSettingsJson["path"].toString();
        QString mode = xhttpSettingsJson["mode"].toString();
        return {
            .network = ssNetwork, .security = ssSecurity,
            .tlsSettings = TlsSettings{fingerprint},
            .xhttpSettings = QSharedPointer<XhttpSettings>(new XhttpSettings{.path = path, .mode = mode})
        };
    } else {
        throw UnsupportedProtocolException("Unsupported protocol: " + ssNetwork.toStdString());
    }
}

QJsonObject ServerInfo::toJson(bool withMetaData) const {
    QJsonObject serverJSON;
    if (withMetaData) {
        serverJSON["id"] = getId();
        serverJSON["active"] = active;
        serverJSON["name"] = name;
    }
    serverJSON["tag"] = "proxy";
    serverJSON["protocol"] = "vless";
    QJsonArray vnextJsonArray;
    QJsonObject vnextItemJson;
    vnextItemJson["address"] = ip;
    vnextItemJson["port"] = port;
    QJsonArray users;
    QJsonObject userJson;
    userJson["id"] = userInfo.id;
    userJson["encryption"] = userInfo.encryption;
    userJson["security"] = userInfo.security;
    users.append(userJson);
    vnextItemJson["users"] = users;
    vnextJsonArray.append(vnextItemJson);
    QJsonObject settingsJson;
    settingsJson["vnext"] = vnextJsonArray;
    serverJSON["settings"] = settingsJson;
    QJsonObject streamSettingsJson = getStreamSettingsJson();
    serverJSON["streamSettings"] = streamSettingsJson;
    QJsonObject sockoptJson;
    sockoptJson["mark"] = 255;
    serverJSON["sockopt"] = sockoptJson;
    return serverJSON;
}

QJsonObject ServerInfo::getStreamSettingsJson() const {
    QJsonObject streamSettingsJson;
    if (streamSettings.network == "xhttp") {
        if (streamSettings.xhttpSettings == nullptr) {
            throw JsonFormatException("xhttpSettings undefined for protocol: xhttp");
        }
        streamSettingsJson["network"] = streamSettings.network;
        streamSettingsJson["security"] = streamSettings.security;
        QJsonObject tlsSettingsJson;
        tlsSettingsJson["fingerprint"] = streamSettings.tlsSettings.fingerprint;
        streamSettingsJson["tlsSettings"] = tlsSettingsJson;
        QJsonObject xhttpSettingsJson;
        xhttpSettingsJson["path"] = streamSettings.xhttpSettings->path;
        xhttpSettingsJson["mode"] = streamSettings.xhttpSettings->mode;
        streamSettingsJson["xhttpSettings"] = xhttpSettingsJson;
        return streamSettingsJson;
    } else {
        throw UnsupportedProtocolException("Unsupported protocol: " + streamSettings.network.toStdString());
    }
}

QString ServerInfo::getId() const {
    return id;
}

QString ServerInfo::getIp() const {
    return ip;
}

void ServerInfo::setIp(const QString &ip) {
    this->ip = ip;
}

int ServerInfo::getPort() const {
    return port;
}

void ServerInfo::setPort(int port) {
    this->port = port;
}

QString ServerInfo::getName() const {
    return name;
}

void ServerInfo::setName(const QString &name) {
    this->name = name;
}

UserInfo ServerInfo::getUser() const {
    return userInfo;
}

void ServerInfo::setUser(const UserInfo &userInfo) {
    this->userInfo = userInfo;
}

StreamSettings ServerInfo::getStreamSettings() const {
    return streamSettings;
}

void ServerInfo::setStreamSettings(const StreamSettings &streamSettings) {
    this->streamSettings = streamSettings;
}

QSharedPointer<ServerInfo> ServerInfo::clone() const {
    return QSharedPointer<ServerInfo>(new ServerInfo(*this));
}

bool ServerInfo::isActive() const {
    return active;
}

void ServerInfo::setActive(bool active) {
    this->active = active;
}
