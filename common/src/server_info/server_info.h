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

struct TlsSettings {
    QString fingerprint;
};

struct StreamSettings {
    QString network = "xhttp";
    QString security = "tls";
    TlsSettings tlsSettings = {.fingerprint = "chrome"};
    QSharedPointer<XhttpSettings> xhttpSettings;
};

class ServerInfo {
public:
    ServerInfo(const QString id, const QString ip, int port, QString name, UserInfo userInfo,
               StreamSettings streamSettings, bool active = false) : id(id), ip(ip), name(name), port(port),
                                                                     userInfo(std::move(userInfo)),
                                                                     streamSettings(std::move(streamSettings)),
                                                                     active(active) {
    }

    bool operator==(const ServerInfo &other) const {
        return id == other.id;
    }

    static ServerInfo fromJson(const QJsonObject &obj);

    static ServerInfo fromVlessUrl(const QString &vlessUrl, const QString &defaultServerNumber);

    QJsonObject toJson(bool withMetaData = true) const;

    QJsonObject getStreamSettingsJson() const;

protected:
    static UserInfo getUserFromJson(const QJsonObject &vNextJson);

    static StreamSettings getStreamSettingsFromJson(const QJsonObject &streamSettingsJson);

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
    QString ip;
    int port;
    QString name;
    UserInfo userInfo;
    StreamSettings streamSettings;
    bool active;
};
