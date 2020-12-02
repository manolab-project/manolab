import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Popup {
    id: popup

    parent: Overlay.overlay
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside


    function setModel(elements)
    {
        comModel.clear();
        for (var i = 0; i < elements.length; i++)
        {
            comModel.append({"title": elements[i].title, "active": elements[i].active, "status": 0 });
        }
    }

    ListModel
    {
        id: comModel
    }

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
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                model: comModel
                ScrollBar.vertical: ScrollBar {
                    active: true
                    background:  Rectangle {
                        implicitWidth: 20
                        color:  "#F0F0F0"
                    }
                }
                Keys.onDownPressed: listView.incrementCurrentIndex();
                Keys.onUpPressed: listView.decrementCurrentIndex();

                delegate: Text {
                    width: parent.width
                    height: 20
                    text: title
                }
            }
        }
    }
}
