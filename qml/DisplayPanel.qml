import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

import "./components"

Rectangle
{
    function setImage(img) {
        imageDisplay.source = img;
    }

    function inputTextEvent(showControls, text) {
        acceptedButton.visible = showControls;
        rejectedButton.visible = showControls;
        cleanButton.visible = showControls;
        userInputText.enabled = showControls;
        inputRectangle.visible = showControls;

        if (showControls) {
            userInputText.forceActiveFocus();
        }
        textMessage.text = text;
    }

    ColumnLayout
    {
        anchors.margins: 10
        anchors.fill: parent

        Rectangle {
            id: textMessageContainer
            border.width: 2
            Layout.fillWidth: true
            border.color: "black"
            Layout.alignment: Qt.AlignTop
            height: 200
            anchors.margins: 10

            Text {
                id: textMessage
                anchors.margins: 10
                font.pointSize: 14
            }
        }

        Image {

            id: imageDisplay

            fillMode: Image.PreserveAspectFit
            Layout.alignment: Qt.AlignBottom
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: TextEdit.AlignVCenter
        }

        Rectangle
        {
            id: buttonBar2
            border.width: 2
            Layout.fillWidth: true
            height: 80

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Rectangle {
                    id: inputRectangle
                    border.width: 2
                    border.color: "black"
                    Layout.fillWidth: true
                    height: 40
                    color: "transparent"
                    visible: false

                    TextInput {
                        id: userInputText

                        function acceptInputText()
                        {
                            manufEngine.acceptInputText(text, true);
                            userInputText.text = "";
                            textMessage.text = "";
                        }

                        function cleanInputText()
                        {
                            text = "";
                        }

                        function rejectInputText()
                        {
                            text = "";
                            textMessage.text = "";
                            manufEngine.acceptInputText(text, false);
                        }

                        focus: true
                        anchors.margins: 10
                        enabled: false
                        selectByMouse: true
                        text: ""
                        font.pointSize: 16
                        font.bold: true
                        onAccepted: {
                            acceptInputText();
                        }
                    }
                }

                 RoundBigButton {
                     id: cleanButton
                     width: 60
                     defaultIcon: "clean"
                     toggledIcon: "clean"
                     defaultColor: "lightblue"
                     toggledColor: "lightblue"
                     visible: false
                     onClicked: {
                        userInputText.cleanInputText();
                     }
                 }

                 RoundBigButton {
                     id: acceptedButton
                     width: 60
                     defaultIcon: "good"
                     toggledIcon: "good"
                     defaultColor: "green"
                     toggledColor: "green"
                     visible: false
                     onClicked: {
                        userInputText.acceptInputText();
                     }
                 }

                 RoundBigButton {
                     id: rejectedButton
                     width: 60
                     defaultIcon: "bad"
                     toggledIcon: "bad"
                     defaultColor: "red"
                     toggledColor: "red"
                     visible: false
                     onClicked: {
                        userInputText.rejectInputText();
                     }
                }
            } // RowLayout
        } // Rectangle
    }
}
