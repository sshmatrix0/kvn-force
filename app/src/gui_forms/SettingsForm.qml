import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: Theme.bgPrimary
    signal dnsSettings
    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: Theme.edgeMargins
        anchors.rightMargin: Theme.edgeMargins
        spacing: Theme.columnSpacing
        Text {
            text: "Settings"
            color: Theme.textPrimary
            font.pixelSize: Theme.textHeader1Size
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.topMargin: 30
        }
        ListView {
            id: settingsView
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: [
                {name: "Routing", icon: "qrc:/res/alt_route.svg"},
                {name: "TEST2", icon: "qrc:/res/add.svg", iconActive: "qrc:/res/add_active.svg"},
                {name: "settings", icon: "qrc:/res/settings.svg", iconActive: "qrc:/res/settings_active.svg"}
            ]

            delegate: Rectangle {
                width: parent.width
                height: 50
                color: "transparent"
                MouseArea {
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill:parent
                    onClicked: {
                        dnsSettings();
                    }
                }
                RowLayout {
                    anchors.fill: parent
                    Image {
                        source: modelData.icon
                        sourceSize.width: 20
                        sourceSize.height: 20
                        mipmap: true
                    }
                    Text {
                        id: itemName
                        font.pixelSize: Theme.textSize
                        color: Theme.textPrimary
                        text: modelData.name
                    }
                    Item {
                        id: itemSpace
                        Layout.fillWidth: true
                    }
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: Theme.border
                    visible: index !== settingsView.count - 1
                }
            }

            spacing: 10
        }
    }
}