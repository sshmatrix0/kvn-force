import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import MyTray 1.0
import ServersManager 1.0
import VPNManager 1.0
import "."

ApplicationWindow {
    id: "mainWindow"
    visible: true
    width: 360
    height: 640
    color: "#22000000" // тёмный фон
    title: "KVN Force"
    Component.onCompleted: {
        SystemTray.visible = true;
    }
    Connections {

        target: SystemTray

        // Обработка активации иконки
        function onActivated(reason) {
            console.log("==============================" + reason);
            if (reason === 3) { // QSystemTrayIcon::Trigger (одинарный клик)
                mainWindow.show()
                mainWindow.raise()
                mainWindow.requestActivate()
            }
        }

        function onQuit() {
            Qt.quit();
        }

        function onShowOrHide() {
            mainWindow.visible = !mainWindow.visible;
        }
    }
    Connections {
        target: ServersManager

        function onWithoutServersChanged(value) {
            if (!value) {
                stack.replace(home);
            } else {
                stack.replace(add);
            }
        }
    }
    StackView {
        id: stack
        anchors.fill: parent

        initialItem: ServersManager.withoutServers ? add : home
    }

    Component {
        id: home
        Connect {
            id: connect
            onEditServerSettings: function (serverId) {
                var serverAsMap = ServersManager.getServerAsMapById(serverId);
                stack.replace(editSettings, {serverAsMap: serverAsMap});
            }
        }
    }
    Component {
        id: add
        Add {
            id: addId
        }
    }
    Component {
        id: routingList
        RoutingSettings {
            id: routingListId
            onCancel: function () {
                bottomPanel.setCurrentTab("settings");
            }
            onSave: function () {
                bottomPanel.setCurrentTab("settings");
            }
        }
    }
    Component {
        id: routingRSList
        RoutingSettingsRS {
            id: routingRSListId
            onCancel: function () {
                bottomPanel.setCurrentTab("settings");
            }
            onSave: function () {
                bottomPanel.setCurrentTab("settings");
            }
        }
    }
    Component {
        id: aboutIdC
        About {
            id: aboutId
        }
    }
    Component {
        id: settings
        SettingsForm {
            id: settingsId
            onRoutingManualSettings: function () {
                stack.replace(routingList);
            }
            onRoutingRuleSetsSettings: function () {
                stack.replace(routingRSList);
            }
            onAbout: function(){
                stack.replace(aboutIdC);
            }
        }
    }
    Component {
        id: editSettings
        ServerSettings {
            id: editSettingsId
            onCancel: function () {
                bottomPanel.setCurrentTab("home");
            }
            onSave: function () {
                bottomPanel.setCurrentTab("home");
            }
        }
    }
    BottomPanel {
        id: bottomPanel
        onTabChanged: function (name) {
            console.info("==================================" + name);
            if (name == "add") {
                stack.replace(add);
            } else if (name == "home") {
                stack.replace(home)
            } else if (name == "settings") {
                stack.replace(settings)
            }
        }
    }
}