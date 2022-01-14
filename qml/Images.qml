import QtQuick 2.0

Item {
    component LabeledImage: Column {
        property alias source: image.source
        property alias caption: text.text

        Image {
            id: image
        }
        Text {
            id: text
            font.bold: true
            anchors.horizontalCenter: image.horizontalCenter
        }
    }
    Row {
        spacing: 16
        LabeledImage {
            id: before
            source: "qrc:/res/before.png"
            caption: "before"
        }
        LabeledImage {
            id: after
            source: "qrc:/res/after.png"
            caption: "after"
        }
    }
}
