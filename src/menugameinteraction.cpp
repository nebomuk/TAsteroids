#include "menugameinteraction.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>
#include <QScreen>

MenuGameInteraction::MenuGameInteraction(QObject *parent) : QObject(parent)
{

    QUrl highScoreResourceUrl(QStringLiteral("qrc:/src/menu/HighScoreModelSingleton.qml"));
    qmlRegisterSingletonType(highScoreResourceUrl, "my.highscoremodel.singleton", 1, 0, "HighScoreModel");
    gameGraphicsView_ = Q_NULLPTR;
    engine_ = Q_NULLPTR;
    playerCount_ = 1;
    highScore_ = 0;
}

MenuGameInteraction::~MenuGameInteraction()
{
    delete gameGraphicsView_;
    delete engine_;
}

void MenuGameInteraction::showMainMenu()
{
    if(!engine_)
    {
        if(gameGraphicsView_)
        {
            gameGraphicsView_->deleteLater();
            gameGraphicsView_ = Q_NULLPTR;
        }

        engine_ = new QQmlApplicationEngine();
//        disconnect(engine_, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
//        disconnect(engine_, &QQmlApplicationEngine::exit,QCoreApplication::instance(),nullptr);



        qreal density = QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio();
        engine_->rootContext()
                ->setContextProperty("screenPixelDensity",density);
        engine_->rootContext()->setContextProperty("menuGameInteraction",this);
        engine_->load(QUrl(QStringLiteral("qrc:/src/menu/mainTouch.qml")));
        QGuiApplication::setQuitOnLastWindowClosed(true);

    }
}

void MenuGameInteraction::showGame()
{
    if(!gameGraphicsView_)
    {
        if(engine_)
        {
            QGuiApplication::setQuitOnLastWindowClosed(false);
            engine_->deleteLater();
            engine_ = Q_NULLPTR;
        }

        highScore_ = 0;
        gameGraphicsView_ = new GraphicsView();
        gameGraphicsView_->setPlayerCount(playerCount_);
        gameGraphicsView_->restart();
        gameGraphicsView_->show();
#ifdef Q_OS_ANDROID
        connect(static_cast<QGuiApplication*>(QGuiApplication::instance()),&QGuiApplication::applicationStateChanged,gameGraphicsView_,&GraphicsView::onApplicationStateChanged);
#endif

        connect(gameGraphicsView_,&GraphicsView::signalHighScore,this,&MenuGameInteraction::slotHighScore);
        connect(gameGraphicsView_,&GraphicsView::signalClose,this,&MenuGameInteraction::showMainMenu);
    }

}

void MenuGameInteraction::slotHighScore(int score)
{
    highScore_ =  score;
}
