//
// Created by ksp on 29.03.2026.
//

#include "http_client.h"

HttpClient::HttpClient(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            emit finished(data);
        } else {
            emit error(reply->errorString());
        }
        reply->close();
        reply->deleteLater(); // Important to clean up the reply object
    });
}

void HttpClient::get(const QUrl &url) {
    QNetworkRequest request(url);
    manager->get(request);
}
