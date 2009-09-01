#include "cancellablenotification.h"

#include <QTimer>
#include <QDebug>
#include <DuiLocale>

CancellableNotification::CancellableNotification(  
        const QString &notifTextPattern,
        const int &appearTime,
        const QString &appearTimeVariable,
        const QHash<QString,QString> &staticVariables)
{    
    //define the textual content of the messagebox
    this->notifTextPattern = notifTextPattern;
    this->appearTime = appearTime;
    this->appearTimeVariable = appearTimeVariable;

    QHash<QString,QString>::const_iterator i;
    for (i = staticVariables.begin(); i != staticVariables.end(); ++i)
        this->notifTextPattern.replace(i.key(),i.value());

    //start the timer
    t = new QTimer();
    connect(t, SIGNAL(timeout()), this, SLOT(updateText()));
    t->setSingleShot(false);

    mb = NULL;

}

CancellableNotification::~CancellableNotification()
{
    delete mb;
    mb = NULL;
    delete t;
    t = NULL;
}

void CancellableNotification::show()
{    
    t->start(1000);
}

void CancellableNotification::updateText()
{
    if(mb != NULL) {
        mb->disappear();
        delete mb;
        mb = NULL;
    }

    if(appearTime > 0) {
        QString number;
        number.setNum(appearTime);
        appearTime--;
        QString notifText = notifTextPattern;
        notifText.replace(appearTimeVariable, number);
        mb = new DuiMessageBox(notifText, DuiMessageBox::Cancel);
        connect(mb, SIGNAL(rejected()), this, SIGNAL(cancelled()));
        mb->appear();
    }
    else
        emit timeout();
}
