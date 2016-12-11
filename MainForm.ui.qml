import QtQuick 2.5
import QtQuick.Controls 1.4
//import my.highscoremodel.singleton 1.0

Rectangle {
    color: "transparent"

    width: 1020
    height: 567
    anchors.fill: parent
    id : rootRectangle
    property alias mainMenu : mainMenu


        BorderImage {
            id: borderImage1
            x: 0
            y: 0

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
            anchors.right: parent.right
            anchors.rightMargin: 512
            anchors.left: parent.left
            anchors.leftMargin: 63
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 150
            anchors.top: parent.top
            anchors.topMargin: 125

            MainMenu
            {
                id : mainMenu
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0

            }

        }

        AsteroidModelView {
            x: 638
            width: 315
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 110
            anchors.top: parent.top
            anchors.topMargin: 144
            anchors.right: parent.right
            anchors.rightMargin: 67
        }

}
