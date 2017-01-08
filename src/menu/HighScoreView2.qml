import QtQuick 2.5
import my.highscoremodel.singleton 1.0
import "Style.js" as Style

ListView
{
    id : listView

    // set some default height width, because ListView does not have such attributes by default
    width: headerItem.width
    height: parent.height

    model : HighScoreModel

    headerPositioning : ListView.InlineHeader

    clip: true

    header :
        Row
    {

    spacing: Style.dp(30)

    Text {

        color: "#01375D"
        font.pointSize: 20
        font.family: "OCR A Extended"
        text: qsTr("place");
        horizontalAlignment: Text.AlignHCenter
    }
    Text {

        color: "#01375D"
        font.pointSize: 20
        font.family: "OCR A Extended"
        text: qsTr("player");
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        color: "#01375D"
        font.family: "OCR A Extended"
        font.pointSize: 20
        text: qsTr("score");
        horizontalAlignment: Text.AlignHCenter
    }
}

delegate:

Row
{
    spacing: Style.dp(5)

    Text {
        color: "#01375D"
        width: headerItem.width/3
        font.pointSize: 20
        font.family: "OCR A Extended"
        text: parseInt(place)+1;
        horizontalAlignment:  Text.AlignHCenter
    }
    Text {
        color: "#01375D"
        width: headerItem.width/3
        font.pointSize: 20
        font.family: "OCR A Extended"
        text: player;
        horizontalAlignment: Text.AlignLeft
    }
    Text {
        color: "#01375D"
        width: headerItem.width/3
        font.pointSize: 20
        font.family: "OCR A Extended"
        text: score;
        horizontalAlignment:  Text.AlignHCenter
    }
}
}
