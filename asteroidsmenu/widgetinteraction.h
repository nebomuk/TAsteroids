#ifndef WIDGETINTERACTION_H
#define WIDGETINTERACTION_H

#include <QObject>

class WidgetInteraction : public QObject
{
    Q_OBJECT
public:
    explicit WidgetInteraction(QObject *parent = 0);

signals:

public slots:
    void showWidget();
};

#endif // WIDGETINTERACTION_H
