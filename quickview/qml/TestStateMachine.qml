import QtQuick 2.0

Rectangle {
    id: root

    width: 640; height: 480
    color: "lightgreen"

    signal clicked()
    signal exited()

    Rectangle {
        width: parent.width / 2
        height: parent.height / 2
        anchors.centerIn: parent
        color: "orange"

        Text {
            text: "Push"
            color: "black"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: root.clicked()
        }
    }
    Rectangle {
        width: 32
        height: 32
        anchors.top: parent.top
        anchors.right: parent.right
        color: "white"

        Text {
            text: "X"
            color: "black"
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: root.exited()
        }
    }
}
