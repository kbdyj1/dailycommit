.import "common_button_impl.js" as Comm

var clickCount = 0

function onClicked(button) {
    clickCount += 1

    if (clickCount % 5 == 0) {
        button.color = Qt.rgba(1, 0, 0, 1)
    } else {
        button.color = Qt.rgba(0, 1, 0, 0)
    }
}

function logButtonProperties(button) {
    Comm.logButton(button)
}
