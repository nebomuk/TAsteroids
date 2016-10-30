#include <QApplication>
#include <QAction>
#include <QDir>
#include <QBitmap>
#include <QTimer>
#include <QInputDialog>
#include "shapedmenu.h"
#include "graphicsview.h"
#include "view.h"
#include "stackedwidget.h"
#include "highscoreview.h"

ShapedMenu::ShapedMenu(QWidget *parent)
	: QWidget(parent, Qt::FramelessWindowHint),
	background_(":menu.png")
{
        gameView_ = Q_NULLPTR;
	playerCount_ = 1;

	QAction *quitAction = new QAction(tr("E&xit"), this);
	quitAction->setShortcut(tr("Ctrl+Q"));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	addAction(quitAction);

	setFixedSize(background_.size());

	stackedWidget_ = new StackedWidget(this);
	stackedWidget_->move(40,108);
	qApp->setPalette(stackedWidget_->palette());

	asteroidView_ = new View(this);
	asteroidView_->setFrameShape(QFrame::NoFrame);
	asteroidView_->setGeometry(614,116,366,386);

	asteroidView_->loadRandomModel();

	connect(stackedWidget_,SIGNAL(signalStartGame(int)),this,SLOT(prepareStart(int)));
	connect(stackedWidget_,SIGNAL(currentChanged(int)),asteroidView_,SLOT(loadRandomModel(int)));

	setContextMenuPolicy(Qt::ActionsContextMenu);
	setWindowTitle(tr("Menu"));
}

void  ShapedMenu::startGame()
{

//	if(!gameView_)
//	{
//		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//		gameView_ = new GraphicsView; // processes lots of images
//		// must connect only once!
//		connect(gameView_,SIGNAL(signalClose()),this,SLOT(showHighscore()),Qt::QueuedConnection);
//		QApplication::restoreOverrideCursor();
//	}
//        //gameView_->setPlayerCount(playerCount_);
//	gameView_->restart();

//	gameView_->show();
//	this->hide();
}

// todo: remove this and use QueuedConnection
void  ShapedMenu::prepareStart(int playerCount /*= 1*/)
{
	playerCount_ = playerCount;
	stackedWidget_->setCurrentIndex(3); // show loading screen
	QTimer::singleShot(0,this,SLOT(startGame())); // process events
}

void ShapedMenu::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void ShapedMenu::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton) {
		move(event->globalPos() - dragPosition);
		event->accept();
	}
}

void ShapedMenu::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawPixmap(0,0,background_);
}

void ShapedMenu::resizeEvent(QResizeEvent * /* event */)
{
	QBitmap mask = background_.createMaskFromColor(QColor(255,0,255,255),Qt::MaskInColor); // pink
	setMask(mask); // cut out the pink borders ( pink is now the transparent color)
}

void ShapedMenu::showEvent(QShowEvent *)
{
	stackedWidget_->setCurrentIndex(0); // show main menu
}

void ShapedMenu::showHighscore()
{
	show();
	stackedWidget_->setCurrentIndex(4); // show highScoreTable

	if(!gameView_)
	return;

	int highScore;
        //highScore = gameView_->lastHighScore();

	stackedWidget_->highScoreView()->appendHighScore(tr("Please enter your name"),highScore);
	stackedWidget_->highScoreView()->editLastInsertedName();
}

QSize ShapedMenu::sizeHint() const
{
	return background_.size();
}

