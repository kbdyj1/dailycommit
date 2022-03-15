import QtQuick 2.0

Rectangle {
    id: signal

    width: 200; height: 200
    color: "blue"

    property int count: 0

    function enterNormal() { console.log("entering Normal...") }
    function enterCritical() { console.log("entering Critical...") }

    states: [
        State {
            name: "Normal"
            StateChangeScript { name: "normal"; script: enterNormal() }
            PropertyChanges { target: signal; color: "green" }
            ParentChange { target: clickArea; parent: signal; rotation: 0 }
            AnchorChanges { target: clickArea; anchors.top: signal.top; anchors.left: signal.left }
            PropertyChanges { target: clickArea; anchors.margins: 10 }
        },
        State {
            name: "Critical"
            StateChangeScript { name: "critical"; script: enterCritical() }
            PropertyChanges { target: signal; color: "red" }
            ParentChange { target: clickArea; parent: smallArea }
            PropertyChanges { target: clickArea; anchors.margins: 0 }
        },
        State {
            name: "Stop"
            when: count == 10
            PropertyChanges { target: clickArea; enabled: false }
        }
    ]
    Item {
        id: smallArea

        width: 50; height: 50
        anchors.right: parent.right; anchors.rightMargin: 10
        anchors.bottom: parent.bottom; anchors.bottomMargin: 10
        RotationAnimation on rotation {
            from: 0
            to: 360
            direction: RotationAnimation.Clockwise
            duration: 10000
            loops: Animation.Infinite
        }
    }

    MouseArea {
        id: clickArea

        anchors.fill: parent
        onClicked: {
            if (signal.state === "Normal")
                signal.state = "Critical"
            else
                signal.state = "Normal"

            ++count
        }

        Rectangle {
            anchors.fill: clickArea
            color: 'transparent'
            border.color: 'yellow'
            visible: clickArea.enabled
        }
    }
    Text {
        id: text
        anchors.centerIn: parent
        text: count
        font.bold: true
        font.pointSize: 18
        color: "yellow"
    }
}
