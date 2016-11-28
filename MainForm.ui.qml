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

        Rectangle {
            id: rectangle1
            x: 96
            y: 125
            width: 430
            height: 292
            color: "#00000000"

            ListView {
                id : menuLevel0ListView
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                       anchors.leftMargin: 0
                       anchors.topMargin: 0
                       z: 1
                       anchors.fill: parent
                       spacing : 10
                       model: ListModel {
                           ListElement {
                               title: qsTr("New Game")
                           }
                           ListElement {
                               title: qsTr("High Scores")
                           }
                           ListElement {
                               title: qsTr("About")
                           }
                           ListElement {
                               title: qsTr("Exit")
                           }
                       }

                       delegate: Item {

                           id: item1
                           x: 0
                           y: 0
                           width: 430
                           height: 62

                           Rectangle {
                               id: rectangle2
                               x: 0
                               y: 0
                               width: 246
                               height: 58
                               color: "#00000000"
                               radius: 0
                               border.color: "#0095a6"
                               border.width: 4

                               Text {
                                   text: title
                                   color: "white"
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.horizontalCenter:  parent.horizontalCenter
                               }
                           }


                       }

            }

        }



}
