//
// Created by ksp on 25.03.2026.
//

#include "process_runner.h"

#include <qfile.h>

#include "logger/logger.h"
#ifdef Q_OS_WIN

#include "windows.h"

#endif
ProcessRunner::ProcessRunner(QString program, QStringList args, bool su, QObject *parent) : QObject(parent),
    program(program),
    args(args), su(su) {
    process = new QProcess(this);
#ifdef Q_OS_WIN
    if (program.contains("sing-box")) {
        process->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
            args->flags |= CREATE_NEW_PROCESS_GROUP;
        });
    }
#endif
    connect(process, &QProcess::started, [=]() {
        _started = true;
        emit started();
    });

    connect(process, &QProcess::errorOccurred, [=](QProcess::ProcessError error) {
        _failed = true;
        emit failed(ProcessFailedException(
            "Process: " + processErrorToString(error).toStdString() + program.toStdString() + " " + (args.join(" ").
                toStdString())));
    });
    connect(process, &QProcess::readyReadStandardOutput, [=]() {
        QByteArray output = process->readAllStandardOutput();
        Logger.trace(QString(output).toStdString());
        emit outLog(output);
    });

    connect(process, &QProcess::readyReadStandardError, [=]() {
        QByteArray error = process->readAllStandardError();
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
        process->start("sudo", args);
    } else {
        process->start(program, args);
    }
}

void ProcessRunner::stop() {
    _started = false;
    _failed = false;
    if (QSysInfo::kernelType() == "linux") {
        process->terminate();
        if (su) {
            QProcess::execute("sudo", QStringList() << "killall" << program);
        } else {
            QProcess::execute("killall", QStringList()  << program);
        }
    } else if (QSysInfo::kernelType() == "winnt") {
        if (program.contains("sing-box")) {
#ifdef Q_OS_WIN
            qint64 pid = process->processId();
            if (pid > 0) {
                SetConsoleCtrlHandler(NULL, TRUE);

                // 1. Пытаемся мягко через CTRL_BREAK
                GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, static_cast<DWORD>(pid));

                if (process->waitForFinished(3000)) {
                    SetConsoleCtrlHandler(NULL, FALSE);
                    Logger.info("finished by CTRL_BREAK_EVENT");
                    return;
                }

                // 2. fallback через stdin
                process->write("exit\n");
                if (process->waitForFinished(2000)) {
                    SetConsoleCtrlHandler(NULL, FALSE);
                    Logger.info("finished by exit\n");
                    return;
                }

                // 3. жёсткий kill (последний вариант)
                process->kill();
                process->waitForFinished(2000);

                SetConsoleCtrlHandler(NULL, FALSE);
            }
#endif
        } else {
            auto splitted = program.split("/");
            QProcess::execute("taskkill", QStringList() << "/F" << "/im" << splitted.constLast());
        }
    }
}
