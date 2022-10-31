import QtQuick 2.5
import my.highscoremodel.singleton 1.0
import "Style.js" as Style

ListView
{

    property color textColor : "#0095a6"
    property string fontFamily : "OCR A Extended"

    id : listView

    // set some default height width, because ListView does not have such attributes by default
    width: headerItem.width
    height: parent.height

    model : HighScoreModel

    headerPositioning : ListView.InlineHeader

    clip: true

    header : Row
    {

    spacing: Style.dp(30)

    Text {

        color: textColor
        font.pointSize: 20
        font.family: fontFamily
        text: qsTr("place");
        horizontalAlignment: Text.AlignHCenter
    }
    Text {

        color: textColor
        font.pointSize: 20
        font.family: fontFamily
        text: qsTr("player");
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        color: textColor
        font.family: fontFamily
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
        color: textColor
        width: headerItem.width/3
        font.pointSize: 20
        font.family: fontFamily
        text: parseInt(place)+1;
        horizontalAlignment:  Text.AlignHCenter
    }
    Text {
        color: textColor
        width: headerItem.width/3
        font.pointSize: 20
        font.family: fontFamily
        text: player;
        horizontalAlignment: Text.AlignLeft
    }
    Text {
        color: textColor
        width: headerItem.width/3
        font.pointSize: 20
        font.family: fontFamily
        text: score;
        horizontalAlignment:  Text.AlignHCenter
    }
}
}
