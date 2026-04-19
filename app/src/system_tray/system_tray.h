#pragma once
#include <QSystemTrayIcon>
#include  <QMenu>

class SystemTray : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit SystemTray(QObject *parent = nullptr);

    ~SystemTray();

    bool isVisible() const;
    void setVisible(bool visible);
signals:
    void visibleChanged();
    void activated(int reason); // 0: unknown, 1: context, 2: doubleClick, 3: trigger, 4: middleClick
    void quit();
    void showOrHide();
    void vpnConnect();
    void vpnDisconnect();
public slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void onVPNConnected();
    void onVPNDisconnected();

private:
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};
