#ifndef NETWORKDBUSINTERFACE_H
#define NETWORKDBUSINTERFACE_H

#include <QObject>
#include <QStringList>


class QDBusInterface;

class NetworkDBusInterface : public QObject
{
    Q_OBJECT

public:
    NetworkDBusInterface();
    virtual ~NetworkDBusInterface();

    void showNotification(const QString &text);

private:
    QDBusInterface *dbusIf;

};

#endif // NETWORKDBUSINTERFACE_H
