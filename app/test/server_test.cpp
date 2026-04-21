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
    void fromJson();

    void toJson();
};

void ServerTest::fromJson() {
    Logger.setLogLevel(LogLevel::TRACE);

    QString jsonString =
            R"({"id":"1", "tag": "proxy", "protocol": "vless", "settings": { "vnext": [ { "address": "212.119.43.244", "port": 443, "users": [ { "id": "06fb17bf-32c6-4d2d-92a2-e052dcd26780", "encryption": "none" } ] } ] }, "streamSettings": { "network": "xhttp", "security": "tls", "tlsSettings": { "fingerprint": "chrome" }, "xhttpSettings": { "path": "/repos/octocat/Spoon-Knife/issues", "mode": "auto" } }, "sockopt": { "mark": 255 } })";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    QVERIFY(parseError.error == QJsonParseError::NoError);

    try {
        auto server = ServerInfo::fromJson(doc.object());
        QVERIFY(server.getId() == "1");
        QVERIFY(server.getIp() == "212.119.43.244");
        QVERIFY(server.getPort() == 443);
        QVERIFY(server.getUser().id == "06fb17bf-32c6-4d2d-92a2-e052dcd26780");
        QVERIFY(server.getUser().encryption == "none");
        QVERIFY(server.getStreamSettings().network=="xhttp");
        QVERIFY(server.getStreamSettings().security=="tls");
        QVERIFY(server.getStreamSettings().tlsSettings.fingerprint=="chrome");
        QVERIFY(server.getStreamSettings().xhttpSettings->path=="/repos/octocat/Spoon-Knife/issues");
        QVERIFY(server.getStreamSettings().xhttpSettings->mode=="auto");
    } catch (std::exception &ex) {
        Logger.error("Error: ", ex);
        throw ex;
    }
}

void ServerTest::toJson() {
    QString jsonString =
            R"({"id":"1", "tag": "proxy", "protocol": "vless", "settings": { "vnext": [ { "address": "212.119.43.244", "port": 443, "users": [ { "id": "06fb17bf-32c6-4d2d-92a2-e052dcd26780", "encryption": "none" } ] } ] }, "streamSettings": { "network": "xhttp", "security": "tls", "tlsSettings": { "fingerprint": "chrome" }, "xhttpSettings": { "path": "/repos/octocat/Spoon-Knife/issues", "mode": "auto" } }, "sockopt": { "mark": 255 } })";
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    QVERIFY(parseError.error == QJsonParseError::NoError);

    auto serverInfoJson = doc.object();
    QVERIFY(!serverInfoJson.isEmpty());
    QVERIFY(serverInfoJson["id"].toString() == "1");
    QVERIFY(serverInfoJson["tag"].toString() == "proxy");
    QVERIFY(serverInfoJson["protocol"].toString() == "vless");
    QVERIFY(
        serverInfoJson["settings"].toObject()["vnext"].toArray()[0].toObject()["address"].toString() ==
        "212.119.43.244");
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

QTEST_MAIN(ServerTest)
#include "server_test.moc"
