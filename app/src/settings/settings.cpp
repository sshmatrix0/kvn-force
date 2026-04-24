//
// Created by ksp on 23.03.2026.
//

#include "settings.h"

#include <magic_enum/magic_enum.hpp>

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

QString Settings::getRouteByDefault() const {
    auto value = qSettings.value(getRouteByDefaultLabel(), QString());
    if (value.isValid() && !value.toString().isEmpty()) {
        return value.toString();
    }
    return "direct";
}

QString Settings::getDomainsForProxy() const {
    auto value = qSettings.value(getDomainsForProxyLabel(), QString());
    if (value.isValid() && !value.toString().isEmpty()) {
        return value.toString();
    }
    return "telegram.org";
}

QString Settings::getDomainsForDirect() const {
    auto value = qSettings.value(getDomainsForDirectLabel(), QString());
    if (value.isValid() && !value.toString().isEmpty()) {
        return value.toString();
    }
    return ".ru;.рф;vkuseraudio.net;vkuseraudio.com;userapi.com;.su;yandex.net;yandex.com";
}

QString Settings::getProcessNamesForProxy() const {
    auto value = qSettings.value(getProcessNamesForProxyLabel(), QString());
    if (value.isValid() && !value.toString().isEmpty()) {
        return value.toString();
    }
    return "Telegram;Telegram.exe;Discord.exe;Discord";
}

QString Settings::getProcessNamesForDirect() const {
    auto value = qSettings.value(getProcessNamesForDirectLabel(), QString());
    if (value.isValid() && !value.toString().isEmpty()) {
        return value.toString();
    }
    return "";
}

void Settings::setRouteByDefault(QString value)  {
    qSettings.setValue(getRouteByDefaultLabel(), value);
    qSettings.sync();
}

void Settings::setDomainsForProxy(QString value) {
    qSettings.setValue(getDomainsForProxyLabel(), value);
    qSettings.sync();
}

void Settings::setDomainsForDirect(QString value) {
    qSettings.setValue(getDomainsForDirectLabel(), value);
    qSettings.sync();
}

void Settings::setProcessNamesForProxy(QString value) {
    qSettings.setValue(getProcessNamesForProxyLabel(), value);
    qSettings.sync();
}

void Settings::setProcessNamesForDirect(QString value) {
    qSettings.setValue(getProcessNamesForDirectLabel(), value);
    qSettings.sync();
}

QStringList Settings::getRuleSetsForProxy() const {
    auto value = qSettings.value(getRuleSetsForProxyLabel(), QVariant());
    if (value.isValid() && !value.isNull()) {
        return value.toStringList();
    }
    QStringList defaultList;
    defaultList.append("https://github.com/SagerNet/sing-geosite/raw/rule-set/geosite-youtube.srs");
    defaultList.append("https://github.com/SagerNet/sing-geosite/raw/rule-set/geosite-google-play.srs");
    defaultList.append("https://github.com/burjuyz/RuRulesets/raw/main/ruleset-domain-torrent_trackres.srs");
    defaultList.append("https://github.com/burjuyz/RuRulesets/raw/main/ruleset-domain-antifilter_community.srs");
    defaultList.append("https://github.com/burjuyz/RuRulesets/raw/main/ruleset-ip-antifilter_allyouneed.srs");
    return defaultList;
}

void Settings::setRuleSetsForProxy(QStringList ruleSets) {
    qSettings.setValue(getRuleSetsForProxyLabel(), ruleSets);
    qSettings.sync();
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

QString Settings::getAppVersion() {
    return QString("%1").arg(APP_VERSION);
}

QString Settings::getGitHash() {
    return QString("%1").arg(GIT_HASH);
}

QString Settings::getRouteByDefaultLabel() const {
    return "routeByDefault";
}

QString Settings::getDomainsForProxyLabel() const {
    return "domainsForProxy";
}

QString Settings::getDomainsForDirectLabel() const {
    return "domainsForDirect";
}

QString Settings::getProcessNamesForProxyLabel() const {
    return "processNamesForProxy";
}

QString Settings::getProcessNamesForDirectLabel() const {
    return "processNamesForDirect";
}

QString Settings::getRuleSetsForProxyLabel() const {
    return "ruleSetsForProxy";
}
