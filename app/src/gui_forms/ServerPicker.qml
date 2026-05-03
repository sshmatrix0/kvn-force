import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.15
import ServersManager 1.0

Rectangle {
    id: sheet
    width: parent.width
    height: opened ? parent.height - 100 : 200
    radius: 20
    anchors.bottom: parent.bottom
    border.width: 1
    border.color: Theme.border
    color: Theme.bgSecondary
    property bool opened: false

    signal editServerSettings(string serverId)

    Behavior on height {
        NumberAnimation {
            duration: 300
            easing.type: Easing.OutCubic
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        Text {
            text: ServersManager.currentServerName
            color: Theme.textPrimary
            font.pixelSize: 20
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.topMargin: 50
        }
        RowLayout{
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Text {
                text: ServersManager.currentServerIp
                color: Theme.textPrimary
                font.pixelSize: 10
            }
            Text {
                text: "|"
                color: Theme.textPrimary
                font.pixelSize: 10
            }
            Text {
                text: ServersManager.currentServerNetwork
                color: Theme.textPrimary
                font.pixelSize: 10
            }
            Text {
                text: "|"
                color: Theme.textPrimary
                font.pixelSize: 10
            }
            Text {
                text: ServersManager.currentServerSecurity
                color: Theme.textPrimary
                font.pixelSize: 10
            }
        }

        Text {
            text: "Servers"
            color: Theme.textPrimary
            font.pixelSize: 20
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 30
            Layout.rightMargin: 17
            Layout.topMargin: 100
        }

        // Список серверов
        ListView {
            id: serversView
            Layout.leftMargin: 30
            Layout.rightMargin: 15
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            model: ServersManager

            delegate: Rectangle {
                width: parent ? parent.width : 0
                height: 50
                radius: 10
                color: Theme.bgSecondary
                border.color: Theme.bgSecondary
                border.width: 1
                ButtonGroup {
                    id: buttonGroup
                }
                RowLayout {
                    anchors.fill: parent
                    RadioButton {
                        id: control
                        text: model.ip
                        property string serverId: model.id
                        property string serverName: model.name
                        property string network: model.network
                        property string security: model.security
                        ButtonGroup.group: buttonGroup
                        checked: model.id == ServersManager.currentServerId
                        // Кастомный текст
                        contentItem: ColumnLayout {
                            Text {
                                text: control.serverName
                                color: control.checked ? Theme.active : Theme.textPrimary
                                font.pixelSize: 18
                                verticalAlignment: Text.AlignVCenter
                                leftPadding: control.indicator.width + control.spacing
                            }
                            RowLayout{
                                Text {
                                    text: control.text
                                    color: control.checked ? Theme.active : Theme.textPrimary
                                    font.pixelSize: 10
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: "|"
                                    color: control.checked ? Theme.active : Theme.textPrimary
                                    font.pixelSize: 10
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: control.network
                                    color: control.checked ? Theme.active : Theme.textPrimary
                                    font.pixelSize: 10
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: "|"
                                    color: control.checked ? Theme.active : Theme.textPrimary
                                    font.pixelSize: 10
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: control.security
                                    color: control.checked ? Theme.active : Theme.textPrimary
                                    font.pixelSize: 10
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }

                        }
                        // кастомный индикатор (кружок)
                        indicator: Rectangle {
                            implicitWidth: 20
                            implicitHeight: 20
                            radius: 10
                            border.color: control.checked ? Theme.active : Theme.textPrimary
                            border.width: 2
                            color: control.checked ? Theme.active : "transparent"
                            Canvas {
                                anchors.fill: parent
                                visible: control.checked
                                Layout.topMargin: 10
                                onPaint: {
                                    var ctx = getContext("2d")
                                    ctx.beginPath()
                                    ctx.arc(width / 2, height / 2, 5, 0, 2 * Math.PI)
                                    ctx.fillStyle = "white"
                                    ctx.fill()
                                }
                            }
                        }
                    }

                    Item {
                        id: itemSpace
                        Layout.fillWidth: true
                    }
                    Image {
                        source: "qrc:/res/delete.svg"
                        sourceSize.width: 20
                        sourceSize.height: 20
                        mipmap: true
                        MouseArea {
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                if (control.serverId == ServersManager.currentServerId) {
                                    toast.show("Can't delete active server");
                                    return;
                                }
                                dialog.serverId = control.serverId;
                                dialog.serverName = control.serverName;
                                dialog.open();
                            }
                        }
                    }
                    Image {
                        source: "qrc:/res/settings.svg"
                        sourceSize.width: 20
                        sourceSize.height: 20
                        mipmap: true
                        MouseArea {
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                console.log("Try to open panel for edit server: " + control.serverId);
                                sheet.editServerSettings(control.serverId);
                            }
                        }
                    }
                }

                MouseArea {
                    cursorShape: Qt.PointingHandCursor
                    width: control.width + itemSpace.width
                    height: parent.height
                    onClicked: {
                        ServersManager.setNewCurrentServer(control.serverId)
                    }
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: Theme.border
                    visible: index !== serversView.count - 1
                }
            }

            spacing: 10
        }
    }

    MouseArea {
        anchors.fill: parent
        visible: !sheet.opened
        onClicked: {
            sheet.opened = !opened;
        }
    }
    DeleteDialog {
        id: dialog

        onConfirmed: {
            ServersManager.deleteServer(serverId);
        }
    }
    Toast {
        id: toast
    }
}