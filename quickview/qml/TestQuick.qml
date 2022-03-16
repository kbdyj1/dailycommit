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
            StateChangeScript { name: "playBanner"; script: playBanner.start() }
        }
    ]
    // animates transitions during state changes
    Behavior on color { ColorAnimation { target: signal; duration: 250 } }

    transitions: [
        Transition {
            from: "Normal"; to: "Critical";
            NumberAnimation { target: text; properties: "font.pixelSize"; to: 36; easing: Easing.InBack; duration: 200 }
        },
        Transition {
            from: "Critical"; to: "Normal";
            NumberAnimation { target: text; properties: "font.pixelSize"; to: 18; easing: Easing.OutElastic; duration: 200 }
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
    Item {
        id: banner
        anchors.fill: parent
        Column {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 16
            spacing: 4
            Text {
                id: textAnimation
                text: "Animation"
                opacity: 0.0
                color: "white"
            }
            Text {
                id: textDone
                text: "Done"
                opacity: 0.0
                color: "white"
            }
            Text {
                id: textByMe
                text: "by ymhong"
                opacity: 0.0
                color: "white"
            }
        }
        SequentialAnimation {
            id: playBanner
            running: false
            NumberAnimation { target: textAnimation; properties: "opacity"; to: 0.75; duration: 200 }
            NumberAnimation { target: textDone; properties: "opacity"; to: 0.75; duration: 200 }
            NumberAnimation { target: textByMe; properties: "opacity"; to: 0.75; duration: 200 }
        }
    }
}
