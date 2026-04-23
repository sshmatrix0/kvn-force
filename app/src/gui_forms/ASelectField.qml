import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    width: 320
    height: 64
    property alias currentValue: combo.currentValue
    property alias currentText: combo.currentText
    property alias currentIndex: combo.currentIndex
    property string label: ""
    property var model: []
    function findIndex(text) {
        for (var i = 0; i < combo.model.length; i++) {
            if (combo.model[i] === text) {
                return i;
                break
            }
        }
        return -1;
    }
    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 14
        color: "#1c1f26"
        border.width: 1
        border.color: combo.focus ? Theme.active : Theme.border

        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
    }

    // Floating label
    Text {
        id: labelText
        text: parent.label
        color: Theme.selectedTextColor
        font.pixelSize: 14

        x: 16
        y: combo.currentIndex !== -1 || combo.focus ? 8 : 22

        Behavior on y {
            NumberAnimation { duration: 150 }
        }
    }

    ComboBox {
        id: combo
        model: parent.model

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 16
        anchors.bottomMargin: 6

        font.pixelSize: Theme.textSize

        background: Item {} // убираем дефолтный фон

        contentItem: Text {
            text: combo.displayText
            color: Theme.textPrimary
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            font.pixelSize:Theme.textSize
        }

        indicator: Text {
            text: "▼"
            color: Theme.textPrimary
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }

        popup: Popup {
            y: combo.height + 4
            width: combo.width

            background: Rectangle {
                color: "#1c1f26"
                radius: 10
                border.color: Theme.border
            }

            contentItem: ListView {
                implicitHeight: contentHeight
                clip: true

                model: combo.popup.visible ? combo.delegateModel : null

                delegate: ItemDelegate {
                    width: parent.width
                    height: 40
                    text: modelData
                }
            }
        }
    }
}