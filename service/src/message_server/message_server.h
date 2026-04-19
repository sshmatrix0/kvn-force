//
// Created by ksp on 10.04.2026.
//
#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSet>

class MessageServer : public QObject {
    Q_OBJECT

public:
    MessageServer(QObject *parent = nullptr);

    ~MessageServer();

    void sendMessage(const QByteArray &message);

private slots:
    void onNewConnection();

    void onClientConnected() const;

    void onClientDisconnected();

    void onErrorOccurred();

signals:
    void newMessage(QLocalSocket *clientSocket, QString message);
    void clientDisconnected();

private:
    QLocalServer *server;
    QString serverName = "KVNForceService";
    QList<QLocalSocket *> clients;
};
