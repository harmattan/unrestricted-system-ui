#include "profiledbusinterface.h"
#include <QtDBus>
#include <QDebug>

ProfileDBusInterface::ProfileDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/",
                                "com.nokia.systemui.profile",
                                QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(currentProfile(int)), this, SIGNAL(currentProfile(int)));
    connect(dbusIf, SIGNAL(volumeLevel(int, int)), this, SIGNAL(volumeLevel(int, int)));
    connect(dbusIf, SIGNAL(vibrationValue(int, bool)), this, SIGNAL(vibrationValue(int, bool)));
}

ProfileDBusInterface::~ProfileDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

QString ProfileDBusInterface::getCurrentProfileName()
{
    qDebug() << Q_FUNC_INFO;
    QString name = "";
    QDBusMessage reply = dbusIf->call(QString("getCurrentProfileName"));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << Q_FUNC_INFO << "error reply:" << reply.errorName();
    } else if (reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        name = reply.arguments().at(0).toString();
    } else {
        qDebug() << Q_FUNC_INFO << "reply type:" << reply.type();
    }
    qDebug() << Q_FUNC_INFO << ":" << name;
    return name;
}

void ProfileDBusInterface::currentProfileRequired()
{
    qDebug() << "ProfileDBusInterface::currentProfileRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getCurrentProfile"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void ProfileDBusInterface::volumeLevelsRequired()
{
    qDebug() << "ProfileDBusInterface::volumeLevelsRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getVolumeLevels"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void ProfileDBusInterface::vibrationValuesRequired()
{
    qDebug() << "ProfileDBusInterface::vibrationValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("getVibrationValues"), list, this, SIGNAL(querySent()), SLOT(DBusMessagingFailure()));
}

void ProfileDBusInterface::setProfile(int profileId)
{
    qDebug() << "ProfileDBusInterface::setProfile(" << profileId << ")";
    QList<QVariant> list;
    list << QVariant(profileId);
    dbusIf->callWithCallback(QString("setProfile"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}


void ProfileDBusInterface::setVibration(int profileId, bool enabled)
{
    qDebug() << "ProfileDBusInterface::setVibration(" << profileId << ", " << enabled << ")";
    QList<QVariant> list;
    list << QVariant(profileId);
    list << QVariant(enabled);
    dbusIf->callWithCallback(QString("setVibration"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void ProfileDBusInterface::setVolumeLevel(int profileId, int levelIndex)
{
    qDebug() << "ProfileDBusInterface::setVolumeLevel(" << profileId << ", " << levelIndex << ")";
    QList<QVariant> list;
    list << QVariant(profileId);
    list << QVariant(levelIndex);
    dbusIf->callWithCallback(QString("setVolumeLevel"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void ProfileDBusInterface::valueSet()
{
    qDebug() << "ProfileDBusInterface::valueSet()";
}

void ProfileDBusInterface::querySent()
{
    qDebug() << "ProfileDBusInterface::querySent()";
}

void ProfileDBusInterface::DBusMessagingFailure()
{
    qDebug() << "ProfileDBusInterface::DBusMessagingFailure()";
}

