import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Effects
import QtQuick.Layouts 1.15
import ServersManager 1.0

Rectangle {
    function addNewServer() {
        console.info("Try to add new server");
        try {
            ServersManager.addNewServer(textInput.text);
        } catch (ex) {
            console.error("Error: " + ex);
            toast.show("Error to add", 2000, "ERROR");
            return;
        }
        toast.show("Success", 2000, "SUCCESS");
    }
    color: Theme.bgPrimary
    ColumnLayout {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: 20
        spacing: 5
        Text {
            text: "Connection"
            color: Theme.textPrimary
            font.pixelSize: 30
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.topMargin: 30
        }
        Rectangle {
            id: fieldContainer
            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            height: 70
            color: Theme.bgSecondary
            radius: 12
            border.width: 1
            border.color: Theme.border
            Layout.topMargin: 100
            Behavior on border
            .
            width {
                NumberAnimation {
                    duration: 200; easing.type: Easing.OutCubic
                }
            }
            Behavior on border
            .
            color {
                ColorAnimation {
                    duration: 200; easing.type: Easing.OutCubic
                }
            }

            // Эффект свечения через MultiEffect
            MultiEffect {
                id: glowEffect
                anchors.fill: fieldContainer
                source: fieldContainer

                // Настройка тени для создания свечения
                shadowEnabled: true
                shadowBlur: 0.8
                shadowColor: Theme.active
                shadowHorizontalOffset: 0
                shadowVerticalOffset: 0
                shadowOpacity: 0.6

                opacity: 0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 200; easing.type: Easing.OutCubic
                    }
                }
            }

            TextField {
                id: textInput
                anchors.fill: parent
                anchors.margins: 12
                background: Rectangle {
                    color: "transparent"
                    radius: 8
                }
                placeholderText: "Insert key..."
                placeholderTextColor: Theme.textPrimary
                color: "#FFFFFF"
                font.pixelSize: 16
                font.family: "Inter"
                selectByMouse: true
                leftPadding: 16
                rightPadding: 16
                verticalAlignment: TextInput.AlignVCenter

                onActiveFocusChanged: {
                    if (activeFocus) {
                        fieldContainer.border.width = 2
                        fieldContainer.border.color = Theme.active
                        glowEffect.opacity = 0.6
                    } else {
                        fieldContainer.border.width = 0
                        fieldContainer.border.color = Theme.border
                        glowEffect.opacity = 0
                    }
                }
                TextMetrics {
                    elide: Text.ElideRight
                }
                onTextChanged: function(){
                    textInput.text= textInput.text.trim();
                }
                onAccepted: function () {
                    addNewServer();
                }
            }
        }
        RoundButton {
            text: "Add"
            Layout.topMargin: 10
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 16
            onClicked: {
                addNewServer();
            }
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 50
                // Use a high radius for a perfect circle or match the button's radius
                radius: parent.radius
                color: parent.down ? "#bbbbbb" : Theme.active // Changes color when pressed
                border.color: "#26282a"
                border.width: 1
            }
        }

    }
    Toast {
        id: toast
    }
}