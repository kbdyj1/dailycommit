import QtQuick 2.0

Item {
    id: resolvingProperties

    width: 400; height: 200
    Rectangle {
        id: rect

        anchors.fill: parent
        color: "blue"
    }

    function printValue(which, value) {
        console.log(which + " = "  + value)
    }

    function bad() {
        for (var i=0; i<1000; i++) {
            printValue("red", rect.color.r)
            printValue("green", rect.color.g)
            printValue("blue", rect.color.b)
            printValue("alpha", rect.color.a)
        }
    }
    function good() {
        var rectColor = rect.color
        for (var i=0; i<1000; i++) {
            printValue("red", rectColor.r)
            printValue("green", rectColor.g)
            printValue("blue", rectColor.b)
            printValue("alpha", rectColor.a)
        }
    }

    Component.onCompleted: {
        var t0 = new Date();
        {
            //bad()
            good()
        }
        var t1 = new Date();
        console.log("Took: " + (t1.valueOf() - t0.valueOf()) + " milliseconds for 1000 iterations")
    }
}
