import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Rectangle {
    width: 640; height: 480; color: 'lightsteelblue'

    Text {

        text: '?'
        anchors.centerIn: parent

        function findGreetings() {
            var db = LocalStorage.openDatabaseSync('localdb', '1.0', 'local db sample', 1000000)
            db.transaction(
                function(tx){
                    tx.executeSql('CREATE TABLE IF NOT EXISTS Greeting(salutation TEXT, salutee TEXT)')

                    tx.executeSql('INSERT INTO Greeting VALUES(?, ?)', ['hello', 'Qt6'])

                    var rs = tx.executeSql('SELECT * FROM Greeting')
                    var r = ''
                    for (var i=0; i<rs.rows.length; i++) {
                        r += rs.rows.item(i).salutation + ', ' + rs.rows.item(i).salutee + '\n'
                    }

                    text = r
                }
            )
        }
        Component.onCompleted: findGreetings()
    }
}
