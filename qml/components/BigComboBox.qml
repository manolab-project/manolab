import QtQuick 2.6
import QtQuick.Controls 2.1

ComboBox {
    id: control

    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: modelData
            color: "black"
            font: control.font
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: control.pressed ? "black" : "black"
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: "transparent"
        border.width: 1
        border.color: "black"
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "black"
            radius: 2
        }
    }
}
