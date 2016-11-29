import QtQuick 2.5
import QtQuick.Controls 1.4
//import my.highscoremodel.singleton 1.0

Rectangle {
    property alias highScoreListView: highScoreListView
    color: "transparent"

    width: 1020
    height: 567
    anchors.fill: parent
    id : rootRectangle




        ListView {
            id: highScoreListView
            x: 714
            y: 166
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

            //model : HighScoreModel


        }



        BorderImage {
            id: borderImage1
            x: 0
            y: 0
            width: 1020
            height: 567
            z: -1
            antialiasing: false
            anchors.fill: parent
            scale: 1
            verticalTileMode: BorderImage.Stretch
            horizontalTileMode: BorderImage.Round
            border.bottom: 190
            border.top: 145
            border.right: 560
            border.left: 262
            source: "qrc:/menu.png"
        }

        Item {
            id: mainMenuArea
            x: 96
            y: 125
            width: 430
            height: 292

            MainMenu
            {

            }

        }

}
