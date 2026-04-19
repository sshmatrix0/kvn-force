//
// Created by ksp on 21.03.2026.
//

#include "system_tray.h"
#include <QIcon>
#include <QObject>
SystemTray::SystemTray(QObject *parent) : QObject(parent) {
    auto showOrHideAction = new QAction("Show / Hide", this);
    auto vpnConnectAction = new QAction("Connect", this);
    auto vpnDisconnectAction = new QAction("Disconnect", this);
    auto quitAction = new QAction("Exit", this);
    connect(showOrHideAction, &QAction::triggered, this, &SystemTray::showOrHide);
    connect(vpnConnectAction, &QAction::triggered, this, &SystemTray::vpnConnect);
    connect(vpnDisconnectAction, &QAction::triggered, this, &SystemTray::vpnDisconnect);
    connect(quitAction, &QAction::triggered, this, &SystemTray::quit);
    trayIconMenu = new QMenu();
    trayIconMenu->addAction(showOrHideAction);
    trayIconMenu->addAction(vpnConnectAction);
    trayIconMenu->addAction(vpnDisconnectAction);
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon();
    QIcon icon;
    icon.addFile(":/res/icon.svg", QSize(50, 50));
    // auto icon = Qi(":/res/icon.svg", QSize(32, 32));
    trayIcon->setIcon(icon);
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTray::onActivated);
}

SystemTray::~SystemTray() {
    trayIcon->hide();
    delete trayIconMenu;
}


bool SystemTray::isVisible() const
{
    return trayIcon->isVisible();
}

void SystemTray::setVisible(bool visible)
{
    if (visible == trayIcon->isVisible())
        return;
    trayIcon->setVisible(visible);
    emit visibleChanged();
}

void SystemTray::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    int reasonInt = static_cast<int>(reason);
    emit activated(reasonInt);
}

void SystemTray::onVPNConnected() {
    trayIcon->setIcon(QPixmap(":/res/icon_active.svg"));
}

void SystemTray::onVPNDisconnected() {
    trayIcon->setIcon(QPixmap(":/res/icon.svg"));
}
