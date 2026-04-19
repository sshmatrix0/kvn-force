#pragma once
#include <QAbstractListModel>

#include "server_info.h"
#include "settings/settings.h"
//
// Created by ksp on 23.03.2026.
//
class ServersManager : public QAbstractListModel {
    Q_OBJECT

public:
    ServersManager(QObject *parent = nullptr);

    void loadFromSettings();

    Q_INVOKABLE void setNewCurrentServer(QString serverId);
    Q_INVOKABLE void addNewServer(QString vlessUrl);
    Q_INVOKABLE QVariantMap getServerAsMapById(const QString &id);
    Q_INVOKABLE void updateServer(const QString &id, QVariantMap serverAsMap);
    Q_INVOKABLE void deleteServer(const QString &id);
    Q_PROPERTY(QString currentServerIp READ getCurrentServerIp NOTIFY currentServerIpChanged);
    Q_PROPERTY(QString currentServerId READ getCurrentServerId NOTIFY currentServerIdChanged);
    Q_PROPERTY(QString currentServerName READ getCurrentServerName NOTIFY currentServerNameChanged);
    Q_PROPERTY(bool withoutServers READ isEmpty NOTIFY withoutServersChanged);

    QString getCurrentServerIp();
    QString getCurrentServerId();
    QString getCurrentServerName();

public:
    // QAbstractListModel методы
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    bool isEmpty() const;

public:
    enum ServerRoles {
        IdRole = Qt::UserRole + 1,
        IpRole,
        NameRole,
    };

signals:
    void sendResponseCurrentServer(ServerInfo server);

    void currentServerIpChanged(const QString &ip);
    void currentServerIdChanged(const QString &id);
    void currentServerNameChanged(const QString &name);
    void withoutServersChanged(bool withoutServers);

public slots:
    void processRequestCurrentServer();
private:
    ServerInfo emptyServer = ServerInfo("empty","", 0, "empty",{},{});
    ServerInfo getServerById(const QString &id);
    int getServerIndexById(const QString &id);
    QString getDefaultServerNumber();
    void saveServers();
    void updateServer(ServerInfo &serverInfo);
private:
    const QString serversKey = "servers";
    Settings settings;
    QList<ServerInfo> servers;
    QString currentServerId;
};
