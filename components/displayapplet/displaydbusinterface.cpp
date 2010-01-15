#include "displaydbusinterface.h"
#include <QtDBus>
#include <QDebug>

DisplayDBusInterface::DisplayDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/",
                                "com.nokia.systemui.display",
                                QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(brightnessValuesReceived(int, QStringList)), this, SIGNAL(brightnessValuesReceived(int, QStringList)));
    connect(dbusIf, SIGNAL(screenLightsValuesReceived(int, QStringList)), this, SIGNAL(screenLightsValuesReceived(int, QStringList)));
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
    dbusIf->callWithCallback(QString("brightnessValues"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::screenLightsValuesRequired()
{
    qDebug() << "DisplayDBusInterface::screenLightsValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("screenLightsValues"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::blankInhibitValueRequired()
{
    qDebug() << "DisplayDBusInterface::blankInhibitValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("blankInhibitValue"), list, this, SIGNAL(blankInhibitValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::setBrightnessValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface::setBrightnessValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBrightnessValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::setScreenLightsValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface::setScreenLightsValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setScreenLightsValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::setBlankInhibitValue(bool value)
{
    qDebug() << "DisplayDBusInterface::setBlankInhibitValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBlankInhibitValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::valueSet()
{
    qDebug() << "DisplayDBusInterface::valueSet()";
}

void DisplayDBusInterface::DBusMessagingFailure()
{
    qDebug() << "DisplayDBusInterface::DBusMessagingFailure()";
}
