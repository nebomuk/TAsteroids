import QtQuick 2.0
import my.highscoremodel.singleton 1.0

ListView {
    id: highScoreListView
    width: 207
    height: 155
    z: 1
    delegate: Item {
        x: 5
        width: 80
        height: 40
        Row {
            id: row1
//                    Rectangle {
//                        width: 40
//                        height: 40
//                        color: colorCode
//                    }

            Text {
                text: player
                color: "white"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: score
                color: "white"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
            spacing: 10
        }
    }

    model : HighScoreModel


}
