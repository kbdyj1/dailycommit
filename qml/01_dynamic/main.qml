import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: window

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Item {
        id: root

        property int speed: 0

        width: window.width; height: window.height
        states: [
            State {
                name: "analog"
                PropertyChanges { target: analogButton; color: "green" }
                PropertyChanges { target: mainLoader; source: "Analog.qml" }
            },
            State {
                name: "digital"
                PropertyChanges { target: digitalButton; color: "green" }
                PropertyChanges { target: mainLoader; source: "Digital.qml" }
            }
        ]
        Loader {
            id: mainLoader

            anchors.fill: parent
            anchors.bottomMargin: 32

            onLoaded: {
                binder.target = mainLoader.item
            }
            Row {
                width: parent.width; height: 32
                anchors.top: parent.bottom

                Rectangle {
                    width: parent.width / 2; height: 32
                    color: "grey"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.state = "analog"
                        Text {
                            anchors.centerIn: parent
                            text: "Analog"
                        }
                    }
                }
                Rectangle {
                    width: parent.width / 2; height: 32
                    color: "grey"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: root.state = "digital"
                        Text {
                            anchors.centerIn: parent
                            text: "Digital"
                        }
                    }
                }
            }
        }
        Binding {
            id: binder
            property: "speed"
            value: speed
        }
    }
}
