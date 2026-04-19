//
// Created by ksp on 25.03.2026.
//
#pragma once
#include <QProcess>

#include "exceptions/process_failed_exception.h"

class ProcessRunner : public QObject {
    Q_OBJECT

public:
    ProcessRunner(QString program, QStringList args, bool su = false, QObject *parent = nullptr);

    void start(bool withLogs = false);

    [[nodiscard]] bool isStarted() const {
        return _started;
    };
    [[nodiscard]] bool isFailed() const {
        return _failed;
    };

    void stop();

    ~ProcessRunner();

signals:
    void started();

    void failed(ProcessFailedException ex);

    void outLog(QString log);

    void errorLog(QString log);

private:
    QProcess *proccess;
    QStringList args;
    QString program;
    bool su = false;
    bool _started = false;
    bool _failed = false;

protected:
    static QString processErrorToString(QProcess::ProcessError error) {
        switch (error) {
            case QProcess::FailedToStart:
                return QStringLiteral("Failed to start");
            case QProcess::Crashed:
                return QStringLiteral("Crashed");
            case QProcess::Timedout:
                return QStringLiteral("Timed out");
            case QProcess::WriteError:
                return QStringLiteral("Write error");
            case QProcess::ReadError:
                return QStringLiteral("Read error");
            case QProcess::UnknownError:
            default:
                return QStringLiteral("Unknown error");
        }
    }
};
