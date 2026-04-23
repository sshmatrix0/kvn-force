pragma Singleton
import QtQuick 2.15

QtObject {
    // Цвета текста
    readonly property color bgPrimary: "#0e0e11"
    readonly property color bgSecondary: "#1c1d21"
    readonly property color textPrimary: "#bebebe"
    readonly property int textSize: 18
    readonly property int textSubSize: 10
    readonly property int textHeader1Size: 30
    readonly property int textHeader2Size: 20
    readonly property color textSecondary: "#757575"
    readonly property color textAccent: "#ff4081"
    readonly property color selectionColor: "#633303"
    readonly property color selectedTextColor: "#fbb26a"
    readonly property color textError: "#f44336"
    readonly property color active: "#fbb26a"
    readonly property color disable: "#bebebe"
    readonly property color border: "#363434"
    readonly property int edgeMargins: 20
    readonly property int columnSpacing: 5

    // Цвета фона и другие можно добавить здесь же
}