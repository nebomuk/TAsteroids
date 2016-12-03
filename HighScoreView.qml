import QtQuick 2.0
import my.highscoremodel.singleton 1.0

ListView {
    id: highScoreListView
    width: 207
    height: 155
    z: 1
    delegate:
        Row {
            id: row1               

            Text {
                text: player
                font.pointSize: 20
                color: "#01375D"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: score
                font.pointSize: 20
                color: "#01375D"
                font.bold: true
                anchors.verticalCenter: parent.verticalCenter
            }
            spacing: 10
        }


    model : HighScoreModel


}
