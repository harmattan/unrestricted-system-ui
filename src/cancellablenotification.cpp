#include "cancellablenotification.h"
#include <DuiNotification>
#include <QTimer>

CancellableNotification::CancellableNotification(
        DuiNotification *n,
        const QString &notifTextPattern,
        const int &appearTime,
        const QString &appearTimeVariable,
        const QHash<QString,QString> &staticVariables) : QObject()
{
    this->n = n;
    this->notifTextPattern = notifTextPattern;
    this->appearTime = appearTime;
    this->appearTimeVariable = appearTimeVariable;

    QHash<QString,QString>::const_iterator i;
    for (i = staticVariables.begin(); i != staticVariables.end(); ++i)
        this->notifTextPattern.replace(i.key(),i.value());

    QTimer *t = new QTimer();
    connect(t, SIGNAL(timeout()), this, SLOT(updateText()));
    t->setSingleShot(false);
    t->start(1000);
}

CancellableNotification::~CancellableNotification()
{
}

void CancellableNotification::updateText()
{
    if(appearTime > 0) {
        QString number;
        number.setNum(appearTime);
        appearTime--;
        QString notifText = notifTextPattern;
        notifText.replace(appearTimeVariable, number);
        n->setBody(notifText);
    }
    else {
        n->disappear();
        emit notifTimeout();
    }
}
