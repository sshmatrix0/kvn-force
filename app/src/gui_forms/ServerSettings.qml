import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import ServersManager 1.0

Rectangle {
    id: root
    color: Theme.bgPrimary
    property var serverAsMap
        signal
    cancel
        signal
    save
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.edgeMargins
        spacing: Theme.columnSpacing

        Text {
            text: serverAsMap.name + " settings"
            color: Theme.textPrimary
            font.pixelSize: Theme.textHeader2Size
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 50
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            Layout.topMargin: 20
            Column {                              // вместо ColumnLayout
                width: parent.width
                spacing: Theme.columnSpacing
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
                ASelectField {
                    id: networkTypeId
                    Layout.topMargin: 20
                    label: "Network"
                    model: ["xhttp", "grpc"]
                    currentIndex: networkTypeId.findIndex(serverAsMap.network)
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: networkTypeId.currentValue === "xhttp"
                    id: xHttpPath
                    label: "xHttpPath"
                    text: serverAsMap.xHttpPath
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: networkTypeId.currentValue === "grpc"
                    id: grpcServiceName
                    label: "Grpc Service Name"
                    text: serverAsMap.grpcServiceName
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ASelectField {
                    id: securityTypeId
                    Layout.topMargin: 20
                    label: "Security"
                    model: ["tls", "reality"]
                    currentIndex: securityTypeId.findIndex(serverAsMap.security)
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: securityTypeId.currentValue === "tls"
                    id: tlsFingerprint
                    label: "Tls Fingerprint"
                    text: serverAsMap.tlsFingerprint
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: securityTypeId.currentValue === "reality"
                    id: realityServerName
                    label: "Reality Server Name"
                    text: serverAsMap.realityServerName
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: securityTypeId.currentValue === "reality"
                    id: realityFingerprint
                    label: "Reality FingerPrint"
                    text: serverAsMap.realityFingerprint
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: securityTypeId.currentValue === "reality"
                    id: realityPublicKey
                    label: "Reality Public Key"
                    text: serverAsMap.realityPublicKey
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: securityTypeId.currentValue === "reality"
                    id: realityShortId
                    label: "Reality Short Id"
                    text: serverAsMap.realityShortId
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
                ATextField {
                    visible: securityTypeId.currentValue === "reality"
                    id: realitySpiderX
                    label: "Reality SpiderX"
                    text: serverAsMap.realitySpiderX
                    Layout.alignment: Qt.AlignCenter | Qt.AlignTop
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 50
            RoundButton {
                text: "Save"
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: Theme.textSize
                onClicked: {
                    var modifiedServerAsMap = {
                        id: root.serverAsMap.id,
                        ip: serverIp.text,
                        name: serverName.text,
                        port: parseInt(serverPort.text, 10),
                        network: networkTypeId.currentValue,
                        security: securityTypeId.currentValue,
                        xHttpPath: xHttpPath.text,
                        grpcServiceName: grpcServiceName.text,
                        tlsFingerprint: tlsFingerprint.text,
                        realityServerName: realityServerName.text,
                        realityFingerprint: realityFingerprint.text,
                        realityPublicKey: realityPublicKey.text,
                        realityShortId: realityShortId.text,
                        realitySpiderX: realitySpiderX.text
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