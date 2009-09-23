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

void DisplayDBusInterface::blankTimeoutValuesRequired()
{
    qDebug() << "DisplayDBusInterface::blankTimeoutValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("blankTimeoutValues"), list, this, SIGNAL(blankTimeoutValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::blankTimeoutValueRequired()
{
    qDebug() << "DisplayDBusInterface::blankTimeoutValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("blankTimeoutValue"), list, this, SIGNAL(blankTimeoutValueReceived(QString)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::dimTimeoutValuesRequired()
{
    qDebug() << "DisplayDBusInterface::dimTimeoutValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("dimTimeoutValues"), list, this, SIGNAL(dimTimeoutValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::dimTimeoutValueRequired()
{
    qDebug() << "DisplayDBusInterface::dimTimeoutValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("dimTimeoutValue"), list, this, SIGNAL(dimTimeoutValueReceived(QString)), SLOT(DBusMessagingFailure()));
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

void DisplayDBusInterface::setBlankTimeoutValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface::setBlankTimeoutValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBlankTimeoutValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void DisplayDBusInterface::setDimTimeoutValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface::setDimTimeoutValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setDimTimeoutValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
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
