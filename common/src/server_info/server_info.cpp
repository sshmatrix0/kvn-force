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
    QSharedPointer<QList<VNext> > vnext = QSharedPointer<QList<VNext> >(new QList<VNext>());
    QList<UserInfo> users;
    users.append(userInfo);
    vnext->append(VNext{.address = ip, .port = port, .users = users});
    ServerSettings settings = {
        .vnext = vnext
    };
    bool active = true;

    if (serverJSON.contains("active")) {
        active = serverJSON["active"].toBool();
    }

    auto serverInfo = ServerInfo(serverId, name, settings, streamSettings);
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
    Logger.debug(
        "Server host: " + url.host().toStdString() + ", server port: " + std::to_string(url.port()) + ", " + url.
        userInfo().toStdString());

    QString serverName = url.fragment();
    if (serverName.isEmpty()) {
        serverName = defaultServerNumber;
    }
    QUuid uuid = QUuid::createUuid();
    auto settings = getSettingsFromVlessUrl(url);
    auto streamSettings = getStreamSettingsFromVlessUrl(url);
    ServerInfo serverInfo(uuid.toString(), serverName, settings, streamSettings);
    return serverInfo;
}

ServerSettings ServerInfo::getSettingsFromVlessUrl(const QUrl &url) {
    QSharedPointer<QList<VNext> > vnext = QSharedPointer<QList<VNext> >(new QList<VNext>());
    QList<UserInfo> users;
    UserInfo userInfo = {.id = url.userInfo()};
    users.append(userInfo);
    vnext->append(VNext{.address = url.host(), .port = url.port(), .users = users});
    ServerSettings settings = {
        .vnext = vnext
    };
    return settings;
}

StreamSettings ServerInfo::getStreamSettingsFromVlessUrl(const QUrl &url) {
    QUrlQuery query(url);
    QString network = getStringParameterFromUrlQuery(query, "type");
    QString security = getStringParameterFromUrlQuery(query, "security");
    QSharedPointer<XhttpSettings> xhttpSettings;
    QSharedPointer<GrpcSettings> grpcSettings;
    if (network == "xhttp") {
        xhttpSettings = getXhttpSettingsFromVlessUrl(query);
    } else if (network == "grpc") {
        grpcSettings = getGrpcSettingsFromVlessUrl(query);
    } else {
        throw BadUrlFormatException("Unsupported type: " + network.toStdString());
    }

    QSharedPointer<TlsSettings> tlsSettings;
    QSharedPointer<RealitySettings> realitySettings;
    if (security == "tls") {
        tlsSettings = getTlsSettingsFromVlessUrl(query);
    } else if (security == "reality") {
        realitySettings = getRealitySettingsFromVlessUrl(query);
    } else {
        throw BadUrlFormatException("Unsupported secutity: " + security.toStdString());
    }

    StreamSettings streamSettings = {
        .network = network,
        .security = security,
        .tlsSettings = tlsSettings,
        .realitySettings = realitySettings,
        .grpcSettings = grpcSettings,
        .xhttpSettings = xhttpSettings
    };
    return streamSettings;
}

QSharedPointer<XhttpSettings> ServerInfo::getXhttpSettingsFromVlessUrl(const QUrlQuery &query) {
    QSharedPointer<XhttpSettings> xhttpSettings;
    QString path = getStringParameterFromUrlQuery(query, "path");
    return QSharedPointer<XhttpSettings>(new XhttpSettings{
        .path = path
    });
}

QSharedPointer<GrpcSettings> ServerInfo::getGrpcSettingsFromVlessUrl(const QUrlQuery &query) {
    QSharedPointer<GrpcSettings> grpcSettings;
    QString serviceName = getStringParameterFromUrlQuery(query, "serviceName");
    return QSharedPointer<GrpcSettings>(new GrpcSettings{.serviceName = serviceName});
}

QSharedPointer<TlsSettings> ServerInfo::getTlsSettingsFromVlessUrl(const QUrlQuery &query) {
    QString fingerprint = getStringParameterFromUrlQuery(query, "fp");
    if (fingerprint.isEmpty()) {
        fingerprint = "chrome";
    }
    return QSharedPointer<TlsSettings>(new TlsSettings{.fingerprint = fingerprint});
}

QSharedPointer<RealitySettings> ServerInfo::getRealitySettingsFromVlessUrl(const QUrlQuery &query) {
    QString serverName = getStringParameterFromUrlQuery(query, "sni");
    QString fingerprint = getStringParameterFromUrlQuery(query, "fp");
    if (fingerprint.isEmpty()) {
        fingerprint = "chrome";
    }
    QString publicKey = getStringParameterFromUrlQuery(query, "pbk");
    QString shortId = getStringParameterFromUrlQuery(query, "sid");
    QString spiderX = getStringParameterFromUrlQuery(query, "spx");
    if (spiderX.isEmpty()) {
        spiderX = "/";
    }

    return QSharedPointer<RealitySettings>(new RealitySettings{
        .serverName = serverName,
        .fingerprint = fingerprint,
        .publicKey = publicKey,
        .shortId = shortId,
        .spiderX = spiderX
    });
}

QString ServerInfo::getStringParameterFromUrlQuery(const QUrlQuery &query, const QString &parameterName) {
    return QUrl::fromPercentEncoding(query.queryItemValue(parameterName).toUtf8());
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
        || !streamSettingsJson.contains("security")) {
        throw JsonFormatException("streamSettings bad format");
    }

    QString ssNetwork = streamSettingsJson["network"].toString();
    QString ssSecurity = streamSettingsJson["security"].toString();
    QSharedPointer<XhttpSettings> xhttpSettings;
    QSharedPointer<GrpcSettings> grpcSettings;
    if (ssNetwork == "xhttp") {
        if (!streamSettingsJson.contains("xhttpSettings")) {
            throw JsonFormatException("xhttpSettings undefined");
        }
        QJsonObject xhttpSettingsJson = streamSettingsJson["xhttpSettings"].toObject();
        xhttpSettings = getXhttpSettingsFromJson(xhttpSettingsJson);
    } else if (ssNetwork == "grpc") {
        if (!streamSettingsJson.contains("grpcSettings")) {
            throw JsonFormatException("xhttpSettings undefined");
        }
        QJsonObject grpcSettingsJson = streamSettingsJson["grpcSettings"].toObject();
        grpcSettings = getGrpcSettingsFromJson(grpcSettingsJson);
    } else {
        throw UnsupportedProtocolException("Unsupported protocol: " + ssNetwork.toStdString());
    }

    QSharedPointer<TlsSettings> tlsSettings;
    QSharedPointer<RealitySettings> realitySettings;
    if (ssSecurity == "tls") {
        if (!streamSettingsJson.contains("tlsSettings")) {
            throw JsonFormatException("tlsSettings undefined");
        }
        QJsonObject tlsSettingsJson = streamSettingsJson["tlsSettings"].toObject();
        tlsSettings = getTlsSettingsFromJson(tlsSettingsJson);
    } else if (ssSecurity == "reality") {
        if (!streamSettingsJson.contains("realitySettings")) {
            throw JsonFormatException("realitySettings undefined");
        }
        QJsonObject realitySettingsJson = streamSettingsJson["realitySettings"].toObject();
        realitySettings = getRealitySettingsFromJson(realitySettingsJson);
    }
    return StreamSettings{
        .network = ssNetwork, .security = ssSecurity, .tlsSettings = tlsSettings, .realitySettings = realitySettings,
        .grpcSettings = grpcSettings, .xhttpSettings = xhttpSettings
    };
}

QSharedPointer<XhttpSettings> ServerInfo::getXhttpSettingsFromJson(const QJsonObject &xhttpSettingsJson) {
    if (!xhttpSettingsJson.contains("path")) {
        throw JsonFormatException("xhttp: path undefined");
    }
    if (!xhttpSettingsJson.contains("mode")) {
        throw JsonFormatException("xhttp: mode undefined");
    }
    QString path = xhttpSettingsJson["path"].toString();
    QString mode = xhttpSettingsJson["mode"].toString();
    return QSharedPointer<XhttpSettings>(new XhttpSettings{.path = path, .mode = mode});
}

QSharedPointer<GrpcSettings> ServerInfo::getGrpcSettingsFromJson(const QJsonObject &grpcSettingsJson) {
    if (!grpcSettingsJson.contains("serviceName")) {
        throw JsonFormatException("grpc: serviceName undefined");
    }
    return QSharedPointer<GrpcSettings>(new GrpcSettings{.serviceName = grpcSettingsJson["serviceName"].toString()});
}

QSharedPointer<TlsSettings> ServerInfo::getTlsSettingsFromJson(const QJsonObject &tlsSettingsJson) {
    if (!tlsSettingsJson.contains("fingerprint")) {
        throw JsonFormatException("tls: fingerprint undefined");
    }
    QString fingerprint = tlsSettingsJson["fingerprint"].toString();
    return QSharedPointer<TlsSettings>(new TlsSettings(fingerprint));
}

QSharedPointer<RealitySettings> ServerInfo::getRealitySettingsFromJson(const QJsonObject &realitySettingsJson) {
    if (!realitySettingsJson.contains("serverName")) {
        throw JsonFormatException("reality: serverName undefined");
    }
    QString serverName = realitySettingsJson["serverName"].toString();
    if (!realitySettingsJson.contains("fingerprint")) {
        throw JsonFormatException("reality: fingerprint undefined");
    }
    QString fingerprint = realitySettingsJson["fingerprint"].toString();

    bool show = false;

    if (!realitySettingsJson.contains("publicKey")) {
        throw JsonFormatException("reality: publicKey undefined");
    }
    QString publicKey = realitySettingsJson["publicKey"].toString();

    if (!realitySettingsJson.contains("shortId")) {
        throw JsonFormatException("reality: shortId undefined");
    }
    QString shortId = realitySettingsJson["shortId"].toString();

    if (!realitySettingsJson.contains("spiderX")) {
        throw JsonFormatException("reality: spiderX undefined");
    }
    QString spiderX = realitySettingsJson["spiderX"].toString();

    QString mldsa65Verify = "";

    return QSharedPointer<RealitySettings>(new RealitySettings{
        .serverName = serverName,
        .fingerprint = fingerprint,
        .publicKey = publicKey, .shortId = shortId, .spiderX = spiderX, .mldsa65Verify = mldsa65Verify
    });
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
    vnextItemJson["address"] = settings.vnext->at(0).address;
    vnextItemJson["port"] = settings.vnext->at(0).port;
    QJsonArray users;
    QJsonObject userJson;
    userJson["id"] = settings.vnext->at(0).users[0].id;
    userJson["encryption"] = settings.vnext->at(0).users[0].encryption;
    userJson["security"] = settings.vnext->at(0).users[0].security;
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
    streamSettingsJson["network"] = streamSettings.network;
    streamSettingsJson["security"] = streamSettings.security;
    if (streamSettings.network == "xhttp") {
        streamSettingsJson["xhttpSettings"] = getXhttpSettingsJson();
    }else if (streamSettings.network == "grpc") {
        streamSettingsJson["grpcSettings"] = getGrpcSettingsJson();
    } else {
        throw UnsupportedProtocolException("Unsupported protocol: " + streamSettings.network.toStdString());
    }
    if (streamSettings.tlsSettings != nullptr) {
        streamSettingsJson["tlsSettings"] = getTlsSettingsJson();
    }
    if (streamSettings.realitySettings != nullptr) {
        streamSettingsJson["realitySettings"] = getRealitySettingsJson();
    }
    return streamSettingsJson;
}

QJsonObject ServerInfo::getTlsSettingsJson() const {
    QJsonObject tlsSettingsJson;
    tlsSettingsJson["fingerprint"] = streamSettings.tlsSettings->fingerprint;
    return tlsSettingsJson;
}

QJsonObject ServerInfo::getRealitySettingsJson() const {
    QJsonObject realitySettingsJson;
    realitySettingsJson["serverName"] = streamSettings.realitySettings->serverName;
    realitySettingsJson["fingerprint"] = streamSettings.realitySettings->fingerprint;
    realitySettingsJson["show"] = false;
    realitySettingsJson["publicKey"] = streamSettings.realitySettings->publicKey;
    realitySettingsJson["shortId"] = streamSettings.realitySettings->shortId;
    realitySettingsJson["spiderX"] = streamSettings.realitySettings->spiderX;
    return realitySettingsJson;
}

QJsonObject ServerInfo::getXhttpSettingsJson() const {
    if (streamSettings.xhttpSettings == nullptr) {
        throw JsonFormatException("xhttpSettings undefined for protocol: xhttp");
    }
    QJsonObject xhttpSettingsJson;
    xhttpSettingsJson["path"] = streamSettings.xhttpSettings->path;
    xhttpSettingsJson["mode"] = streamSettings.xhttpSettings->mode;
    return xhttpSettingsJson;
}

QJsonObject ServerInfo::getGrpcSettingsJson() const {
    if (streamSettings.grpcSettings == nullptr) {
        throw JsonFormatException("grpcSettings undefined for protocol: grpc");
    }
    QJsonObject grpcSettingsJson;
    grpcSettingsJson["serviceName"] = streamSettings.grpcSettings->serviceName;
    grpcSettingsJson["multiMode"] = streamSettings.grpcSettings->multiMode;
    grpcSettingsJson["idle_timeout"] = streamSettings.grpcSettings->idleTimeout;
    grpcSettingsJson["health_check_timeout"] = streamSettings.grpcSettings->healthCheckTimeout;
    grpcSettingsJson["permit_without_stream"] = streamSettings.grpcSettings->permitWithoutStream;
    grpcSettingsJson["initial_windows_size"] = streamSettings.grpcSettings->initialWindowsSize;
    return grpcSettingsJson;
}

QString ServerInfo::getId() const {
    return id;
}

QString ServerInfo::getIp() const {
    return settings.vnext->at(0).address;
}

void ServerInfo::setIp(const QString &ip) {
    auto vnext = settings.vnext->at(0);
    vnext.address = ip;
    settings.vnext->replace(0, vnext);
}

int ServerInfo::getPort() const {
    return settings.vnext->at(0).port;
}

void ServerInfo::setPort(int port) {
    auto vnext = settings.vnext->at(0);
    vnext.port = port;
    settings.vnext->replace(0, vnext);
}

QString ServerInfo::getName() const {
    return name;
}

void ServerInfo::setName(const QString &name) {
    this->name = name;
}

UserInfo ServerInfo::getUser() const {
    auto vnext = settings.vnext->at(0);
    auto userInfo = vnext.users[0];
    return userInfo;
}

void ServerInfo::setUser(const UserInfo &userInfo) {
    auto vnext = settings.vnext->at(0);
    vnext.users[0] = userInfo;
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
