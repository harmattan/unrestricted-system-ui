#include "profiledbusinterface.h"
#include <QtDBus>
#include <QDebug>

profileDBusInterface::profileDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
				"com.nokia.systemui.profile", 
				QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(brightnessValuesReceived(int, QStringList)), this, SIGNAL(brightnessValuesReceived(int, QStringList)));
    connect(dbusIf, SIGNAL(screenLightsValuesReceived(int, QStringList)), this, SIGNAL(screenLightsValuesReceived(int, QStringList)));
}

profileDBusInterface::~profileDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void profileDBusInterface::brightnessValuesRequired()
{
    qDebug() << "profileDBusInterface::brightnessValuesRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("brightnessValues"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void profileDBusInterface::screenLightsValuesRequired()
{
    qDebug() << "profileDBusInterface::screenLightsValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("screenLightsValues"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void profileDBusInterface::blankInhibitValueRequired()
{
    qDebug() << "profileDBusInterface::blankInhibitValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("blankInhibitValue"), list, this, SIGNAL(blankInhibitValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void profileDBusInterface::setBrightnessValue(const QString &value)
{
    qDebug() << "profileDBusInterface::setBrightnessValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBrightnessValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void profileDBusInterface::setScreenLightsValue(const QString &value)
{
    qDebug() << "profileDBusInterface::setScreenLightsValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setScreenLightsValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void profileDBusInterface::setBlankInhibitValue(bool value)
{
    qDebug() << "profileDBusInterface::setBlankInhibitValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBlankInhibitValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void profileDBusInterface::valueSet()
{
    qDebug() << "profileDBusInterface::valueSet()";
}

void profileDBusInterface::DBusMessagingFailure()
{
    qDebug() << "profileDBusInterface::DBusMessagingFailure()";
}
