import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Rectangle
{
    height: 50
    id: messageBar

    function getColor(level)
    {
        if (level === "Error") {
            return "red";
        } else if (level === "Warning") {
            return "orange";
        } else {
            return "black";
        }
    }

    function scrollViewToEnd()
    {
        var newIndex = listView.count - 1; // last index
        listView.positionViewAtEnd();
        listView.currentIndex = newIndex;
     }

    function setMessage(msg)
    {
        var strings = msg.split(",");

        //                    "Error" "Server"  "Info" "Network"  "Script"
        if (strings.length > 0) {
            messageItem.color = getColor(strings[0]);
        }
        messageItem.text = msg;
        messageModel.append({"message" : msg, "level": strings[0]});
        scrollViewToEnd();
    }

    function clear()
    {
        messageItem.text = "";
        messageModel.clear();
    }

    ListModel
    {
        id: messageModel
    }

    RowLayout
    {
        anchors.fill: parent

        Label {
            id: messageItem
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: "OK";
            color: "darkgreen"
            font.bold: true
            verticalAlignment: Label.AlignVCenter
        }

        Button {
            text: "History"

            onClicked: popup.open()

            Popup {
                id: popup

                parent: Overlay.overlay

                x: messageBar.x + 10
                y: messageBar.y - 400 + 10
                width: messageBar.width
                height: 400

                Rectangle
                {
                    anchors.fill: parent
                    border.color: "black"
                    border.width: 2
                    color: "aliceblue"

                    ColumnLayout
                    {
                        anchors.fill: parent
                        anchors.margins: 10

                        ListView
                        {
                            id: listView
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            clip: true
                            model: messageModel
                            ScrollBar.vertical: ScrollBar {
                                active: true
                                background:  Rectangle {
                                    implicitWidth: 20
                                    color:  "#F0F0F0"
                                }
                            }
                            Keys.onDownPressed: listView.incrementCurrentIndex();
                            Keys.onUpPressed: listView.decrementCurrentIndex();

                            delegate: TextEdit {
                                width: parent.width
                                text: message
                                readOnly: true
                                height: 20
                                color: messageBar.getColor(level)
                            }
                        }
                    }
                }

            }
        }

    }
}
