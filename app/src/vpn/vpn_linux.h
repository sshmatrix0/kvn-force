//
// Created by ksp on 25.03.2026.
//
#pragma once

#include "abstract_vpn.h"
#include "logger/logger.h"
#include "utils/process_runner.h"
#include "utils/http_client/http_client.h"

class VPNLinux : public AbstractVPN {
    Q_OBJECT
public:
    VPNLinux(QObject *parent = nullptr);

    void start(ServerInfo server) override;

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
    QSharedPointer<HttpClient> httpClient;
};

