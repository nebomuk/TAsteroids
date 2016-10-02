#ifndef HIGHSCOREVIEW_H
#define HIGHSCOREVIEW_H

#include <QTableView>
#include <QStandardItemModel>

/*
This view shows a table with name / points  pairs sorted by the points
the name /points pairs are called highScores in this class
By default, editing is disabled on the view.
*/


class HighScoreView : public QTableView
{
	//Q_PROPERTY(int maxNameLength READ maxNameLength WRITE setMaxNameLength)

Q_OBJECT
public:
	HighScoreView(QWidget *parent = 0);

	//write the highScores returned by highScores() to a file,
	// if the file exists, it will be overridden
	void writeToFile(const QString& filename);

	//read the highScores returned by highScores() from a file
	void readFromFile(const QString& filename);

	QStandardItemModel * model() const{return model_;}

	// this is a convenience function that enables text editing on the row that has been least inserted
	void editLastInsertedName();

	// append highscores to the model, sort them by score and show them on the view
	void appendHighScore(const QString& player, int score);
	void appendHighScores(const QMap<QString, int>& highscores);

	// read the highScores from the model and return them
	QMap<QString, int> highScores() const;

	// longer player names will be truncated. Default is 12
	//void setMaxNameLength(int maxNameLength){ maxNameLength_ = maxNameLength; }
	//int maxNameLength() const { return maxNameLength_; }

private:
	QStandardItemModel * model_;
	QStandardItem * lastInsertedNameItem_;
	//unsigned int maxNameLength_;
};

#endif // HIGHSCOREVIEW_H
