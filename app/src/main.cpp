#include <QApplication>
#include <QQmlApplicationEngine>

#include "logger/logger.h"
#include "qml_register_manager.h"
#include "system_tray/system_tray.h"
#include "version.h"
void on_terminate()
{
    Logger.fatal("on_terminate"); // перепосылка сигнала
    exit(3); // выход из программы. Если не сделать этого, то обработчик будет вызываться бесконечно.
}
int main(int argc, char *argv[]) {
    std::set_terminate(on_terminate);
    Logger.setLogLevel(LogLevel::TRACE);
    QString versionString = QString("Version: %1 hash: %2").arg(APP_VERSION,GIT_HASH);
    Logger.info(versionString.toStdString());
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/res/app_icon.png"));
    QMLRegisterManager qmlRegisterManager;
    qmlRegisterManager.initialize();
    auto *engine = new QQmlApplicationEngine();
    QObject::connect(
        &(*engine),
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [&]() {
            delete engine;
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine->loadFromModule("kvn_force", "Main");
    QObject *root = engine->rootObjects().first();
    //QObject *form = root->findChild<QObject*>("Main");
    if (root) {
        //root->setProperty("visible", true);
    }
    QApplication::setQuitOnLastWindowClosed(false);
    return QCoreApplication::exec();
}
