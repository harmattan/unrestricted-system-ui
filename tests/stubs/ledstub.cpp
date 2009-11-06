#include "ledstub.h"
#include <QDebug>

namespace Maemo {

QmLED::QmLED(QObject *parent) : QObject(parent)
{
}

QmLED::~QmLED()
{
}

bool QmLED::activate(const QString &pattern)
{
    qDebug() << pattern;
    return true;
}

bool QmLED::deactivate(const QString &pattern)
{
    qDebug() << pattern;
    return false;
}

}
