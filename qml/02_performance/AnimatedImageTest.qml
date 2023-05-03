import QtQuick 2.0

Item {
    anchors.fill: parent

    AnimatedImage {
        id: gif

        anchors.centerIn: parent
        source: "qrc:/sgshin.gif"
        cache: false

        // cached: VIRT(934812), RES(158216)
        //!cached: VIRT(860012), RES(119208)
    }
    Text {
        anchors.left: parent.left; anchors.leftMargin: 4
        anchors.top: parent.top; anchors.topMargin: 4
        text: "source: " + gif.source +
              "\nsize: " + gif.width + "x" + gif.height +
              "\nframe count: " + gif.currentFrame + "/" + gif.frameCount
    }
}
