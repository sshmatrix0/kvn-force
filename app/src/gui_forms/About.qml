import QtQuick 2.15
import QtQuick.Layouts 1.15
import Settings 1.0

Rectangle {
    color: Theme.bgPrimary
    Column {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.centerIn: parent
        spacing: 10
        Image {
            source: "qrc:/res/icon_active.svg"
            sourceSize.width: 150
            sourceSize.height: 150
            mipmap: true
        }
        Text {
            text: "KVN Force"
            color: Theme.selectedTextColor
            font.pixelSize: Theme.textHeader1Size
        }
        TextEdit {
            text: "Ver: " + Settings.getAppVersion() + "(" + Settings.getGitHash() + ")"
            readOnly: true
            color: Theme.textPrimary
            font.pixelSize: Theme.textSize
            selectByMouse: true
            selectionColor: Theme.selectedTextColor
        }
    }
}