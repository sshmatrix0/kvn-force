//
// Created by ksp on 25.03.2026.
//

#include "process_runner.h"

#include <qfile.h>

#include "logger/logger.h"

ProcessRunner::ProcessRunner(QString program, QStringList args, bool su, QObject *parent) : QObject(parent),
    program(program),
    args(args), su(su) {
    proccess = new QProcess(this);
    connect(proccess, &QProcess::started, [=]() {
        _started = true;
        emit started();
    });

    connect(proccess, &QProcess::errorOccurred, [=](QProcess::ProcessError error) {
        _failed = true;
        emit failed(ProcessFailedException(
            "Process: " + processErrorToString(error).toStdString() + program.toStdString() + " " + (args.join(" ").
                toStdString())));
    });
    connect(proccess, &QProcess::readyReadStandardOutput, [=]() {
        QByteArray output = proccess->readAllStandardOutput();
        Logger.trace(QString(output).toStdString());
        emit outLog(output);
    });

    connect(proccess, &QProcess::readyReadStandardError, [=]() {
        QByteArray error = proccess->readAllStandardError();
        Logger.trace(QString(error).toStdString());
        emit errorLog(error);
    });
}

ProcessRunner::~ProcessRunner() {
    stop();
}

void ProcessRunner::start(bool withLogs) {
    _failed = false;
    _started = false;
    QFile file(program);
    if (!file.exists()) {
        _failed = true;
        emit failed(ProcessFailedException("File not found: " + program.toStdString()));
        return;
    }

    if (su && QSysInfo::kernelType() == "linux") {
        if (args.isEmpty() || args[0] != program) {
            args.prepend(program);
        }
        proccess->start("sudo", args);
    } else {
        proccess->start(program, args);
    }
}

void ProcessRunner::stop() {
    _started = false;
    _failed = false;
    proccess->terminate();
    if (su && QSysInfo::kernelType() == "linux") {
        QProcess::execute("sudo", QStringList() << "killall" << program);
    }
}
