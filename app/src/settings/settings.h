#pragma once
#include <QSettings>

class Settings: public QObject {
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
    Q_INVOKABLE void set(const QString &key, const QVariant &value);
    Q_INVOKABLE QVariant get(const QString &key) const;
    Q_INVOKABLE QStringList getAllSettingsKeys() const;
    Q_INVOKABLE bool remove(const QString &key);
private:
    QSettings qSettings;
};
