import QtQuick 2.5
import QtQuick.Controls 1.4
//import my.highscoremodel.singleton 1.0

Rectangle {
    color: "transparent"

    // width height only set for correct display in design mode, the main form is stretched to the window size
    width: 1020
    height: 567
    anchors.fill: parent
    id : rootRectangle
    property alias mainMenu : mainMenu


        BorderImage {
            id: borderImage1
            x: 0
            y: 0
            cache : false
            z: -1
            asynchronous: true
            antialiasing: false
            anchors.fill: parent
            verticalTileMode: BorderImage.Stretch
            horizontalTileMode: BorderImage.Stretch
            border.bottom: 190
            border.top: 145
            border.right: 560
            border.left: 262
            source: "qrc:/menu_full.png"
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
            id: asteroidModelView
            x: 614
            width: 366
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 67
            anchors.top: parent.top
            anchors.topMargin: 115

        }

}
