import QtQuick 2.0
import "my_button_impl.js" as Logic
import "factorial.js" as Factorial

Rectangle {
    id: rect

    width: 200; height: 100
    color: "red"

    property int number: 0

    property int edgePosition

    // type annotations
    function factorialCount() : int {
        return Factorial.factorialCallCount()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Logic.onClicked(rect)
            number += 1
            Logic.logButtonProperties(rect)

            console.log('factorialCount():' + factorialCount())

            // Qt object
            console.log(Qt.platform.os) //qml: linux
            console.log(Qt.application.name) //qml: quickview
            console.log(Qt.uiLanguage)
            console.log(Qt.alpha("red", 0.5)) //qml: #80ff0000

            var date = new Date()
            console.log(Qt.formatDate(date, 'ddd MMMM d yy')) //qml: Tue January 25 22
            console.log(Qt.md5(date)) //f47b5ea09507f7241c7879ca1884a40c
        }
        onDoubleClicked: {
            Qt.openUrlExternally("https://blog.qt.io")

            Qt.exit(0) //Signal QQmlEngine::exit() emitted, but no receivers connected to handle it.
        }
    }
    Text {
        anchors.centerIn: parent
        text: number + ":" + Factorial.factorial(number)
        property color parentColor: (parent as Rectangle)?.color || "#00000000"
    }

    Component.onCompleted: {
        edgePosition = Qt.binding(function(){ return x + width })
    }
}
