pragma Singleton

import QtQuick 2.0
import QtQuick.LocalStorage 2.0


ListModel {
    id : highScoreModel
    property int topScore: 0
    property string topPlayer: ""
    property int maxScores: 30

    function __db()
    {
        return LocalStorage.openDatabaseSync("HighScoreModel", "1.0", "Generic High Score Functionality for QML", 1000000);
    }
    function __ensureTables(tx)
    {
        tx.executeSql('CREATE TABLE IF NOT EXISTS HighScores(score INT, player TEXT)', []);
    }

    function fillModel() {
        __db().transaction(
            function(tx) {
                __ensureTables(tx);
                var rs = tx.executeSql("SELECT score,player FROM HighScores ORDER BY score DESC");
                highScoreModel.clear();
                if (rs.rows.length > 0) {
                    topScore = rs.rows.item(0).score
                    topPlayer = rs.rows.item(0).player
                    for (var i=0; i<rs.rows.length; ++i) {
                        if (i < maxScores)
                        {
                            var rowItem = rs.rows.item(i);
                            rowItem["place"] = i
                            highScoreModel.append(rowItem)
                        }
                    }
                    if (rs.rows.length > maxScores)
                        tx.executeSql("DELETE FROM HighScores WHERE score <= ?",
                                            [rs.rows.item(maxScores).score]);
                }
            }
        )
    }

    function savePlayerScore(player,score) {
        __db().transaction(
            function(tx) {
                __ensureTables(tx);
                tx.executeSql("INSERT INTO HighScores VALUES(?,?)", [score,player]);
                fillModel();
            }
        )
    }

    function saveScore(score) {
        savePlayerScore("player",score);
    }

    function clearScores() {
        __db().transaction(
            function(tx) {
                tx.executeSql("DELETE FROM HighScores WHERE player=?");
                fillModel();
            }
        )
    }

    Component.onCompleted: {
        fillModel()

    }
}
