#pragma once
#include <QSettings>
#include "version.h"
class Settings: public QObject {
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    Q_INVOKABLE void set(const QString &key, const QVariant &value);
    Q_INVOKABLE QVariant get(const QString &key) const;

    Q_INVOKABLE QString getRouteByDefault() const;
    Q_INVOKABLE QString getDomainsForProxy() const;
    Q_INVOKABLE QString getDomainsForDirect() const;
    Q_INVOKABLE QString getProcessNamesForProxy() const;
    Q_INVOKABLE QString getProcessNamesForDirect() const;
    Q_INVOKABLE void setRouteByDefault(QString value);
    Q_INVOKABLE void setDomainsForProxy(QString value);
    Q_INVOKABLE void setDomainsForDirect(QString value);
    Q_INVOKABLE void setProcessNamesForProxy(QString value);
    Q_INVOKABLE void setProcessNamesForDirect(QString value);

    Q_INVOKABLE QStringList getRuleSetsForProxy() const;
    Q_INVOKABLE void setRuleSetsForProxy(QStringList ruleSets);

    Q_INVOKABLE QStringList getAllSettingsKeys() const;
    Q_INVOKABLE bool remove(const QString &key);
    Q_INVOKABLE QString getAppVersion();
    Q_INVOKABLE QString getGitHash();
    QString getRouteByDefaultLabel() const;
    QString getDomainsForProxyLabel() const;
    QString getDomainsForDirectLabel() const;
    QString getProcessNamesForProxyLabel() const;
    QString getProcessNamesForDirectLabel() const;
    QString getRuleSetsForProxyLabel() const;
private:
    QSettings qSettings;
};
