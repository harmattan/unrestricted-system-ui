#include "displaydbusinterface.h"
#include <QtDBus>
#include <QDebug>

DisplayDBusInterface::DisplayDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
				"com.nokia.systemui.display", 
				QDBusConnection::sessionBus());
}

DisplayDBusInterface::~DisplayDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void DisplayDBusInterface::brightnessValuesRequired()
{
    qDebug() << "DisplayDBusInterface::brightnessValuesRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("brightnessValues"), list, this, SIGNAL(brightnessValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::brightnessValueRequired()
{
    qDebug() << "DisplayDBusInterface::brightnessValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("brightnessValue"), list, this, SIGNAL(brightnessValueReceived(QString)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::screenLightsValuesRequired()
{
    qDebug() << "DisplayDBusInterface::screenLightsValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("screenLightsValues"), list, this, SIGNAL(screenLightsValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::screenLightsValueRequired()
{
    qDebug() << "DisplayDBusInterface::screenLightsValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("screenLightsValue"), list, this, SIGNAL(screenLightsValueReceived(QString)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::screenLightsToggleValueRequired()
{
    qDebug() << "DisplayDBusInterface::screenLightsToggleValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("screenLightsToggleValue"), list, this, SIGNAL(screenLightsToggleValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface:: setBrightnessValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface:: setBrightnessValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBrightnessValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface:: setScreenLightsValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface:: setScreenLightsValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setScreenLightsValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface:: setScreenLightsToggleValue(bool value)
{
    qDebug() << "DisplayDBusInterface:: setScreenLightsToggleValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setScreenLightsToggleValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::valueSet()
{
    qDebug() << "DisplayDBusInterface::valueSet()";
}

void DisplayDBusInterface::DBusMessagingFailure()
{
    qDebug() << "DisplayDBusInterface::DBusMessagingFailure()";
}
