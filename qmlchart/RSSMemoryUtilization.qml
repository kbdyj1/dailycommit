import QtQuick 2.0
import "component/"

Item {
    property string title: "RSS memory utilization"

    Text {
        x:8; y:8

        text: title
    }
    Line {
        width: parent.width
        color: "lightgrey"
        y: 32
    }
    ListView {
        id: mainGraph

        y: 64
        width: parent.width; height: 24 * 5
        model: [ 2.5, 2, 1.5, 1, 0.5, 0 ]
        delegate: Item {
            width: parent.width - 32
            height: 24
            anchors.right: parent.right

            Line {
                id: line
                width: parent.width
                anchors.right: parent.right
                color: "#f4f4f4"
            }
            Text {

                anchors.right: parent.left; anchors.rightMargin: 4
                anchors.verticalCenter: line.verticalCenter
                text: modelData
                font.pixelSize: 10
            }
        }
        Canvas {
            x: 32
            width: parent.width - x
            height: parent.height
            opacity: 0.75

            property int h: 24*5

            onPaint: {
                var ctx = getContext("2d")
                ctx.lineWidth = 2
                ctx.strokeStyle = "#54aad4"
                ctx.fillStyle = "#99cce5"
                ctx.beginPath()
                    ctx.moveTo(0, h-0)
                    ctx.lineTo(0, h-5)
                    ctx.lineTo(20, h-10)
                    ctx.lineTo(40, h-12)
                    ctx.lineTo(150, h-12)
                    ctx.lineTo(190, h-20)
                    ctx.lineTo(250, h-30)
                    ctx.lineTo(260, h-30)
                    ctx.lineTo(470, h-24*3)
                    ctx.lineTo(470, h-0)
                    ctx.lineTo(0, h-0)
                    ctx.fill()
                    ctx.stroke()
                ctx.closePath()
            }
        }
    }
    ListView {
        orientation: ListView.Horizontal
        x: 32
        y: 24*5 + 64
        width: parent.width - x; height: 16

        model: ListModel {
            ListElement { desc: '10:45'; pos: 20 }
            ListElement { desc: '11:00'; pos: 150 }
            ListElement { desc: '11:15'; pos: 150 }
            ListElement { desc: '11:30'; pos: 150 }
        }
        delegate: Item {
            width: pos; height: parent.height

            LineV {
                anchors.right: parent.right
                height: 6
                color: "grey"

                Text {
                    anchors.top: parent.bottom; anchors.bottomMargin: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: desc
                    font.pixelSize: 10
                }
            }
        }
    }
}
