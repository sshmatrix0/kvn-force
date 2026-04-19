//
// Created by ksp on 29.03.2026.
//
#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);
    void get(const QUrl &url);

signals:
    void finished(const QByteArray &response);
    void error(const QString &errorString);

private slots:
    //void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};
