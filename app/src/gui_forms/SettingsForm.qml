import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    color: Theme.bgPrimary
    signal routingManualSettings
    signal routingRuleSetsSettings
    signal about
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
                {name: "Routing(Manual)", icon: "qrc:/res/alt_route.svg", callback:routingManualSettings},
                {name: "Routing(Rule Sets For Proxy)", icon: "qrc:/res/alt_route.svg", callback:routingRuleSetsSettings},
                {name: "About", icon: "qrc:/res/about.svg", callback:about}
            ]

            delegate: Rectangle {
                width: parent.width
                height: 50
                color: "transparent"
                MouseArea {
                    cursorShape: Qt.PointingHandCursor
                    anchors.fill:parent
                    onClicked: {
                        modelData.callback();
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