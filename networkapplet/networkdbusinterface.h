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

    void someValueRequired();    
    void setSomeValue(const QString &value);

private slots:
    void valueSet();
    void DBusMessagingFailure();    

signals:
    void someValueReceived(QString);

private:
    QDBusInterface *dbusIf;

};

#endif // NETWORKDBUSINTERFACE_H
