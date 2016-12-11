import QtQuick 2.0
import QtQuick.Controls 1.4

StackView
{
    id : stackView
    z: 1
    anchors.fill: parent


    // empty transitions
    delegate: StackViewDelegate {
    }

    property Component about :
        Item
        {

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top : parent.top


    Text
    {
        color : aboutRect.border.color
        text : "TAsteroids 0.7 \n Copyright © 2016 \n  Taiko000"
        font.pointSize: 32
        horizontalAlignment: Text.AlignHCenter

    }

    MainMenuRectangle
    {
        title1 : backToMainMenuStr

        height: Qt.platform.os == "android" ? 0 : height
        visible:  Qt.platform.os != "android"

        anchors.bottom: parent.bottom

        id : aboutRect

        MouseArea
        {

            anchors.fill: parent
            onPressed: {
                stackView.pop();
            }


        }
    }

    }

property Component newGame : Item
{

}

property Component highScores : Item
{
    Item
    {

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top : parent.top



    HighScoreView
    {
            anchors.top : parent.top
            anchors.bottom: highScoresBack.top
    }

        MainMenuRectangle
        {
            id : highScoresBack

            title1 : backToMainMenuStr
            anchors.bottom: parent.bottom

            height: Qt.platform.os == "android" ? 0 : height
            visible:  Qt.platform.os != "android"

            MouseArea
            {

                anchors.fill: parent
                onPressed: {
                    stackView.pop();
                }


            }
        }

    }
}

property var newGameStr: qsTr("New Game")
property var aboutStr : qsTr("About")
property var highScoreStr : qsTr("High Scores")
property var exitStr : qsTr("Exit")
property var backToMainMenuStr : qsTr("Main Menu")


property var componentMap: {
    newGameStr : newGame
    aboutStr : about

}


property Component listDelegate1 :

    MainMenuRectangle
    {
        id : rectangle2
        height: stackView.height / 4 - menuLevel0ListView.spacing

    MouseArea
    {

        anchors.fill: parent
        onPressed: {
            if(name === exitStr)
            {
                Qt.quit()
            }
            if (stackView.depth === 1) {
                if(name === aboutStr)
                {
                    stackView.push({item: about, properties:{title1:"Main Menu"}});

                }
                else if(name === highScoreStr)
                {
                    stackView.push({item : highScores});
                }

            }

        }


    }

    }


initialItem : ListView {



    id : menuLevel0ListView

    spacing : 16
    model: ListModel {
        id: listModel1
        ListElement {
            name: qsTr("New Game")
        }
        ListElement {
            name: qsTr("High Scores")
        }
        ListElement {
            name: qsTr("About")
        }
        ListElement {
            name: qsTr("Exit")

        }
    }

    delegate: listDelegate1



    }

}
