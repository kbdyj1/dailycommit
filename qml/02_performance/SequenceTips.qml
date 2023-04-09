import QtQuick 2.0
import Performance.Example 1.0

SequenceType {
    id: root
    width: 200; height: 200

    readonly property int testLoopCnt: 500

//    property int first: qrealList[1] + 10
//    property int second:qrealList[1] + 20
//    property int third: qrealList[1] + 30

    property int intermediateBinding: qrealList[1]
    property int first: intermediateBinding + 10
    property int second:intermediateBinding + 20
    property int third: intermediateBinding + 30

    function bad() { //elapsed: 53 ms
        qrealList.length = 100;
        for (var i=0; i<testLoopCnt; i++) {
            for (var j=0; j<100; j++) {
                qrealList[j] = j;
            }
        }
    }

    function good() { //[qml] elapsed: 5 ms
        var someData = [1.1, 1.2, 1.3]
        someData.length = 100
        for (var i=0; i<testLoopCnt; i++) {
            for (var j=0; j<100; j++) {
                someData[j] = j
            }
        }
        qrealList = someData
    }

    function bad2() { //elapsed: 5 ms
        for (var i=0; i<1000; i++) {
            qrealList[2] = i;
        }
    }

    Component.onCompleted: {
        var t0 = new Date();
        {
            //bad()
            //good()

            bad2()
        }
        var t1 = new Date();
        console.log("elapsed: " + (t1.valueOf() - t0.valueOf()) + " ms");
    }
}
