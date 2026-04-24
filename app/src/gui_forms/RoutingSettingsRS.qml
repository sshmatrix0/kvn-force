import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Settings 1.0

Rectangle {
    id: root
    color: Theme.bgPrimary
        signal cancel
        signal save

    function getRuleSet(index) {
        var ruleSets = Settings.getRuleSetsForProxy();
        if (ruleSets.length > index) {
            return ruleSets[index];
        }
        return "";
    }

    ColumnLayout {
        anchors.fill: parent                     // заполняем весь родитель
        anchors.margins: Theme.edgeMargins
        spacing: Theme.columnSpacing

        Text {
            text: "Routing (Rule Sets For Proxy)"
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
                    id: ruleSet1
                    label: "Rule set #1"
                    text: getRuleSet(0)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet2
                    label: "Rule set #2"
                    text: getRuleSet(1)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet3
                    label: "Rule set #3"
                    text: getRuleSet(2)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet4
                    label: "Rule set #4"
                    text: getRuleSet(3)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet5
                    label: "Rule set #5"
                    text: getRuleSet(4)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet6
                    label: "Rule set #6"
                    text: getRuleSet(5)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet7
                    label: "Rule set #7"
                    text: getRuleSet(6)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet8
                    label: "Rule set #8"
                    text: getRuleSet(6)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet9
                    label: "Rule set #9"
                    text: getRuleSet(6)
                    width: parent.width
                }
                ATextField {
                    id: ruleSet10
                    label: "Rule set #10"
                    text: getRuleSet(6)
                    width: parent.width
                }
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 50
            RoundButton {
                text: "Save"
                Layout.topMargin: 10
                font.pixelSize: Theme.textSize
                onClicked: {
                    var ruleSets = [];
                    if(ruleSet1.text){
                        ruleSets.push(ruleSet1.text);
                    }
                    if(ruleSet2.text){
                        ruleSets.push(ruleSet2.text);
                    }
                    if(ruleSet3.text){
                        ruleSets.push(ruleSet3.text);
                    }
                    if(ruleSet4.text){
                        ruleSets.push(ruleSet4.text);
                    }
                    if(ruleSet5.text){
                        ruleSets.push(ruleSet5.text);
                    }
                    if(ruleSet6.text){
                        ruleSets.push(ruleSet6.text);
                    }
                    if(ruleSet7.text){
                        ruleSets.push(ruleSet7.text);
                    }
                    if(ruleSet8.text){
                        ruleSets.push(ruleSet8.text);
                    }
                    if(ruleSet9.text){
                        ruleSets.push(ruleSet9.text);
                    }
                    if(ruleSet10.text){
                        ruleSets.push(ruleSet10.text);
                    }
                    Settings.setRuleSetsForProxy(ruleSets);
                    root.save();
                }
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    radius: parent.radius
                    color: parent.down ? "#bbbbbb" : Theme.active
                    border.color: "#26282a"
                    border.width: 1
                }
            }

            RoundButton {
                text: "Cancel"
                Layout.topMargin: 10
                font.pixelSize: Theme.textSize
                onClicked: root.cancel()
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    radius: parent.radius
                    color: parent.down ? "#bbbbbb" : Theme.active
                    border.color: "#26282a"
                    border.width: 1
                }
            }
        }
    }
}