import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg

Rectangle {
    visible: true
    anchors.fill: parent

    // Tab buttons
    Rectangle {
        id: tabBar
        x: 10; y: 10
        width: parent.width-20; height: 30
        color: "transparent"

        Row {
            anchors.top: parent.top
            height: 40
            property string selb: "Windows"
            spacing: 10
            x: 0; y: 10; width: root.width-20;
            Button { text: "Windows";     height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; }}
            Button { text: "Cmdline";     height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; qmlInterface.callCmd("change_controls test");}}
            Button { text: "Bookmarks";   height: parent.selb===text? 32:18;
                      onClicked: { parent.selb = text; }}
        }
    }

    // Windows toggle area
    Rectangle {
        id: windowsArea
        x: 10; y: 44
        width: parent.width-20
        height: parent.height - 54
        color: "#D8DDE8"
        clip: true
        visible: tabBar.children[0].selb === "Windows"

        Flow {
            x: 5; y: 5
            width: parent.width - 10
            spacing: 5

            Repeater {
                model: mainTabs
                delegate: Rectangle {
                    visible: index > 0
                    width: visible ? 100 : 0; height: visible ? 30 : 0
                    radius: 2; color: appStyle.barColor
                    Text {
                        anchors.centerIn: parent
                        text: model.icon + " " + model.text
                        font.pointSize: 10; font.bold: true; color: appStyle.textColor
                    }
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.toggleWindow(index)
                    }
                }
            }
        }

        Rectangle {
            anchors.bottom: parent.bottom; anchors.bottomMargin: 5
            x: 5
            width: 160; height: 30; radius: 2; color: "#708090"
            Text { anchors.centerIn: parent; text: "Save Screen Position"; font.pointSize: 10; color: "#fff" }
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: qmlInterface.callCmd("save_window_position")
            }
        }
    }

    // Command buttons area
    Rectangle {
        id: cmdArea
        x: 10; y: 44
        width: parent.width-20
        height: parent.height - 54
        color: "#CAD7E8"
        clip: true
        visible: tabBar.children[0].selb === "Cmdline"

        Flickable {
            anchors.fill: parent
            contentHeight: cmdFlow.implicitHeight
            flickableDirection: Flickable.VerticalFlick
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            Flow {
                id: cmdFlow
                width: parent.width - 15
                spacing: 10

                Repeater {
                    model: cmdline
                    delegate: Loader {
                        sourceComponent: {
                            if (rowType === 0) return textComponent;
                            else if (rowType === 1) return editComponent;
                            else if (rowType === 2) return buttonComponent;
                        }
                        onLoaded: {
                            item.content = content;
                            if (rowType === 1 || rowType === 2)
                                item.command = model.command
                        }
                    }
                }
            }
        }
    }

    // Bookmarks area
    Rectangle {
        id: bookmarksArea
        x: 10; y: 44
        width: parent.width-20
        height: parent.height - 54
        color: "#D8DDE8"
        clip: true
        visible: tabBar.children[0].selb === "Bookmarks"

        Row {
            x: 5; y: 5
            spacing: 10

            Button {
                text: "\u25B2 Up"
                height: 30
                onClicked: qmlInterface.callCmd("bookmark_shift -1")
            }
            Button {
                text: "\u25BC Down"
                height: 30
                onClicked: qmlInterface.callCmd("bookmark_shift 1")
            }
            Button {
                text: "+ Add"
                height: 30
                onClicked: {
                    var item = mdDirModel.selectedItem
                    if (item && item.filePath)
                        qmlInterface.callCmd("bookmark_add " + item.filePath)
                }
            }
        }
    }

    // Components
    Component {
        id: textComponent
        Text { y:6; height: 26; property string content: ""; text: content }
    }
    Component {
        id: editComponent
        TextField {
            height: 26; property string content: ""; property string command: ""
            placeholderText: content
            onAccepted: { qmlInterface.callCmd(command.replace("$text", text)); text = "" }
        }
    }
    Component {
        id: buttonComponent
        Button {
            height: 26; property string content: ""; property string command: ""
            onClicked: { qmlInterface.callCmd(command); }
            text: content
        }
    }
}
///@view:end
