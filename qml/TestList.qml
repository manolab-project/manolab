import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import "./components"

Rectangle
{
    id: idTestListRoot

    property bool isAdmin: false;

    function testFinishedEvent() {
        startButton.status = 0; // play
    }

    function updateComList() {
        devicePopup.setModel(manufEngine.getComList()); // refresh view with the current model
    }

    function setTestStatus(itemIndex, status)
    {
        scriptSteps.setProperty(itemIndex, "status", status);
        idTestListView.currentIndex = itemIndex;
    }

    function resetAllStatus()
    {
        for (var i = 0; i < scriptSteps.count; i++) {
            scriptSteps.setProperty(i, "status", 0);
        }
        idTestListView.currentIndex = 0;
        idTestListView.positionViewAtIndex(0, idTestListView.Beginning);
    }

    function setModel(elements)
    {
        scriptSteps.clear();
        for (var i = 0; i < elements.length; i++)
        {
            scriptSteps.append({"title": elements[i].title, "active": elements[i].active, "status": 0 });
        }
    }

    signal sigItemToggled(int index, bool active)

    function slotItemToggled(index, active)
    {
        idTestListRoot.sigItemToggled(index, active);
    }

    ListModel {
        id: scriptSteps
    }

    ColumnLayout
    {
        anchors.margins: 10
        anchors.fill: parent

        ListView {
            id: idTestListView
            model: scriptSteps

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            ScrollBar.vertical: ScrollBar {
                active: true
                background:  Rectangle {
                    implicitWidth: 20
                    color:  "#F0F0F0"
                }
            }
          //  highlightRangeMode: idTestListView.StrictlyEnforceRange
            Keys.onDownPressed: idTestListView.incrementCurrentIndex();
            Keys.onUpPressed: idTestListView.decrementCurrentIndex();

            delegate: TestItem {
                idx: index
                name: title
                selected: active
                color: {
                   if (status === 1) {
                       return "steelblue";  // ON GOING
                   } else if (status === 2) {
                       return "green";  // OK !
                   } else if (status === 3) {
                       return "red";  // ERROR
                   } else {
                       return "white";  // NOT DONE / SKIPPED
                   }
                }

                width: parent.width
                implicitHeight: 60
                isAdmin: idTestListRoot.isAdmin
                Component.onCompleted: {
                    sigItemToggled.connect(idTestListRoot.slotItemToggled)
                }
            }
        }


        Rectangle
        {
            id: buttonBar
            border.width: 2
            Layout.fillWidth: true
            height: 80

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                RoundBigButton {
                    id: selectAllButton
                    width: 60
                    defaultIcon: "select_all"
                    toggledIcon: "select_all"
                    defaultColor: "steelblue"
                    toggledColor: "steelblue"
                    visible: manufEngine.admin
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Select all tests")

                    onClicked: {
                        manufEngine.selectAllTests(true);
                        idTestListView.setModel(manufEngine.getTestList()); // refresh model
                    }
                }

                RoundBigButton {
                    id: selectNoneButton
                    width: 60
                    defaultIcon: "select_none"
                    toggledIcon: "select_none"
                    defaultColor: "steelblue"
                    toggledColor: "steelblue"
                    visible: manufEngine.admin
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Un-select all tests")

                    onClicked: {
                        manufEngine.selectAllTests(false);
                        idTestListView.setModel(manufEngine.getTestList()); // refresh model
                    }
                }

                Item {
                    // spacer item
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                //    Rectangle { anchors.fill: parent; color: "#ffaaaa" } // to visualize the spacer
                }

                RoundBigButton {
                    id: startButton
                    width: 60
                    defaultIcon: "play"
                    toggledIcon: "play" // Unicode PAUSE
                    defaultColor: enabled ? "DarkSeaGreen" : "black"
                    toggledColor: enabled ? "DarkSeaGreen" : "black"
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Start or continue current script")
                    enabled: !manufEngine.running
                    visible: manufEngine.loaded
                    onClicked: {
                        if (manufEngine.running) {
                            manufEngine.resume();
                        } else {
                          //  messageBar.clear();
                            resetAllStatus();
                            manufEngine.start();
                        }
                    }
                }

                RoundBigButton {
                    id: pauseButton
                    width: 60
                    defaultIcon: "pause"
                    toggledIcon: "pause"
                    defaultColor: enabled ? "DarkSalmon" : "black"
                    toggledColor: enabled ? "DarkSalmon" : "black"
                    visible: (manufEngine.admin && manufEngine.loaded)
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Pause current script")
                    enabled: manufEngine.running
                    onClicked: {
                        manufEngine.pause();
                    }
                }

                RoundBigButton {
                    id: stopButton
                    width: 60
                    defaultIcon: "stop"
                    toggledIcon: "stop"
                    defaultColor: enabled ? "LightBlue" : "black"
                    toggledColor: enabled ? "LightBlue" : "black"
                    enabled: manufEngine.running
                    visible: manufEngine.loaded
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Stop current script")
                    onClicked: {
                        manufEngine.stop();
                    }
                }

                RoundBigButton {
                    id: settingsButton
                    width: 60
                    defaultIcon: "settings"
                    toggledIcon: "settings"
                    defaultColor: enabled ? "bisque" : "black"
                    toggledColor: enabled ? "bisque" : "black"
                    visible: manufEngine.admin
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Peripheral status")
                    enabled: !manufEngine.running
                    onClicked: {
                        devicePopup.open();
                        manufEngine.autoTest();
                    }

                    DeviceStatus {
                        id: devicePopup

                        x: buttonBar.x + 10
                        y: buttonBar.y - 400 + 10
                        width: buttonBar.width
                        height: 400
                    }
                }

            } // fin RowLayout

       } // Fin rectangle

    }
}
