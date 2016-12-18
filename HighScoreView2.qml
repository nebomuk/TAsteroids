import QtQuick 2.5
import my.highscoremodel.singleton 1.0
import "Style.js" as Style

ListView
{
    id : listView
    width: headerItem.width

    model : HighScoreModel

    headerPositioning : ListView.PullBackHeader

    header :
        Row
    {

    spacing: Style.dp(20)

    Text {

        font.pointSize: 20
        text: "place";
        horizontalAlignment: Text.AlignHCenter
    }
    Text {

        font.pointSize: 20
        text: "player";
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        font.pointSize: 20
        text: "score";
        horizontalAlignment: Text.AlignHCenter
    }
}

delegate:

Row
{
    spacing: 0

    Text {
        width: headerItem.width/3
        font.pointSize: 20
        text: place;
        horizontalAlignment:  Text.AlignHCenter
    }
    Text {
        width: headerItem.width/3
        font.pointSize: 20
        text: player;
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        width: headerItem.width/3
        font.pointSize: 20
        text: score;
        horizontalAlignment:  Text.AlignHCenter
    }
}
}
