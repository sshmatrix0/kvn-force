import QtQuick
import QtQuick.Controls

Item {
    id: root
    anchors.fill: parent
    visible: false

    property alias text: label.text
    property int duration: 2000

    Rectangle {
        id: background
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60

        color: Theme.bgPrimary
        radius: 16
        opacity: 0.0

        width: label.paintedWidth + 24
        height: label.paintedHeight + 16

        Text {
            id: label
            anchors.centerIn: parent
            font.pixelSize: 14
            color: Theme.textPrimary
            wrapMode: Text.Wrap
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 250
            }
        }
    }

    Timer {
        id: hideTimer
        interval: root.duration
        onTriggered: {
            background.opacity = 0
            root.visible = false
        }
    }

    function show(message, time, type) {
        label.text = message
        if (time !== undefined)
            duration = time
        if (type && type == "ERROR") {
            background.color = Theme.textError;
            label.color = "white"
        } else if (type && type == "SUCCESS") {
            background.color = Theme.active;
        }
        root.visible = true
        background.opacity = 1.0
        hideTimer.restart()
    }
}