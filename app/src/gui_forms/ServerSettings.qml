import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ServersManager 1.0

Rectangle {
    id: root
    color: Theme.bgPrimary
    property var serverAsMap
    signal cancel
    signal save
    ColumnLayout {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 20
        spacing: 5
        Text {
            text: serverAsMap.name + " settings"
            color: Theme.textPrimary
            font.pixelSize: 20
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.topMargin: 50
        }
        ATextField {
            Layout.topMargin: 20
            id: serverName
            label: "Name"
            text: serverAsMap.name
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            id: serverIp
            label: "Ip"
            text: serverAsMap.ip
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            id: serverPort
            label: "Port"
            text: serverAsMap.port
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            id: serverXHttpPath
            label: "XHTTP Path"
            text: serverAsMap.xHttpPath
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            RoundButton {
                text: "Save"
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 16
                onClicked: {
                    var modifiedServerAsMap = {
                        id: root.serverAsMap.id,
                        ip: serverIp.text,
                        name: serverName.text,
                        port: parseInt(serverPort.text,10),
                        xHttpPath: serverXHttpPath.text
                    }
                    ServersManager.updateServer(root.serverAsMap.id, modifiedServerAsMap);
                    root.save();
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    // Use a high radius for a perfect circle or match the button's radius
                    radius: parent.radius
                    color: parent.down ? "#bbbbbb" : Theme.active // Changes color when pressed
                    border.color: "#26282a"
                    border.width: 1
                }
            }
            RoundButton {
                text: "Cancel"
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 16
                onClicked: {
                    root.cancel();
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    // Use a high radius for a perfect circle or match the button's radius
                    radius: parent.radius
                    color: parent.down ? "#bbbbbb" : Theme.active // Changes color when pressed
                    border.color: "#26282a"
                    border.width: 1
                }
            }
        }
    }
}