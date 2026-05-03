//
// Created by ksp on 24.03.2026.
//
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTest>

#include "logger/logger.h"
#include "server_info/server_info.h"

class ServerTest : public QObject {
    Q_OBJECT

private slots:
    void fromJsonXhttpTls1();

    void toJsonXhttptls1();

    void fromJsonGrpcReality1();

    void toJsonGrpcReality1();
};

void ServerTest::fromJsonXhttpTls1() {
    Logger.setLogLevel(LogLevel::TRACE);

    QString jsonString =
            R"({"id":"1", "tag": "proxy", "protocol": "vless", "settings": { "vnext": [ { "address": "213.119.43.300", "port": 443, "users": [ { "id": "06fb17bf-32c6-4d2d-92a2-e052dcd26780", "encryption": "none" } ] } ] }, "streamSettings": { "network": "xhttp", "security": "tls", "tlsSettings": { "fingerprint": "chrome" }, "xhttpSettings": { "path": "/repos/octocat/Spoon-Knife/issues", "mode": "auto" } }, "sockopt": { "mark": 255 } })";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    QVERIFY(parseError.error == QJsonParseError::NoError);

    try {
        auto server = ServerInfo::fromJson(doc.object());
        QVERIFY(server.getId() == "1");
        QVERIFY(server.getIp() == "213.119.43.300");
        QVERIFY(server.getPort() == 443);
        QVERIFY(server.getUser().id == "06fb17bf-32c6-4d2d-92a2-e052dcd26780");
        QVERIFY(server.getUser().encryption == "none");
        QVERIFY(server.getStreamSettings().network=="xhttp");
        QVERIFY(server.getStreamSettings().security=="tls");
        QVERIFY(server.getStreamSettings().tlsSettings->fingerprint=="chrome");
        QVERIFY(server.getStreamSettings().xhttpSettings->path=="/repos/octocat/Spoon-Knife/issues");
        QVERIFY(server.getStreamSettings().xhttpSettings->mode=="auto");
    } catch (std::exception &ex) {
        Logger.error("Error: ", ex);
        throw ex;
    }
}

void ServerTest::toJsonXhttptls1() {
    QString jsonString =
            R"({"id":"1", "tag": "proxy", "protocol": "vless", "settings": { "vnext": [ { "address": "213.119.43.244", "port": 443, "users": [ { "id": "06fb17bf-32c6-4d2d-92a2-e052dcd26780", "encryption": "none" } ] } ] }, "streamSettings": { "network": "xhttp", "security": "tls", "tlsSettings": { "fingerprint": "chrome" }, "xhttpSettings": { "path": "/repos/octocat/Spoon-Knife/issues", "mode": "auto" } }, "sockopt": { "mark": 255 } })";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    QVERIFY(parseError.error == QJsonParseError::NoError);
    auto server = ServerInfo::fromJson(doc.object());
    auto serverInfoJson = server.toJson();
    QVERIFY(!serverInfoJson.isEmpty());
    QVERIFY(serverInfoJson["id"].toString() == "1");
    QVERIFY(serverInfoJson["tag"].toString() == "proxy");
    QVERIFY(serverInfoJson["protocol"].toString() == "vless");
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["address"].toString() ==
        "213.119.43.244");
    QVERIFY(serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["port"].toInt() == 443);
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["users"].toArray()[0].toObject()["id"].
        toString() == "06fb17bf-32c6-4d2d-92a2-e052dcd26780");
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["users"].toArray()[0].toObject()[
            "encryption"].toString() == "none");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["network"] == "xhttp");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["security"] == "tls");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["tlsSettings"].toObject()["fingerprint"] == "chrome");
    QVERIFY(
        serverInfoJson["streamSettings"].toObject()["xhttpSettings"].toObject()["path"] ==
        "/repos/octocat/Spoon-Knife/issues");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["xhttpSettings"].toObject()["mode"] == "auto");
    QVERIFY(serverInfoJson["sockopt"].toObject()["mark"].toInt() == 255);
}

void ServerTest::fromJsonGrpcReality1() {
    Logger.setLogLevel(LogLevel::TRACE);

    QString jsonString =
            R"({"id":"2", "tag": "proxy", "protocol": "vless", "settings": { "vnext": [ { "address": "83.152.8.300", "port": 54211, "users": [ { "id": "581b0223-7791-4a99-aba2-0d194b78051c", "email": "t@t.tt", "security": "auto", "encryption": "none" } ] } ] }, "streamSettings": { "network": "grpc", "security": "reality", "realitySettings": { "serverName": "www.intel.com", "fingerprint": "chrome", "show": false, "publicKey": "WMNUjlId9G2szFJ5GXuR_oYjCC3JWHOMEGEo1JfEL1k", "shortId": "cd06d2ee60b69ed9", "spiderX": "/", "mldsa65Verify": "" }, "grpcSettings": { "serviceName": "", "multiMode": false, "idle_timeout": 60, "health_check_timeout": 20, "permit_without_stream": false, "initial_windows_size": 0 } }})";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    QVERIFY(parseError.error == QJsonParseError::NoError);

    try {
        auto server = ServerInfo::fromJson(doc.object());
        QVERIFY(server.getId() == "2");
        QVERIFY(server.getIp() == "83.152.8.300");
        QVERIFY(server.getPort() == 54211);
        QVERIFY(server.getUser().id == "581b0223-7791-4a99-aba2-0d194b78051c");
        QVERIFY(server.getUser().encryption == "none");
        QVERIFY(server.getStreamSettings().network=="grpc");
        QVERIFY(server.getStreamSettings().security=="reality");
        QVERIFY(server.getStreamSettings().grpcSettings != nullptr);
        QVERIFY(server.getStreamSettings().grpcSettings->serviceName=="");
        QVERIFY(server.getStreamSettings().realitySettings != nullptr);
        QVERIFY(server.getStreamSettings().realitySettings->serverName=="www.intel.com");
        QVERIFY(server.getStreamSettings().realitySettings->fingerprint=="chrome");
        QVERIFY(server.getStreamSettings().realitySettings->publicKey=="WMNUjlId9G2szFJ5GXuR_oYjCC3JWHOMEGEo1JfEL1k");
        QVERIFY(server.getStreamSettings().realitySettings->shortId=="cd06d2ee60b69ed9");
        QVERIFY(server.getStreamSettings().realitySettings->spiderX=="/");
        QVERIFY(server.getStreamSettings().realitySettings->mldsa65Verify=="");
    } catch (std::exception &ex) {
        Logger.error("Error: ", ex);
        throw ex;
    }
}

void ServerTest::toJsonGrpcReality1() {
        QString jsonString =
            R"({"id":"2", "tag": "proxy", "protocol": "vless", "settings": { "vnext": [ { "address": "83.152.8.300", "port": 54211, "users": [ { "id": "581b0223-7791-4a99-aba2-0d194b78051c", "email": "t@t.tt", "security": "auto", "encryption": "none" } ] } ] }, "streamSettings": { "network": "grpc", "security": "reality", "realitySettings": { "serverName": "www.intel.com", "fingerprint": "chrome", "show": false, "publicKey": "WMNUjlId9G2szFJ5GXuR_oYjCC3JWHOMEGEo1JfEL1k", "shortId": "cd06d2ee60b69ed9", "spiderX": "/", "mldsa65Verify": "" }, "grpcSettings": { "serviceName": "", "multiMode": false, "idle_timeout": 60, "health_check_timeout": 20, "permit_without_stream": false, "initial_windows_size": 0 }}})";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    QVERIFY(parseError.error == QJsonParseError::NoError);
    auto server = ServerInfo::fromJson(doc.object());
    auto serverInfoJson = server.toJson();
    QVERIFY(!serverInfoJson.isEmpty());
    QVERIFY(serverInfoJson["id"].toString() == "2");
    QVERIFY(serverInfoJson["tag"].toString() == "proxy");
    QVERIFY(serverInfoJson["protocol"].toString() == "vless");
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["address"].toString() ==
        "83.152.8.300");
    QVERIFY(serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["port"].toInt() == 54211);
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["users"].toArray()[0].toObject()["id"].
        toString() == "581b0223-7791-4a99-aba2-0d194b78051c");
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["users"].toArray()[0].toObject()[
            "encryption"].toString() == "none");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["network"] == "grpc");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["grpcSettings"].toObject()["serviceName"] == "");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["grpcSettings"].toObject()["multiMode"] == false);
    QVERIFY(serverInfoJson["streamSettings"].toObject()["grpcSettings"].toObject()["idle_timeout"] == 60);
    QVERIFY(serverInfoJson["streamSettings"].toObject()["grpcSettings"].toObject()["health_check_timeout"] == 20);
    QVERIFY(serverInfoJson["streamSettings"].toObject()["grpcSettings"].toObject()["permit_without_stream"] == false);
    QVERIFY(serverInfoJson["streamSettings"].toObject()["grpcSettings"].toObject()["initial_windows_size"] == 0);

    QVERIFY(serverInfoJson["streamSettings"].toObject()["realitySettings"].toObject()["serverName"] == "www.intel.com");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["realitySettings"].toObject()["fingerprint"] == "chrome");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["realitySettings"].toObject()["publicKey"] == "WMNUjlId9G2szFJ5GXuR_oYjCC3JWHOMEGEo1JfEL1k");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["realitySettings"].toObject()["shortId"] == "cd06d2ee60b69ed9");
    QVERIFY(serverInfoJson["streamSettings"].toObject()["realitySettings"].toObject()["spiderX"] == "/");
    //QVERIFY(serverInfoJson["streamSettings"].toObject()["realitySettings"].toObject()["mldsa65Verify"] == "");
}

QTEST_MAIN(ServerTest)
#include "server_test.moc"
