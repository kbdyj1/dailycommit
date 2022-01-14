import QtQuick
import Actor 1.0

Item {
    function read(array, objects) {
        for (var i=0; i<array.length; i++)
            console.log("array[" + i + "] : " + array[i])

        for (var prop in objects)
            console.log("objects[" + prop + "] : " + objects[prop])
    }
    function readDate(dt) {
        console.log("c++ date is : " + dt.toUTCString())

        return new Date()
    }

    function readActor(actor) {
        console.log("actor.name : " + actor.name)
        switch (actor.status) {
        case Actor.Ready: console.log("Ready"); break
        case Actor.Loading: console.log("Loading"); break
        case Actor.Error:
        default:
            console.log("Error"); break
        }
    }
}
