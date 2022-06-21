import QtQuick 2.0

Rectangle {
    width: 640; height: 480
    color: "lightyellow"

    Column {
        id: column
        property bool mirroring: false
        readonly property string meText: "خامل"
        readonly property string l2r: "\u200E"
        readonly property string r2l: "\u200F"
        LayoutMirroring.enabled: mirroring
        LayoutMirroring.childrenInherit: true
        spacing: 8

        Text {
            text: "1 " + column.meText
            width: 200
        }
        Text {
            text: column.l2r + "2 " + column.meText
            width: 200
        }
        Text {
            text: column.r2l + "3 " + "Hello, Qt"
        }
        Text {
            text: "4 " + column.meText
            horizontalAlignment: Text.AlignLeft
            width: 200
        }
        Text {
            text: "5 " + column.meText
            horizontalAlignment: Text.AlignRight
            //LayoutMirroring.enabled: true
            width: 200
        }
        Text {
            text: column.l2r + "6 " + column.meText
        }
        Text {
            text: column.r2l + "7 " + column.meText
        }
    }
    Rectangle {
        anchors.top: column.top
        width: 32; height: 16; color: 'red'
        Text {
            anchors.centerIn: parent
            color: 'white'
            text: 'COL'
        }
        MouseArea {
            anchors.fill: parent
            onClicked: column.mirroring = !column.mirroring
        }
    }
    Row {
        id: row
        anchors.top: column.bottom; anchors.topMargin: 16
        width: parent.width
        property bool mirroring: false

        LayoutMirroring.enabled: mirroring

        Rectangle {
            width: 64; height: 64; color: "yellow"
            Text {
                text: "1"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    row.layoutDirection = Qt.LeftToRight
                }
            }
        }
        Rectangle {
            width: 64; height: 64; color: "blue"
            Text {
                text: "2"
                color: "white"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    row.mirroring = !row.mirroring
                }
                Text {
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                    width: parent.width; height: parent.height-8
                    text: "<-> " + row.mirroring
                    color: "white"
                }
            }
        }
        Rectangle {
            width: 64; height: 64; color: "red"
            Text {
                text: "3"
                color: "white"
                anchors.centerIn: parent
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    row.layoutDirection = Qt.RightToLeft
                }
            }
        }
    }
    Rectangle {
        anchors.fill: row
        color: 'transparent'; border.color: 'black'
        Rectangle {
            width: 32; height: 16; color: 'black'
            Text {
                anchors.centerIn: parent
                color: 'white'
                text: 'ROW'
            }
        }
    }
}
