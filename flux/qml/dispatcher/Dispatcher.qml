import QtQuick 2.0

Item {
    property var stores: []

    function regist(store) {
        stores.push(store)
    }

    function dispatch(action) {
        for (var i=0; i<stores.length; i++) {
            stores[i].subscribe(action)
        }
    }
}
