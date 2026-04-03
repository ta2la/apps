import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    anchors.fill: parent
    color: "#FAFAF0"
    focus: true

    ListView {
        id: listView
        anchors.fill: parent
        anchors.margins: 10
        spacing: 6
        model: mdModel
        clip: true

        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

        delegate: Flow {
            id: itemDelegate
            width: listView.width
            spacing: 4
            topPadding: model.type <= 2 ? 8 : 0

            property int itemIndex: index
            property int itemType: model.type

            Repeater {
                model: wordModel
                delegate: Rectangle {
                    width: wordText.implicitWidth + 6
                    height: wordText.implicitHeight + 2
                    radius: 2
                    color: (mdModel.cursorItem === itemDelegate.itemIndex
                         && mdModel.cursorWord === index)
                         ? "#FFD700" : "transparent"

                    Text {
                        id: wordText
                        anchors.centerIn: parent
                        text: model.text
                        font.pointSize: itemDelegate.itemType === 0 ? 18 : itemDelegate.itemType === 1 ? 14 : itemDelegate.itemType === 2 ? 12 : 10
                        font.bold: itemDelegate.itemType <= 2 || model.style === 1
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: mdModel.setCursor(itemDelegate.itemIndex, index)
                    }
                }
            }
        }
    }

    Keys.onLeftPressed:  mdModel.moveCursor(-1)
    Keys.onRightPressed: mdModel.moveCursor(1)
}
