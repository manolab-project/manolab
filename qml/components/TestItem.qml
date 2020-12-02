import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

ItemDelegate
{
    id: delegateComponent
    property int idx: 0
    property string name: "default"
    property string color: "white"
    property bool selected: true
    property bool isAdmin: false;

    signal sigItemToggled(int index, bool active)

    Rectangle
    {
        id: listDelegateRectangle
        anchors.fill: parent
        border.color: "black"
        border.width: 2
        color: delegateComponent.color

        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 4

            CheckBox {
                checked: delegateComponent.selected
                visible: delegateComponent.isAdmin
                onClicked: {
                    delegateComponent.sigItemToggled(delegateComponent.idx, checked);
                }
            }

            Label {
                text: (delegateComponent.idx+1) + ": "  + delegateComponent.name
                font.pixelSize: 16
            }
        }

//            Label {
//                Layout.alignment: Qt.AlignTop
//                visible: false
//                text: "Durée 7 min 08 secondes"
//            }

    }

}
