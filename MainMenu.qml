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
    Column
    {

    anchors.fill: parent

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
    HighScoreView
    {

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

property Component listDelegate1 : Item {

    id: item1
    width: parent.width
    height: 62

    MainMenuRectangle
    {
        id : rectangle2
    }



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

    anchors.fill: parent
    spacing : 10
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
