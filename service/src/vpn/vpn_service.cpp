#include "vpn_service.h"
#include "utils/http_client/http_client.h"

VPNService::VPNService(QObject *parent) : AbstractVPN(parent, "VPNService") {
    #ifdef Q_OS_WIN
      singBoxBinPath = QDir::cleanPath(appBinPath + "/windows_amd64" + "/" + "sing-box.exe");
      xrayBinPath = QDir::cleanPath(appBinPath + "/windows_amd64" + "/" + "xray.exe");
    #elif defined(Q_OS_LINUX)
      singBoxBinPath = QDir::cleanPath(appBinPath + "/linux_amd64" + "/" + "sing-box");
      xrayBinPath = QDir::cleanPath(appBinPath + "/linux_amd64" + "/" + "xray");
    #endif
    singBoxRunner =
            QSharedPointer<ProcessRunner>(
                new ProcessRunner(singBoxBinPath
                                  , QStringList() << "run" << "-c" << singBoxConfigPath));
    xrayRunner = QSharedPointer<ProcessRunner>(
        new ProcessRunner(xrayBinPath, QStringList() << "run" << "-c" << xrayConfigPath));
    connect(xrayRunner.data(), &ProcessRunner::started, this, &VPNService::xrayStarted);
    connect(xrayRunner.data(), &ProcessRunner::failed, this, &VPNService::xrayFailed);
    connect(singBoxRunner.data(), &ProcessRunner::started, this, &VPNService::singBoxStarted);
    connect(singBoxRunner.data(), &ProcessRunner::failed, this, &VPNService::singBoxFailed);

    connect(this, &VPNService::configGenerated, this, &VPNService::processConfigGenerated);
}

void VPNService::start(ServerInfo server) {
    connectionState = ConnectionState::CONNECTING;
    emit connecting();
    this->server = server.clone();
    BaseQThread::start();
}

void VPNService::stop() {
    Logger.info("Stopping VPN Service");
    singBoxRunner->stop();
    xrayRunner->stop();
    connectionState = ConnectionState::DISCONNECTED;
    emit disconnected();
}

void VPNService::runMethod() {
    AbstractVPN::start(*server.data());
}

void VPNService::xrayStarted() {
    Logger.info("Xray started");
    Logger.info("Starting sing-box");
    singBoxRunner->start(true);
}

void VPNService::xrayFailed(const ProcessFailedException &ex) {
    Logger.error("xray ", ex);
    stopAndEmitFailed();
}

void VPNService::singBoxStarted() {
    Logger.info("Sing-box started");
    httpClient = QSharedPointer<HttpClient>(new HttpClient());
    connect(httpClient.data(), &HttpClient::finished, [this] {
        connectionState = ConnectionState::CONNECTED;
        emit connected();
        disconnect(httpClient.data(), nullptr, this, nullptr);
    });
    connect(httpClient.data(), &HttpClient::error, [this] {
        stopAndEmitFailed();
        disconnect(httpClient.data(), nullptr, this, nullptr);
    });
    httpClient->get(QUrl(URL_FOR_CHECK));
}

void VPNService::singBoxFailed(ProcessFailedException ex) {
    Logger.error("sing-box ", ex);
    stopAndEmitFailed();
}

void VPNService::processConfigGenerated() {
    try {
        Logger.info("Starting xray");
        xrayRunner->start(true);
    } catch (std::exception &ex) {
        xrayFailed(ProcessFailedException(ex.what()));
    }
}

void VPNService::stopAndEmitFailed() {
    xrayRunner->stop();
    singBoxRunner->stop();
    connectionState = ConnectionState::FAILED;
    emit failed();
}
