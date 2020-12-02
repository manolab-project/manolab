import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Button {
    id: bigButton
    property string defaultColor: "white"
    property string toggledColor: "white"
    property string defaultText: ""
    property string toggledText: ""
    property string defaultIcon: ""
    property string toggledIcon: ""
    property  int status: 0
    height: width
    icon.name: bigButton.status === 0 ? bigButton.defaultIcon : bigButton.toggledIcon

    background:  Rectangle {
        implicitWidth: bigButton.width
        implicitHeight: width
        color: bigButton.status === 0 ? bigButton.defaultColor : bigButton.toggledColor
        border.color: "black"
        border.width: bigButton.activeFocus ? 1 : 2
        radius: width * 0.5
   }
}
