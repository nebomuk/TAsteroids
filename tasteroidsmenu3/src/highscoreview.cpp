#include "highscoreview.h"
#include <QFile>
#include <QHeaderView>

HighScoreView::HighScoreView(QWidget *parent) :
	QTableView(parent)
{
	lastInsertedNameItem_ = NULL;
	model_ = new QStandardItemModel(0,2,this);
	model_->setHeaderData(0,Qt::Horizontal,tr("Player"));
	model_->setHeaderData(1,Qt::Horizontal,tr("Score"));
	setEditTriggers(QAbstractItemView::NoEditTriggers); // disable editing
	setModel(model_);
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setDefaultAlignment(Qt::AlignRight);
}

void HighScoreView::appendHighScore(const QString& player, int score)
{
	model_->insertRow(0);
	model_->setData(model_->index(0,0),player);
	model_->setData(model_->index(0,1),score);
	lastInsertedNameItem_ = model_->item(0,0);
	QStandardItem * item = model_->item(0,1);
	item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);// align all numbers to right
	sortByColumn(1,Qt::DescendingOrder); // higher numbers on top
	resizeColumnsToContents(); // expand to the available space
	horizontalHeader()->setStretchLastSection(true);
}

void HighScoreView::appendHighScores(const QMap<QString, int>& highscores)
{
	QMap<QString, int>::const_iterator i = highscores.constBegin();
	for(;i != highscores.constEnd(); ++i)
	{
		appendHighScore(i.key(),i.value());
	}
}

void HighScoreView::writeToFile(const QString& filename)
{
	QFile file(filename);
	if(!file.open(QIODevice::WriteOnly))
	{
		qDebug("HighScoreView::writeToFile failed");
		return;
	}
	QDataStream out(&file);
	out.setVersion(QDataStream::Qt_4_4);
	out << highScores();
	file.close();
}

void HighScoreView::readFromFile(const QString& filename)
{
	QFile file(filename);
	if(!file.open(QIODevice::ReadOnly))
	{
		qDebug("HighScoreView::readFromFile failed");
		return;
	}
	QMap<QString, int> map;
	QDataStream in(&file);
	in.setVersion(QDataStream::Qt_4_4);
	in >> map;
	file.close();
	appendHighScores(map);
}

QMap<QString, int> HighScoreView::highScores() const
{
	QMap<QString, int> map;
	for(int i = 0; i< model_->rowCount(); ++i)
	{
		QString player = model_->data(model_->index(i,0)).toString();
		int score = model_->data(model_->index(i,1)).toInt();
		map.insertMulti(player, score);
	}
	return map;
}

void HighScoreView::editLastInsertedName()
{
	scrollTo(lastInsertedNameItem_->index());
	edit(lastInsertedNameItem_->index()); // let the user enter his name
}
