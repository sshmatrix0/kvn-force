import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Popup {
    id: deleteDialog
    modal: true
    focus: true
    anchors.centerIn: parent
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    property string serverId
    property string serverName
    width: parent.width-20
    height: 200
    padding: 0

    background: Rectangle {
        radius: 20
        color: Theme.bgSecondary   // dark background
        border.color: Theme.border
    }

    Overlay.modal: Rectangle {
        color: "#00000088" // dim background
    }

    property string title: "Delete server: " + deleteDialog.serverName
    property string message: "Are you sure you want to delete this server?"
    signal confirmed()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        // Title
        Text {
            text: deleteDialog.title
            font.pixelSize: 20
            font.bold: true
            color: Theme.textPrimary
            Layout.fillWidth: true
        }

        // Message
        Text {
            Layout.alignment:Qt.AlignHCenter
            text: deleteDialog.message
            font.pixelSize: 20
            color: Theme.textPrimary
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        // Buttons
        RowLayout {
            Layout.fillWidth: true
            Layout.alignment:Qt.AlignHCenter
            spacing: 10

            RoundButton {
                text: "Cancel"
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 16
                onClicked: {
                    deleteDialog.close();
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
                text: "Delete"
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 16
                onClicked: {
                    deleteDialog.confirmed()
                    deleteDialog.close()
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    // Use a high radius for a perfect circle or match the button's radius
                    radius: parent.radius
                    color: parent.down ? "#bbbbbb" : Theme.textError // Changes color when pressed
                    border.color: "#26282a"
                    border.width: 1
                }
            }

        }
    }

    // Optional smooth animation
    enter: Transition {
        NumberAnimation {
            properties: "opacity,scale"
            from: 0
            to: 1
            duration: 150
            easing.type: Easing.OutQuad
        }
    }

    exit: Transition {
        NumberAnimation {
            properties: "opacity,scale"
            from: 1
            to: 0
            duration: 120
            easing.type: Easing.InQuad
        }
    }
}