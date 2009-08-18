#ifndef CANCELLABLENOTIFICATION_H
#define CANCELLABLENOTIFICATION_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QTimer>
#include <DuiMessageBox>


class CancellableNotification : public QObject
{
    Q_OBJECT

public:
    explicit CancellableNotification(const QString &notifTextPattern,
                            const int &appearTime,
                            const QString &appearTimeVariable,
                            const QHash<QString,QString> &staticVariables);

    virtual ~CancellableNotification();
    void show();

signals:
    void cancelled();
    void timeout();

public slots:
    void updateText();

private: //attributes
    QTimer *t;
    QString notifTextPattern;
    DuiMessageBox *mb;
    int appearTime;
    QString appearTimeVariable;
    QHash<QString,QString> staticVariables;

};

#endif // CANCELLABLENOTIFICATION_H
