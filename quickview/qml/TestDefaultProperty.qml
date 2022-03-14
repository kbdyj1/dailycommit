import QtQuick 2.0
import Birthday 1.0

Item {
    id: root

    BirthdayParty {
        id: party
        host: Person {
            name: "ymhong"
        }

        Person { name: "guest1" }
        Person { name: "guest2" }
        Person { name: "guest3" }
    }
    Timer {
        running: true
        interval: 100
        onTriggered: {
//            for (var i=0; i<party.guests.length; i++) {
//                console.log("guest[" + i + "] : " + party.guests[i].name)
//            }
            console.log("guest count : " + party.guests.length)
            console.log("guest[0] : " + party.guests[0].name)
            console.log("guest[1] : " + party.guests[1].name)
            console.log("guest[2] : " + party.guests[2].name)
        }
    }
}
