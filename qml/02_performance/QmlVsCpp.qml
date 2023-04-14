import QtQuick 2.0

Item {
    function compareArray(left, right) {
        return left.length === right.length && left.every(function(value, index){
            return value.toFixed(6) === right[index].toFixed(6)
        })
    }

    /*
     * Sqrt (QML): 11778ms
     * Sqrt (Cpp): 104ms (concurrent)
     * Sqrt (Cpp): 936ms
     */
    function test() {
        var values = cpp.values()

        console.time("Sqrt (QML)")
        var jsOutput = []
        for (let y = 0; y < values.length; y++) {
            jsOutput.push(Math.sqrt(values[y]))
        }
        console.timeEnd("Sqrt (QML)")

        console.time("Sqrt (Cpp)")
        var cppOutput = cpp.calculateSqrt()
        console.timeEnd("Sqrt (Cpp)")

        console.time("Sqrt (Cpp)")
        var cppOutputAsync = cpp.calculateSqrtAsync()
        console.timeEnd("Sqrt (Cpp)")

        console.assert(compareArray(jsOutput, cppOutput), "Output differs!")
        console.assert(compareArray(jsOutput, cppOutputAsync), "Output differs!")
    }

    Rectangle {
        id: bg

        anchors.fill: parent

        color: "red"

        MouseArea {
            anchors.fill: parent

            onClicked: {
                Qt.callLater(bg.changeBackground, "yellow")
                Qt.callLater(bg.changeBackground, "blue")
                Qt.callLater(bg.changeBackground, "green")
            }
        }

        function changeBackground(newColor) {
            console.log("changedBackground(" + newColor + ")")
            bg.color = newColor
        }
    }

    Component.onCompleted: {
        test()
    }
}
