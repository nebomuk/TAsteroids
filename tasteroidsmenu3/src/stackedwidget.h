#ifndef STACKEDWIDGET_H
#define STACKEDWIDGET_H

#include <QStackedWidget>

namespace Ui {
	class StackedWidget;
}

class HighScoreView;

class StackedWidget : public QStackedWidget {
	Q_OBJECT
public:
	StackedWidget(QWidget *parent = 0);
	~StackedWidget();
	HighScoreView * highScoreView() const;

public slots:
	void on_btnMainMenu_clicked();
	void on_btnMainMenu1_clicked();
	void on_btnMainMenu2_clicked();
	void on_btnHighScore_clicked();
	void on_btnExit_clicked();
	void on_btnNewGame_clicked();
	void on_btnCredits_clicked();
	void on_btn1Player_clicked();
	void on_btn2Players_clicked();

signals:
	void signalStartGame(int);

protected:
	void changeEvent(QEvent *e);

private:
	QString dataLocation;
	Ui::StackedWidget *ui;
};

#endif // STACKEDWIDGET_H
