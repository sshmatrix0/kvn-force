//
// Created by ksp on 24.03.2026.
//
#pragma once
#include <QJsonArray>
#include <QJsonObject>
#include <utility>

struct UserInfo {
    QString id;
    QString security = "auto";
    QString encryption = "none";
};

struct XhttpSettings {
    QString path;
    QString mode = "auto";
};

struct RealitySettings {
    QString serverName;
    QString fingerprint = "chrome";
    QString publicKey;
    QString shortId;
    QString spiderX = "/";
    QString mldsa65Verify = "";
};

struct GrpcSettings {
    QString serviceName;
    bool multiMode = false;
    int idleTimeout = 60;
    int healthCheckTimeout = 20;
    bool permitWithoutStream = false;
    int initialWindowsSize = 0;
};

struct TlsSettings {
    QString fingerprint;
};

struct VNext {
    QString address;
    int port;
    QList<UserInfo> users;
};

struct ServerSettings {
    QSharedPointer<QList<VNext> > vnext;
};

struct StreamSettings {
    QString network;
    QString security;
    QSharedPointer<TlsSettings> tlsSettings;
    QSharedPointer<RealitySettings> realitySettings;
    QSharedPointer<GrpcSettings> grpcSettings;
    QSharedPointer<XhttpSettings> xhttpSettings;
};

class ServerInfo {
public:
    ServerInfo(const QString id, QString name, ServerSettings settings,
               StreamSettings streamSettings, bool active = false) : id(id), name(name), settings(settings),
                                                                     streamSettings(std::move(streamSettings)),
                                                                     active(active) {
    }

    bool operator==(const ServerInfo &other) const {
        return id == other.id;
    }

    static ServerInfo fromJson(const QJsonObject &obj);

    static ServerInfo fromVlessUrl(const QString &vlessUrl, const QString &defaultServerNumber);

    static ServerSettings getSettingsFromVlessUrl(const QUrl &url);

    static StreamSettings getStreamSettingsFromVlessUrl(const QUrl &url);

    static QSharedPointer<XhttpSettings> getXhttpSettingsFromVlessUrl(const QUrlQuery &query);

    static QSharedPointer<GrpcSettings> getGrpcSettingsFromVlessUrl(const QUrlQuery &query);

    static QSharedPointer<TlsSettings> getTlsSettingsFromVlessUrl(const QUrlQuery &query);

    static QSharedPointer<RealitySettings> getRealitySettingsFromVlessUrl(const QUrlQuery &query);

    static QString getStringParameterFromUrlQuery(const QUrlQuery &query, const QString &parameterName);

    QJsonObject toJson(bool withMetaData = true) const;

    QJsonObject getStreamSettingsJson() const;

    QJsonObject getXhttpSettingsJson() const;

    QJsonObject getGrpcSettingsJson() const;

    QJsonObject getTlsSettingsJson() const;

    QJsonObject getRealitySettingsJson() const;

protected:
    static UserInfo getUserFromJson(const QJsonObject &vNextJson);

    static StreamSettings getStreamSettingsFromJson(const QJsonObject &streamSettingsJson);

    static QSharedPointer<XhttpSettings> getXhttpSettingsFromJson(const QJsonObject &xhttpSettingsJson);

    static QSharedPointer<GrpcSettings> getGrpcSettingsFromJson(const QJsonObject &grpcSettingsJson);

    static QSharedPointer<TlsSettings> getTlsSettingsFromJson(const QJsonObject &tlsSettingsJson);

    static QSharedPointer<RealitySettings> getRealitySettingsFromJson(const QJsonObject &realitySettingsJson);

public:
    QString getId() const;

    QString getIp() const;

    void setIp(const QString &ip);

    int getPort() const;

    void setPort(int port);

    QString getName() const;

    void setName(const QString &name);

    UserInfo getUser() const;

    void setUser(const UserInfo &userInfo);

    StreamSettings getStreamSettings() const;

    void setStreamSettings(const StreamSettings &streamSettings);

    QSharedPointer<ServerInfo> clone() const;

    bool isActive() const;

    void setActive(bool active);

private:
    QString id;
    QString name;
    ServerSettings settings;
    StreamSettings streamSettings;
    bool active;
};
