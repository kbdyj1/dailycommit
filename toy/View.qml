import QtQuick 2.0

Item {
    id: root

    property var view

    function createAction(id, param) {
        if (view !== undefined && view !== null) {
            view.subscribe(id, param)
        }
    }
}
