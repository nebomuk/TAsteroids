#include "widgetinteraction.h"

#include <QWidget>

WidgetInteraction::WidgetInteraction(QObject *parent) : QObject(parent)
{

}

void WidgetInteraction::showWidget()
{
    auto * w = new QWidget();
    w->show();

}
