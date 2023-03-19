import QtQuick

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Item {
        width: window.width; height: window.height

        Item {
            width: parent.width/2
            height: parent.height

            Rectangle {
                anchors.centerIn: parent

                width: 100; height: 50
                color: 'grey'

                Text {
                    anchors.centerIn: parent
                    text: "On"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: remocon.turnOn()
                }
            }
        }

        Item {
            width: parent.width/2
            height: parent.height
            anchors.right: parent.right

            Rectangle {
                anchors.centerIn: parent

                width: 100; height: 50
                color: 'grey'

                Text {
                    anchors.centerIn: parent
                    text: "Off"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: remocon.turnOff()
                }
            }
        }
    }
}
