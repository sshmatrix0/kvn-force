import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 320
    height: 64

    property alias text: input.text
    property string label: ""

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 14
        color: "#1c1f26"
        border.width: 1
        border.color: input.focus ? Theme.active : Theme.border

        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
    }

    // Заголовок (floating label)
    Text {
        id: labelText
        text: parent.label
        color: Theme.textPrimary
        font.pixelSize: 14

        x: 16
        y: input.text.length > 0 || input.focus ? 8 : 22

        Behavior on y {
            NumberAnimation { duration: 150 }
        }
    }

    TextInput {
        id: input
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 16
        anchors.bottomMargin: 10
        selectByMouse: true
        font.pixelSize: 16
        color: Theme.textPrimary
        selectionColor: Theme.selectionColor
        selectedTextColor: Theme.selectedTextColor
    }
}