//
// Created by ksp on 25.03.2026.
//
#pragma once
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QMutex>

#include "exceptions/copy_config_exception.h"
#include "exceptions/create_dir_exception.h"
#include "exceptions/delete_dir_exception.h"
#include "exceptions/json_format_exception.h"
#include "logger/logger.h"
#include "server_info/server_info.h"
#include "utils/qthread/base_thread.h"

enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    FAILED
};

class AbstractVPN : public BaseQThread {
    Q_OBJECT

protected:
    AbstractVPN(QObject *parent = nullptr, QString name = "AbstractVPN") : BaseQThread(parent, name) {
        appPath = QCoreApplication::applicationDirPath();
        appBinPath = QDir::cleanPath(appPath + "/" + "bin");
        QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        configPath = QDir::cleanPath(cachePath + "/" + "config");
        singBoxConfigPath = QDir::cleanPath(configPath + "/" + "config_singbox.json");
        xrayConfigPath = QDir::cleanPath(configPath + "/" + "config_xray.json");
    }

    virtual ~AbstractVPN() = default;

public:
    virtual void start(ServerInfo server) {
        genConfigs(server, "proxy", QList<QString>(), QList<QString>(), QList<QString>(), QList<QString>());
    };

    virtual void start(ServerInfo server, QString routeByDefault, QList<QString> domainsForProxy,
                       QList<QString> domainsForDirect, QList<QString> processNamesForProxy,
                       QList<QString> processNamesForDirect) {
        genConfigs(server, routeByDefault, domainsForProxy, domainsForDirect, processNamesForProxy,
                   processNamesForDirect);
    };

    virtual void stop() = 0;

    [[nodiscard]] ConnectionState getConnectionState() const {
        return connectionState;
    }

signals:
    void configGenerated();

    void connected();

    void connecting();

    void failed();

    void disconnected();

public slots:
    virtual void processConfigGenerated() = 0;

protected:
    QString appPath;
    QString appBinPath;
    QString singBoxBinPath;
    QString xrayBinPath;
    QString configPath;
    QString singBoxConfigPath;
    QString xrayConfigPath;
    QMutex mutex;
    ConnectionState connectionState = ConnectionState::DISCONNECTED;
    QString URL_FOR_CHECK = "https://google.com";

    void genConfigs(ServerInfo server, QString routeByDefault, QList<QString> domainsForProxy,
                    QList<QString> domainsForDirect, QList<QString> processNamesForProxy,
                    QList<QString> processNamesForDirect) {
        Logger.trace("Try to recreate app configs dir...");
        auto configPath = reCreateConfigsDir();
        Logger.debug("Recreated app configs dir: " + configPath.toStdString());
        copySingBoxConfig(configPath, routeByDefault, domainsForProxy, domainsForDirect, processNamesForProxy,
                          processNamesForDirect);
        copyXRayConfig(server, configPath);
        emit configGenerated();
    }

    QString reCreateConfigsDir() {
        QDir configDir(configPath);
        if (configDir.exists()) {
            if (!configDir.removeRecursively()) {
                throw DeleteDirException(configPath.toStdString());
            }
        }
        if (!configDir.mkpath(configPath)) {
            throw CreateDirException(configPath.toStdString());
        }
        return configPath;
    }

    void copySingBoxConfig(QString configPath, QString routeByDefault, QList<QString> domainsForProxy,
                           QList<QString> domainsForDirect, QList<QString> processNamesForProxy,
                           QList<QString> processNamesForDirect) {
        Logger.trace("Copy sing-box config to path: " + configPath.toStdString());
        QFile singBoxConfigFile(":/res/configs/config_singbox.json");
        singBoxConfigFile.open(QFile::ReadOnly);
        auto qba = singBoxConfigFile.readAll();
        QString jsonString = QString(qba);
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            throw JsonFormatException("Parse error: " + jsonString.toStdString());
        }
        auto configJson = doc.object();
        if (configJson.isEmpty() || !configJson.contains("route") || !configJson["route"].isObject()) {
            throw JsonFormatException("Missing outbounds config on not array");
        }
        auto routeJson = configJson["route"].toObject();
        if (!routeJson.contains("rules") || !routeJson["rules"].isArray()) {
            throw JsonFormatException("Missing outbounds config on not array");
        }
        auto rules = routeJson["rules"].toArray();

        if (!domainsForProxy.isEmpty()) {
            QJsonObject domainsForProxyJson;
            domainsForProxyJson["domain_suffix"] = convert2JsonArray(domainsForProxy);
            domainsForProxyJson["outbound"] = "proxy";
            rules.append(domainsForProxyJson);
        }
        if (!domainsForDirect.isEmpty()) {
            QJsonObject domainsForDirectJson;
            domainsForDirectJson["domain_suffix"] = convert2JsonArray(domainsForDirect);
            domainsForDirectJson["outbound"] = "direct";
            rules.append(domainsForDirectJson);
        }
        if (!processNamesForProxy.isEmpty()) {
            QJsonObject processNamesForProxyJson;
            processNamesForProxyJson["process_name"] = convert2JsonArray(processNamesForProxy);
            processNamesForProxyJson["outbound"] = "proxy";
            rules.append(processNamesForProxyJson);
        }
        if (!processNamesForDirect.isEmpty()) {
            QJsonObject processNamesForDirectJson;
            processNamesForDirectJson["process_name"] = convert2JsonArray(processNamesForDirect);
            processNamesForDirectJson["outbound"] = "direct";
            rules.append(processNamesForDirectJson);
        }
        routeJson["rules"] = rules;
        routeJson["final"] = routeByDefault;
        configJson["route"] = routeJson;
        QFile destFile(singBoxConfigPath);
        if (!destFile.open(QFile::WriteOnly)) {
            throw CopyConfigException("Error open file: " + configPath.toStdString());
        }
        QJsonDocument destDoc(configJson);
        destFile.write(destDoc.toJson());
    }

    QJsonArray convert2JsonArray(QList<QString> list) {
        QJsonArray array;
        for (QString item: list) {
            array.append(QJsonValue(item));
        }
        return array;
    }

    void copyXRayConfig(ServerInfo server, QString configPath) {
        QFile xRayConfigFile(":/res/configs/config_xray.json");
        xRayConfigFile.open(QFile::ReadOnly);
        auto qba = xRayConfigFile.readAll();
        QString jsonString = QString(qba);
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            throw JsonFormatException("Parse error: " + jsonString.toStdString());
        }
        auto configJson = doc.object();
        auto serverJson = server.toJson(false);
        if (configJson.isEmpty() || !configJson.contains("outbounds") || !configJson["outbounds"].isArray()) {
            throw JsonFormatException("Missing outbounds config on not array");
        }
        auto outbounds = configJson["outbounds"].toArray();
        outbounds.prepend(serverJson);
        configJson["outbounds"] = outbounds;
        QFile destFile(xrayConfigPath);
        if (!destFile.open(QFile::WriteOnly)) {
            throw CopyConfigException("Error open file: " + configPath.toStdString());
        }
        QJsonDocument destDoc(configJson);
        destFile.write(destDoc.toJson());
    }
};
