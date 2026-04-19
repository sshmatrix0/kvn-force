import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import MyTray 1.0
import ServersManager 1.0
import VPNManager 1.0
import QtQuick.Shapes 1.15

Rectangle {
    id: root
    color: Theme.bgPrimary
    signal editServerSettings(string serverId)
    function isConnected() {
        return VPNManager.connectionState == "CONNECTED";
    }

    function isConnecting() {
        return VPNManager.connectionState == "CONNECTING";
    }

    function isFailed() {
        return VPNManager.connectionState == "FAILED";
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        anchors.topMargin: 80
        spacing: 20

        // Кнопка подключения
        Rectangle {
            id: connectButton
            width: 180
            height: 180
            radius: 90
            color: isConnected() ? Theme.bgSecondary : Theme.bgPrimary
            border.color: isConnected() ? Theme.active : Theme.disable
            border.width: 2


            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

            Shape {
                id: progressArc
                anchors.fill: parent
                visible: isConnecting()

                property real rotationAngle: 0
                NumberAnimation on rotationAngle {
                    from: 0;
                    to: 360; duration: 1000
                    loops: Animation.Infinite
                    running: isConnecting()
                }

                ShapePath {
                    strokeWidth: 2
                    strokeColor: "#fbb26a"
                    fillColor: "transparent"
                    capStyle: ShapePath.RoundCap

                    PathAngleArc {
                        centerX: progressArc.width / 2
                        centerY: progressArc.height / 2
                        radiusX: (progressArc.width / 2) - 6
                        radiusY: (progressArc.height / 2) - 6
                        startAngle: progressArc.rotationAngle - 90
                        sweepAngle: 180
                    }
                }
            }
            Text {
                anchors.centerIn: parent
                text: isConnected() ? "Connected" : (isConnecting() ? "Connecting" : "Disconnected")
                color: isConnected() ? Theme.active : Theme.textPrimary
                font.pixelSize: 18
                font.bold: true
            }

            MouseArea {
                cursorShape: Qt.PointingHandCursor
                anchors.fill: parent
                onClicked: {
                    console.log("=========================" + VPNManager.connectionState);
                    if (isConnected() || isConnecting()) {
                        VPNManager.stop();
                    } else {
                        VPNManager.start();
                    }
                }
            }
        }


    }
    MouseArea {
        anchors.fill: parent
        visible: serverPicker.opened
        onClicked: {
            serverPicker.opened = false;
        }
    }
    ServerPicker {
        id: serverPicker
        onEditServerSettings: function(serverId)
        {
            root.editServerSettings(serverId);
        }
    }

    BottomPanel {
        id: bottomPanel
    }
}