import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: root

    width: 480; height: 480

    property int easingIndex
    property var easings: [
        Easing.Linear,
        Easing.InQuad,
        Easing.OutQuad,
        Easing.InOutQuad,
        Easing.OutInQuad,
        Easing.InCubic,
        Easing.OutCubic,
        Easing.InOutCubic,
        Easing.OutInCubic,
        Easing.InQuart,
        Easing.OutQuart,
        Easing.InOutQuart,
        Easing.OutInQuart,
        Easing.InQuint,
        Easing.OutQuint,
        Easing.InOutQuint,
        Easing.OutInQuint,
        Easing.InSine,
        Easing.OutSine,
        Easing.InOutSine,
        Easing.OutInSine,
        Easing.InExpo,
        Easing.OutExpo,
        Easing.InOutExpo,
        Easing.OutInExpo,
        Easing.InCirc,
        Easing.OutCirc,
        Easing.InOutCirc,
        Easing.OutInCirc,
        Easing.InElastic,
        Easing.OutElastic,
        Easing.InOutElastic,
        Easing.OutInElastic,
        Easing.InBack,
        Easing.OutBack,
        Easing.InOutBack,
        Easing.OutInBack,
        Easing.InBounce,
        Easing.OutBounce,
        Easing.InOutBounce,
        Easing.OutInBounce
    ]
    function easingName(easing) {
        switch (easing) {
            case Easing.Linear: return "Linear"
            case Easing.InQuad: return "InQuad"
            case Easing.OutQuad: return "OutQuad"
            case Easing.InOutQuad: return "InOutQuad"
            case Easing.OutInQuad: return "OutInQuad"
            case Easing.InCubic: return "InCubic"
            case Easing.OutCubic: return "OutCubic"
            case Easing.InOutCubic: return "InOutCubic"
            case Easing.OutInCubic: return "OutInCubic"
            case Easing.InQuart: return "InQuart"
            case Easing.OutQuart: return "OutQuart"
            case Easing.InOutQuart: return "InOutQuart"
            case Easing.OutInQuart: return "OutInQuart"
            case Easing.InQuint: return "InQuint"
            case Easing.OutQuint: return "OutQuint"
            case Easing.InOutQuint: return "InOutQuint"
            case Easing.OutInQuint: return "OutInQuint"
            case Easing.InSine: return "InSine"
            case Easing.OutSine: return "OutSine"
            case Easing.InOutSine: return "InOutSine"
            case Easing.OutInSine: return "OutInSine"
            case Easing.InExpo: return "InExpo"
            case Easing.OutExpo: return "OutExpo"
            case Easing.InOutExpo: return "InOutExpo"
            case Easing.OutInExpo: return "OutInExpo"
            case Easing.InCirc: return "InCirc"
            case Easing.OutCirc: return "OutCirc"
            case Easing.InOutCirc: return "InOutCirc"
            case Easing.OutInCirc: return "OutInCirc"
            case Easing.InElastic: return "InElastic"
            case Easing.OutElastic: return "OutElastic"
            case Easing.InOutElastic: return "InOutElastic"
            case Easing.OutInElastic: return "OutInElastic"
            case Easing.InBack: return "InBack"
            case Easing.OutBack: return "OutBack"
            case Easing.InOutBack: return "InOutBack"
            case Easing.OutInBack: return "OutInBack"
            case Easing.InBounce: return "InBounce"
            case Easing.OutBounce: return "OutBounce"
            case Easing.InOutBounce: return "InOutBounce"
            case Easing.OutInBounce: return "OutInBounce"
        }
    }
    function incEasingIndex() {
        if (easingIndex < easings.length-1)
            easingIndex++
    }
    function decEasingIndex() {
        if (0 < easingIndex)
            easingIndex--
    }
    Component.onCompleted: {
        banner0.opacity = 0.25
        banner1.opacity = 0.25
    }

    NumberAnimation {
        id: animX
        target: text
        properties: "x"
        from: 0
        to: 320
        duration: 5000
        easing.type: easings[easingIndex]
    }
    NumberAnimation {
        id: animY
        target: text
        properties: "y"
        from: animX.from
        to: animX.to
        duration: animX.duration
        easing.type: Easing.Linear
    }

    component OpacityAnimation : SequentialAnimation {
        PauseAnimation { duration: 500 }
        NumberAnimation { duration: 3000; easing.type: Easing.InBack }
    }

    Item {
        id: banner
        width: parent.width; height: parent.height

        Text {
            id: banner0
            anchors.bottom: dummy.top; anchors.bottomMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 32
            font.bold: true
            color: "green"
            text: "QML animations"
            opacity: 0.0
            Behavior on opacity { OpacityAnimation{} }
        }
        Item {
            id: dummy
            width: parent.width; height: 1
            anchors.centerIn: parent
        }
        Text {
            id: banner1
            anchors.top: dummy.bottom; anchors.topMargin: 4
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 24
            font.bold: true
            color: "green"
            text: "github.com/kbdyj1/dailycommit"
            opacity: 0.0
            Behavior on opacity { OpacityAnimation{} }
        }

        Behavior on x { SmoothedAnimation { velocity: 200 } }
        Behavior on y { SmoothedAnimation { velocity: 200 } }

        focus: true
        Keys.onRightPressed: x = x + 100
        Keys.onLeftPressed: x = x - 100
        Keys.onUpPressed: y = y - 100
        Keys.onDownPressed: y = y + 100
    }

    Item {
        x: 80
        y: 40

        Rectangle {
            width: root.width-20; height: 1
            color: "blue"
            transform: Rotation { origin.x: 0; origin.y: 0; angle: 45 }
            opacity: 0.75
        }
        Rectangle {
            id: text
            color: "red"
            width: 8; height: 8; radius: 4
        }
    }

    Row {
        height: 30
        anchors.bottom: aniControl.top
        spacing: 4
        Button {
            width: 60; height: 20
            text: "Prev"
            onClicked: {
                decEasingIndex()
            }
        }
        Button {
            width: 60; height: 20
            text: "Next"
            onClicked: {
                incEasingIndex()
            }
        }
        Text {
            width: 120; height: 20
            text: easingName(easingIndex)
            verticalAlignment: Text.AlignVCenter
        }
    }

    Row {
        id: aniControl
        height: 30
        spacing: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        Button {
            width: 60; height: 20
            text: "Start"
            onClicked: {
                animX.start()
                animY.start()
            }
        }
        Button {
            width: 60; height: 20
            text: "Pause"
            onClicked: {
                animX.pause()
                animY.pause()
            }
        }
        Button {
            width: 60; height: 20
            text: "Resume"
            onClicked: {
                animX.resume()
                animY.resume()
            }
        }
        Button {
            width: 60; height: 20
            text: "Stop"
            onClicked: {
                animX.stop()
                animY.stop()
            }
        }
        Button {
            width: 90; height: 20
            text: "Complete"
            onClicked: {
                animX.complete()
                animY.complete()
            }
        }
        Button {
            width: 30; height: 20
            text: "5"
            onClicked: {
                animX.duration = 5000
            }
        }
        Button {
            width: 30; height: 20
            text: "10"
            onClicked: {
                animX.duration = 10000
            }
        }
    }
}
