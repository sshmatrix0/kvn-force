//
// Created by ksp on 23.03.2026.
//

#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent), qSettings("org.sshmatrix", "KVNForce") {
}

void Settings::set(const QString &key, const QVariant &value) {
    if (key.isEmpty()) return;
    qSettings.setValue(key, value);
    qSettings.sync();
}

QVariant Settings::get(const QString &key) const {
    return qSettings.value(key, QVariant());
}

QStringList Settings::getAllSettingsKeys() const {
    return qSettings.allKeys();
}

bool Settings::remove(const QString &key) {
    if (qSettings.contains(key)) {
        qSettings.remove(key);
        qSettings.sync();
        return true;
    }
    return false;
}

