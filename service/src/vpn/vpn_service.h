//
// Created by ksp on 25.03.2026.
//
#pragma once

#include "logger/logger.h"
#include "utils/process_runner.h"
#include "utils/http_client/http_client.h"
#include "vpn/abstract_vpn.h"

class VPNService : public AbstractVPN {
    Q_OBJECT
public:
    VPNService(QObject *parent = nullptr);

    void start(ServerInfo server) override;
    void start(ServerInfo server, QString routeByDefault, QList<QString> domainsForProxy,
                       QList<QString> domainsForDirect, QList<QString> processNamesForProxy,
                       QList<QString> processNamesForDirect, QList<QString> ruleSetsForProxy) override;

    void stop() override;
    void runMethod() override;
private slots:
    void xrayStarted();
    void xrayFailed(const ProcessFailedException& ex);
    void singBoxStarted();
    void singBoxFailed(ProcessFailedException ex);
    void processConfigGenerated() override;
private:
    void stopAndEmitFailed();
private:
    QSharedPointer<ProcessRunner> singBoxRunner;
    QSharedPointer<ProcessRunner> xrayRunner;
    QSharedPointer<ServerInfo> server;
    QString routeByDefault = "proxy";
    QList<QString> domainsForProxy;
    QList<QString> domainsForDirect;
    QList<QString> processNamesForProxy;
    QList<QString> processNamesForDirect;
    QList<QString> ruleSetsForProxy;
    QSharedPointer<HttpClient> httpClient;
};

