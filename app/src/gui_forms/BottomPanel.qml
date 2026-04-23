import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ServersManager 1.0

Rectangle {
    id: taskbar
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: 50
    border.width: 1
    border.color: Theme.border
    color: Theme.bgSecondary
    property string active: ServersManager.withoutServers ? "add" : "home"

    signal tabChanged(string name)
    function setCurrentTab(name) {
        taskbar.active = name;
        tabChanged(name);
    }
    Row {
        id: row
        anchors.centerIn: parent
        spacing: 20

        Repeater {
            model: [
                {name: "home", icon: "qrc:/res/home.svg", iconActive: "qrc:/res/home_active.svg"},
                {name: "add", icon: "qrc:/res/add.svg", iconActive: "qrc:/res/add_active.svg"},
                {name: "settings", icon: "qrc:/res/settings.svg", iconActive: "qrc:/res/settings_active.svg"}
            ]

            delegate: Rectangle {
                width: 35
                height: 35
                color: "transparent"
                visible: (modelData.name != "home") || (modelData.name == "home") && !ServersManager.withoutServers
                Image {
                    objectName: "item_" + index
                    anchors.centerIn: parent
                    source: (taskbar.active == modelData.name) ? modelData.iconActive : modelData.icon
                    width: 32
                    height: 32
                    fillMode: Image.PreserveAspectFit
                }

                MouseArea {
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill: parent
                    onClicked: {
                        taskbar.active = modelData.name;
                        tabChanged(modelData.name);
                        console.log("Switch to", modelData.name + " taskbar.active:" + taskbar.active + ", modelData.name" + modelData.name)
                    }
                }
            }
        }
    }
}