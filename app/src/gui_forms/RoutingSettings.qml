import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Settings 1.0
Rectangle {
    id: root
    color: Theme.bgPrimary
    signal cancel
    signal save

    function getRouteByDefaultIndex(){
        var route = Settings.getRouteByDefault();
        return routeByDefaultId.findIndex(route);
    }
    function getDomainsForProxy(){
        var domains = Settings.getDomainsForProxy();
        return domains;
    }
    function getDomainsForDirect(){
        var domains = Settings.getDomainsForDirect();
        return domains;
    }
    function getProcessNamesForProxy(){
        var processes = Settings.getProcessNamesForProxy();
        return processes;
    }
    function getProcessNamesForDirect(){
        var processes = Settings.getProcessNamesForDirect();
        return processes;
    }
    ColumnLayout {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.margins: Theme.edgeMargins
        spacing: Theme.columnSpacing
        Text {
            text: "Routing"
            color: Theme.textPrimary
            font.pixelSize: Theme.textHeader2Size
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
            Layout.topMargin: 50
        }
        ASelectField {
            id: routeByDefaultId
            Layout.topMargin: 20
            label: "Route By Default"
            model: ["proxy", "direct"]
            currentIndex: getRouteByDefaultIndex()
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            Layout.topMargin: 20
            id: domainProxy
            label: "Domains For Proxy"
            text: getDomainsForProxy()
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            id: domainDirect
            label: "Domains For Direct"
            text: getDomainsForDirect()
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            id: processProxy
            label: "Process Name For Proxy"
            text: getProcessNamesForProxy()
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        ATextField {
            id: processDirect
            label: "Process Name For Direct"
            text: getProcessNamesForDirect()
            Layout.alignment: Qt.AlignCenter | Qt.AlignTop
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            RoundButton {
                text: "Save"
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: Theme.textSize
                onClicked: {
                    Settings.setRouteByDefault(routeByDefaultId.currentValue);
                    Settings.setDomainsForProxy(domainProxy.text);
                    Settings.setDomainsForDirect(domainDirect.text);
                    Settings.setProcessNamesForProxy(processProxy.text);
                    Settings.setProcessNamesForDirect(processDirect.text);
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
                font.pixelSize: Theme.textSize
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