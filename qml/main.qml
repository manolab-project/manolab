import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import fr.novea.manuf 1.0

import "."
import "./components"

Window {
    id: mainWindow
    visible: true
    visibility: Window.Maximized
    width: 1120
    height: 800
    title: qsTr("NovProd 1.6.3") + " - workspace: " + manufEngine.workspace
    color: manufEngine.admin ? "red" : "lightgrey";

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "Ctrl+F"
        onActivated: {
            if (mainWindow.visibility === Window.FullScreen) {
                mainWindow.showNormal();
            } else {
                mainWindow.showFullScreen();
            }
        }
    }

    ManufEngine {
        id: manufEngine
        property int currentTest: 0
        property int currentStep: 0
        property int totalSteps: 0

        onSigMessage: {
            messageBar.setMessage(msg);
        }

        onSigDelay: {
            if (value === 0) {
                stepProgress.setDelay(currentStep, "");
            } else {
                stepProgress.setDelay(currentStep, value.toString());
            }
        }

        onSigTest: {
            currentTest = number;
            currentStep = 0;
            totalSteps = steps;
            currentTestTitle.text = "";
            testList.setTestStatus(number, 1);

            stepModel.clear();
            for (var i = 0; i < steps; i++) {
                stepModel.append({
                      status: 0,
                      busyEnable: false,
                      delayText: ""
                     });
            }
        }

        onSigStep: {

            if (isStart) {
                var testNumber = currentTest + 1;
                var stepNumber = currentStep + 1;

                currentTestTitle.text = "Test: " + testNumber.toString() + ". " + manufEngine.getTestTitle(currentTest) + " -- Step: " + stepNumber.toString() + ". "+ title;
                // Indicate current step status
                stepProgress.setStepStatus(currentStep, 1);
                stepProgress.setDelayVisible(currentStep, true);

            } else {

                stepProgress.setStepStatus(currentStep, 2);
                stepProgress.setDelayVisible(currentStep, false);

                if (currentStep < (totalSteps-1)) {
                    currentStep++;
                }
            }
        }

        onSigTestEnded: {
            testList.setTestStatus(currentTest, 2);
        }

        onSigTestSkipped: {
          //  testList.setTestStatus(currentTest, 0);
        }

        onSigTestError: {
            testList.setTestStatus(currentTest, 3);
            stepProgress.setStepStatus(currentStep, 3);
            stepProgress.setDelayVisible(currentStep, false);
        }

        onSigScriptLoaded: {
            projectTitle.text = manufEngine.title;
            testList.setModel(manufEngine.getTestList());
        }

        onSigFinished: {
            testList.testFinishedEvent();
        }

        onSigInputText: {
            idDisplayPanel.inputTextEvent(showControls, text);
        }

        onSigAutoTestFinished: {
            testList.updateComList();
        }

        onSigShowImage: {
            if (show)
            {
                idDisplayPanel.setImage(getImage());
            }
            else
            {
                idDisplayPanel.setImage("");
            }
        }

    }

    // ---------------------------------------------------------------------------------
    // MAIN LAYOUT
    // ---------------------------------------------------------------------------------
    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4

        // ---------------------------------------------------------------------------------
        // ZONE D'INFORMATION SUR LE DEROULEMENT DU SCRIPT
        // ---------------------------------------------------------------------------------
        Rectangle
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            border.color: "black"
            border.width: 2
            height: 150

            ColumnLayout
            {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                RowLayout {
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    spacing: 10

                    BigComboBox {
                        id: fileList
                        width: 600
                        model: manufEngine.files
                    }

                    Button
                    {
                        text: qsTr("Load")
                        Layout.fillHeight: true
                        enabled: !manufEngine.running
                        onClicked: {
                            stepModel.clear();
                            manufEngine.loadScript(fileList.currentText);
                        }
                    }

                    Label {
                        id: projectTitle
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true
                        text: "-"
                        font.pixelSize: 20
                        elide: Label.ElideRight
                    }
                }
                Label {
                    id: currentTestTitle
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    text: "-" //"Test 4 sur 10 - durée totale : 14 min 17 secondes"
                    font.pixelSize: 12
                }

                RowLayout {
                    Layout.alignment: Qt.AlignBottom
                    ListView {
                        function setStepStatus(index, status)
                        {
                            stepModel.setProperty(index, "status", status);
                        }

                        function setDelayVisible(index, isVisible)
                        {
                            stepModel.setProperty(index, "busyEnable", isVisible);
                        }

                        function setDelay(index, delay)
                        {
                            stepModel.setProperty(index, "delayText", delay);
                        }

                        function getColor(status)
                        {
                            if (status === 1) {
                                return "steelblue";
                            } else if (status === 2) {
                                return "green";
                            } else if (status === 3) {
                                return "red";
                            } else {
                                return "white";
                            }
                        }

                        Layout.fillWidth: true
                        orientation: ListView.Horizontal
                        height: 60
                        id: stepProgress
                        model: ListModel {
                            id: stepModel
                        }
                        delegate: ItemDelegate {
                            Rectangle {
                               // anchors.fill: parent
                                width: 60; height: 60
                                border.width: 2
                                color: stepProgress.getColor(status)
                                border.color: "black"
                                radius: 4
                                BusyIndicator {
                                    id: busyIcon
                                    running: busyEnable
                                    background: Text {
                                        id: delayIndicator
                                        visible: true
                                        text: delayText
                                        anchors.centerIn: busyIcon
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // ---------------------------------------------------------------------------------
        // CENTRAL ZONE WITH ALL STEPS AND USER INTERACTION
        // ---------------------------------------------------------------------------------
        RowLayout
        {
            Layout.fillHeight: true
            Layout.fillWidth: true

            TestList {
                id: testList
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                implicitWidth: mainWindow.width / 2
                border.color: "black"
                border.width: 2
                isAdmin: manufEngine.admin
                onSigItemToggled: {
                    manufEngine.selectOneTest(index, active);
                }
            }

            DisplayPanel
            {
                id: idDisplayPanel
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight
                border.color: "black"
                border.width: 2
            }
        }

        // ---------------------------------------------------------------------------------
        // BARRE DE MENU (MESSAGES)
        // ---------------------------------------------------------------------------------
        MessageBar
        {
            id: messageBar
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
        }
    }
}
