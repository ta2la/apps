import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    anchors.fill: parent
    color: "#FAFAF0"
    focus: true

    Flickable {
        anchors.fill: parent
        anchors.margins: 10
        contentHeight: mdFlow.implicitHeight + 20
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

        Flow {
            id: mdFlow
            width: parent.width
            spacing: 4

            Repeater {
                model: mdModel
                delegate: Rectangle {
                    width: wordText.implicitWidth + 8
                    height: wordText.implicitHeight + 4
                    radius: 2
                    color: model.isCursor ? "#FFD700" : "transparent"

                    Text {
                        id: wordText
                        anchors.centerIn: parent
                        text: model.text
                        font.pointSize: model.type === 0 ? 18 : model.type === 1 ? 14 : model.type === 2 ? 12 : 10
                        font.bold: model.type <= 2 || model.style === 1
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: mdModel.setCursorIndex(index)
                    }
                }
            }
        }
    }

    Keys.onLeftPressed:  mdModel.setCursorIndex(mdModel.cursorIndex - 1)
    Keys.onRightPressed: mdModel.setCursorIndex(mdModel.cursorIndex + 1)
    Keys.onUpPressed:    mdModel.setCursorIndex(mdModel.cursorIndex - 5)
    Keys.onDownPressed:  mdModel.setCursorIndex(mdModel.cursorIndex + 5)
}
