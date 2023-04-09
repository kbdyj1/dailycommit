import QtQuick 2.0

Item {
    id: propertyBindings

    width: 200; height: 200
    property int accumulatedValue: 0

    Text {
        anchors.fill: parent
        text: propertyBindings.accumulatedValue.toString()
        onTextChanged: {
            console.log("text binding re-evaluated")
        }
    }
    function bad() {
        var someData = [ 1, 2, 3, 4, 5, 20 ]
        for (var i=0; i<someData.length; i++) {
            accumulatedValue = accumulatedValue + someData[i]
        }
    }
    function good() {
        var someData = [ 1, 2, 3, 4, 5, 20 ]
        var sum = 0
        for (var i=0; i<someData.length; i++) {
            sum = sum + someData[i]
        }
        accumulatedValue = sum
    }

    Component.onCompleted: {
        //bad()
        good()
    }
}
