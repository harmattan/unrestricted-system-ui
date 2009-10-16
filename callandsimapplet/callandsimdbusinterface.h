#ifndef CALLANDSIMDBUSINTERFACE_H
#define CALLANDSIMDBUSINTERFACE_H

#include <QObject>

class QDBusInterface;

class CallAndSimDBusInterface : public QObject
{
public:
    CallAndSimDBusInterface();
    virtual ~CallAndSimDBusInterface();

private:
    QDBusInterface *dbusIf;
};

#endif // CALLANDSIMDBUSINTERFACE_H
