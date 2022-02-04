import QtQuick 2.15
import QtQuick.Controls 2.0
import Login 1.0

View {
    anchors.fill: parent

    property alias title: text.text
    readonly property int layoutItemWidth: width * (2.0/4.0)
    readonly property int layoutItemHeight: 32
    readonly property int ringRadius: layoutItemWidth + 128
    readonly property int ringWidth: 16

    Component.onCompleted: {
        console.log("Login.onCompleted")
        createAction(Login.Created, {})
    }

    component InputBox : Row {
        id: inputBox

        readonly property int defaultTextWidth: 64
        property alias textWidth: label.width
        property alias textLabel: label.text
        property alias textInput: input.text
        property alias echoMode: input.echoMode
        property alias passwordCharacter: input.passwordCharacter

        width: parent.width
        height: layoutItemHeight

        Text {
            id: label
            width: defaultTextWidth
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }
        TextInput {
            id: input
            width: parent.width - textWidth
            height: parent.height
            verticalAlignment: TextInput.AlignVCenter
            leftPadding: 4.0
            Rectangle {
                anchors.fill: parent
                color: 'transparent'
                border.color: 'lightgray'
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: 'lightsteelblue'

        Text {
            id: text
            y: 16
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle {
            id: ring
            width: ringRadius; height: width
            radius: ringRadius/2
            border.width: ringWidth; border.color: "white"
            color: "transparent"
            anchors.centerIn: parent
            NumberAnimation on opacity {
                duration: 5000
                from: 0.0
                to: 0.5
                easing.type: Easing.InCubic
            }
        }
        Rectangle {
            id: circle
            width: ringWidth-2; height: width
            radius: width/2
            color: "white"
            opacity: 0.75

            property int deg:0

            NumberAnimation on deg {
                from: 0
                to: 3600
                duration: 30*1000
                loops: Animation.Infinite
            }
            onDegChanged: {
                var rad = 2.0 * deg * (Math.PI/3600.0)
                circle.x = parent.width/2 + Math.cos(rad) * (ringRadius/2 - circle.width/2) - circle.width/2
                circle.y = parent.height/2 + Math.sin(rad) * (ringRadius/2 - circle.width/2) - circle.height/2
            }
        }

        Column {
            id: column

            anchors.centerIn: parent

            width: layoutItemWidth

            spacing: 8
            InputBox {
                id: inputId
                textLabel: "id"
            }
            InputBox {
                id: inputPassword
                textLabel: "password"
                echoMode: TextInput.Password
                passwordCharacter: '*'
            }
        }

        Button {
            anchors.left: column.left; anchors.right: column.right
            anchors.top: column.bottom; anchors.topMargin: 32
            height: layoutItemHeight
            text: "Login"

            onClicked: {
                createAction(Login.CheckUser, {'id':inputId.textInput, 'password':inputPassword.textInput})
            }
        }
    }
    Text {
        y: parent.height
        width: layoutItemWidth
        height: layoutItemHeight
        anchors.horizontalCenter: parent.horizontalCenter
        color: "white"
        text: "Rootine Software. 2022"
        horizontalAlignment: Text.AlignHCenter

        NumberAnimation on y {
            id: companyAnim

            from: parent.height
            to: parent.height - layoutItemHeight
            duration: 1000
            easing.type: Easing.OutBack
            running: false
        }
        Timer {
            id: companyTimer
            running: true
            interval: 1000
            onTriggered: {
                companyAnim.running = true
            }
        }
    }
}
