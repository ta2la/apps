import QtQuick 2.15
import QtQuick.Controls 2.15

///@view:beg

// Full-screen blocking overlay shown until a DB profile is active.
// 3-step state machine driven by craseBootstrap.status:
//   "idle" / "picking" / "pick-error"  → pick directory
//   "picked"                            → choose: download remote OR open local
//   "downloading" / "loading"           → progress
//   "connected" / db_use elsewhere      → invisible (overlay hides)
//
// craseBootstrap is registered as a Q_OBJECT context property in Main.cpp.

Rectangle {
    id: root
    anchors.fill: parent
    color: "#A0000000"
    z: 1000
    visible: typeof dbProfiles !== "undefined" && dbProfiles.active.length === 0

    // swallow all clicks/wheel so MDI underneath is inert
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        onPressed: function(){}
        onWheel:   function(){}
    }

    Rectangle {
        id: dialog
        anchors.centerIn: parent
        width: parent.width * 2 / 3
        height: parent.height * 2 / 3
        radius: 8
        color: "#F0F0F0"
        border.color: "#A0A8B0"; border.width: 1

        // ── Title ──
        Text {
            id: title
            anchors.top: parent.top; anchors.topMargin: 18
            anchors.horizontalCenter: parent.horizontalCenter
            text: {
                var s = craseBootstrap.status
                if (s === "idle" || s === "picking" || s === "pick-error") return "Connect a database"
                if (s === "picked")       return "Directory: " + craseBootstrap.dirName
                if (s === "downloading")  return "Downloading…"
                if (s === "loading")      return "Loading…"
                return "Bootstrap"
            }
            font.pointSize: 21; font.bold: true; color: "#808080"
        }
        Text {
            anchors.top: title.bottom; anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            text: {
                var s = craseBootstrap.status
                if (s === "idle" || s === "picking") return "Pick a local directory (Chrome / Edge only)."
                if (s === "pick-error") return "Pick failed. Click again."
                if (s === "picked") return "Choose a database to download or open."
                if (s === "load-error" || s === "download-error" || s === "list-error")
                    return "Something failed. Try again."
                return ""
            }
            font.pointSize: 14; color: "#707070"
        }

        // ── State A: pick directory ──
        Column {
            visible: ["idle","picking","pick-error"].indexOf(craseBootstrap.status) >= 0
            anchors.centerIn: parent
            spacing: 18
            Rectangle {
                id: pickBtn
                anchors.horizontalCenter: parent.horizontalCenter
                width: 240; height: 44; radius: 4
                color: pickMa.containsMouse ? "#506FAA" : "#406090"
                opacity: craseBootstrap.status === "picking" ? 0.6 : 1
                Text { anchors.centerIn: parent; text: craseBootstrap.status === "picking" ? "Picking…" : "Pick directory"; font.pointSize: 12; font.bold: true; color: "#fff" }
                MouseArea {
                    id: pickMa; anchors.fill: parent; hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    enabled: craseBootstrap.status !== "picking"
                    onClicked: craseBootstrap.pickDir()
                }
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.parent.width * 0.8
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
                color: "#555555"
                text: appText.pickDirNotes
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.parent.width * 0.8
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                color: "#555555"
                text: appText.pickDirNoWarranty
            }
        }

        // ── State B: dir picked, choose source ──
        Item {
            visible: craseBootstrap.status === "picked" || craseBootstrap.status === "list-error"
            anchors.fill: parent
            anchors.topMargin: 80
            anchors.bottomMargin: 18
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            Row {
                anchors.fill: parent
                spacing: 12

                // Left: download remote
                Rectangle {
                    width: (parent.width - 12) / 2
                    height: parent.height
                    color: "#FAFCFF"
                    border.color: "#C0C8D0"
                    radius: 4

                    Text {
                        anchors.top: parent.top; anchors.topMargin: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "⬇ Download from net"
                        font.pointSize: 11; font.bold: true; color: "#406090"
                    }
                    ListView {
                        anchors.fill: parent
                        anchors.topMargin: 36; anchors.bottomMargin: 6
                        anchors.leftMargin: 6; anchors.rightMargin: 6
                        clip: true
                        model: craseBootstrap.remoteNames
                        delegate: Rectangle {
                            width: ListView.view.width; height: 28
                            color: rMa.containsMouse ? "#D0D8E0" : "transparent"
                            Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 6; text: modelData; font.pointSize: 9 }
                            MouseArea {
                                id: rMa; anchors.fill: parent; hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: craseBootstrap.openRemote(index)
                            }
                        }
                    }
                }

                // Right: open existing in dir
                Rectangle {
                    width: (parent.width - 12) / 2
                    height: parent.height
                    color: "#FAFCFF"
                    border.color: "#C0C8D0"
                    radius: 4

                    Text {
                        anchors.top: parent.top; anchors.topMargin: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "📂 Open from dir"
                        font.pointSize: 11; font.bold: true; color: "#406090"
                    }
                    Text {
                        visible: craseBootstrap.localFiles.length === 0
                        anchors.centerIn: parent
                        text: "(no .db files in dir)"
                        color: "#909090"; font.pointSize: 9; font.italic: true
                    }
                    ListView {
                        anchors.fill: parent
                        anchors.topMargin: 36; anchors.bottomMargin: 6
                        anchors.leftMargin: 6; anchors.rightMargin: 6
                        clip: true
                        model: craseBootstrap.localFiles
                        delegate: Rectangle {
                            width: ListView.view.width; height: 28
                            color: lMa.containsMouse ? "#D0D8E0" : "transparent"
                            Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 6; text: modelData; font.pointSize: 9 }
                            MouseArea {
                                id: lMa; anchors.fill: parent; hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: craseBootstrap.openLocal(modelData)
                            }
                        }
                    }
                }
            }
        }

        // ── State C: in-progress ──
        Text {
            visible: craseBootstrap.status === "downloading" || craseBootstrap.status === "loading"
            anchors.centerIn: parent
            text: "⏳"
            font.pointSize: 40; color: "#406090"
        }
    }
}
///@view:end
