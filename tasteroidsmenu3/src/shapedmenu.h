#ifndef ShapedMenu_H
#define ShapedMenu_H

#include <QStackedWidget>

class View;
class StackedWidget;
class GraphicsView;

class ShapedMenu : public QWidget
{
	Q_OBJECT

public:
	ShapedMenu(QWidget *parent = 0);
	QSize sizeHint() const;
public slots:
	void prepareStart(int playerCount = 1);
	void startGame();
	void showHighscore();

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent *event);
	void showEvent(QShowEvent *);

private:
	QPoint dragPosition;
	QPixmap background_;
	StackedWidget * stackedWidget_;
	View * asteroidView_;
	GraphicsView * gameView_;
	int playerCount_;
};


#endif
