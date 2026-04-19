#include "vpn_linux.h"
#include "utils/http_client/http_client.h"

VPNLinux::VPNLinux(QObject *parent) : AbstractVPN(parent, "VPNLinux") {
    singBoxBinPath = QDir::cleanPath(appBinPath + "/linux_amd64" + "/" + "sing-box");
    xrayBinPath = QDir::cleanPath(appBinPath + "/linux_amd64" + "/" + "xray");
    singBoxRunner =
            QSharedPointer<ProcessRunner>(
                new ProcessRunner(singBoxBinPath
                                  , QStringList() << "run" << "-c" << singBoxConfigPath, true));
    xrayRunner = QSharedPointer<ProcessRunner>(
        new ProcessRunner(xrayBinPath, QStringList() << "run" << "-c" << xrayConfigPath));
    connect(xrayRunner.data(), &ProcessRunner::started, this, &VPNLinux::xrayStarted);
    connect(xrayRunner.data(), &ProcessRunner::failed, this, &VPNLinux::xrayFailed);
    connect(singBoxRunner.data(), &ProcessRunner::started, this, &VPNLinux::singBoxStarted);
    connect(singBoxRunner.data(), &ProcessRunner::failed, this, &VPNLinux::singBoxFailed);

    connect(this, &VPNLinux::configGenerated, this, &VPNLinux::processConfigGenerated);
}

void VPNLinux::start(ServerInfo server) {
    connectionState = ConnectionState::CONNECTING;
    emit connecting();
    this->server = server.clone();
    BaseQThread::start();
}

void VPNLinux::stop() {
    Logger.info("Stopping VPN Linux");
    singBoxRunner->stop();
    xrayRunner->stop();
    connectionState = ConnectionState::DISCONNECTED;
    emit disconnected();
}

void VPNLinux::runMethod() {
    AbstractVPN::start(*server.data());
}

void VPNLinux::xrayStarted() {
    Logger.info("Xray started");
    Logger.info("Starting sing-box");
    singBoxRunner->start(true);
}

void VPNLinux::xrayFailed(const ProcessFailedException &ex) {
    Logger.error("xray ", ex);
    stopAndEmitFailed();
}

void VPNLinux::singBoxStarted() {
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

void VPNLinux::singBoxFailed(ProcessFailedException ex) {
    Logger.error("sing-box ", ex);
    stopAndEmitFailed();
}

void VPNLinux::processConfigGenerated() {
    try {
        Logger.info("Starting xray");
        xrayRunner->start(true);
    } catch (std::exception &ex) {
        xrayFailed(ProcessFailedException(ex.what()));
    }
}

void VPNLinux::stopAndEmitFailed() {
    xrayRunner->stop();
    singBoxRunner->stop();
    connectionState = ConnectionState::FAILED;
    emit failed();
}
