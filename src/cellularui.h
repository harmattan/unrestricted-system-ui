#ifndef CELLULARUI_H
#define CELLULARUI_H

#include "notificationtype.h"

#include <QObject>

class QDBusInterface;

class CellularUI : public QObject
{
    Q_OBJECT

public:
    CellularUI(QObject* parent = 0);

Q_SIGNALS:
    void showNotification(QString, NotificationType::Type type = NotificationType::info);

private Q_SLOTS:
    void gprsSuspended(int, QString);

private:
    QDBusInterface* gprs;
};

#endif // CELLULARUI_H
