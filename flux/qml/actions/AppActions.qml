import QtQuick 2.0

Item {
    property var dispatcher

    function createAction(type, param) {
        dispatcher.dispatch({type:type, param:param})
    }
}
