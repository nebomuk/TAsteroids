#include "stackedwidget.h"
#include "ui_stackedwidget.h"
#include <QDir>
#include <QDesktopServices>

StackedWidget::StackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::StackedWidget)
{
	ui->setupUi(this);
	ui->highScoreView->setFrameShape(QFrame::NoFrame);
	// dataLocation is the location for this application's data
#ifdef Q_OS_LINUX
	dataLocation = QString(QDir::homePath() + "/.tasteroids/");
#else
	dataLocation = QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tasteroids/";
#endif
	if(QFile::exists(dataLocation + "highscore.dat"))
		ui->highScoreView->readFromFile(dataLocation + "highscore.dat"); // load High Score
}

StackedWidget::~StackedWidget()
{
	ui->highScoreView->writeToFile(dataLocation + "highscore.dat"); // save High Score
	delete ui;
}

HighScoreView * StackedWidget::highScoreView() const
{
	return ui->highScoreView;
}

void StackedWidget::changeEvent(QEvent *e)
{
	QStackedWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void StackedWidget::on_btnMainMenu_clicked()
{
	this->setCurrentIndex(0);
}

void StackedWidget::on_btnMainMenu1_clicked()
{
	on_btnMainMenu_clicked();
}
void StackedWidget::on_btnMainMenu2_clicked()
{
	on_btnMainMenu_clicked();
}

void StackedWidget::on_btnHighScore_clicked()
{
	this->setCurrentIndex(4);
}

void StackedWidget::on_btnExit_clicked()
{
	qApp->quit();
}

void StackedWidget::on_btnNewGame_clicked()
{
	this->setCurrentIndex(1);
}

void StackedWidget::on_btnCredits_clicked()
{
	this->setCurrentIndex(2);
}

void StackedWidget::on_btn1Player_clicked()
{
	emit signalStartGame(1);
}

void StackedWidget::on_btn2Players_clicked()
{
	emit signalStartGame(2);
}
