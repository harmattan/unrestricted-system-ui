#ifndef CANCELLABLENOTIFICATION_H
#define CANCELLABLENOTIFICATION_H

#include <QObject>
#include <QString>
#include <QHash>

class DuiNotification;

class CancellableNotification : public QObject
{
    Q_OBJECT

public:
    CancellableNotification(DuiNotification *n,
                            const QString &notifTextPattern,
                            const int &appearTime,
                            const QString &appearTimeVariable,
                            const QHash<QString,QString> &staticVariables);
    virtual ~CancellableNotification();

signals:
    void notifTimeout();

private slots:
    void updateText();

private: //attributes
    DuiNotification *n;
    QString notifTextPattern;
    int appearTime;
    QString appearTimeVariable;
    QHash<QString,QString> staticVariables;

};

#endif // CANCELLABLENOTIFICATION_H
