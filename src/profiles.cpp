#include "profiles.h"
#include "profile_dbus.h"

#include <QDBusInterface>
#include <QDBusMessage>
#include <QVariant>
#include <QDebug>

namespace {
    const QString KeyVolumeRange = "ringing.alert.volume";
    const QString KeyVolume = "ringing.alert.volume";
    const QString KeyVibra = "vibrating.alert.enabled";
}

Profiles::Profiles()
{
    managerIf = new QDBusInterface ( PROFILED_SERVICE, PROFILED_PATH, PROFILED_INTERFACE);
}

Profiles::~Profiles()
{
    delete managerIf;
    managerIf = NULL;
}

QString Profiles::activeProfile()
{
    QString name = "";
    QDBusMessage reply = managerIf->call(PROFILED_GET_PROFILE);

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::activeProfile() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        name = reply.arguments().at(0).toString();
        qDebug() << "Profiles::activeProfile() result:" << name;
    }
    else {
        qDebug() << "Profiles::activeProfile() reply type:" << reply.type();
    }
    return name;
}

bool Profiles::setActiveProfile(QString name)
{
    bool success = false;
    QDBusMessage reply = managerIf->call(PROFILED_SET_PROFILE, QVariant(name));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::setActiveProfile() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        success = reply.arguments().at(0).toBool();
        qDebug() << "Profiles::setActiveProfile() result:" << name;
        emit activeProfileChanged(name);
    }
    else {
        qDebug() << "Profiles::setActiveProfile() reply type:" << reply.type();
    }
    return success;
}

QStringList Profiles::getProfiles()
{
    QStringList profileNames;
    QDBusMessage reply = managerIf->call(PROFILED_GET_PROFILES);

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::getProfiles() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage) {
        for(int i=0; i < reply.arguments().count(); ++i){
            profileNames.append(reply.arguments().at(i).toString());
        }
        qDebug() << "Profiles::getProfiles() result count:" << profileNames.count();
    }
    else {
        qDebug() << "Profiles::getProfiles() reply type:" << reply.type();
    }
    return profileNames;
}

QStringList Profiles::getVolumeRange()
{
    QStringList range;
    QDBusMessage reply = managerIf->call(PROFILED_GET_VALUE, QVariant(KeyVolumeRange));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::getVolumeLevel() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 1) {
        QStringList tmp = reply.arguments().at(1).toString().split("-", QString::SkipEmptyParts);
        for(int i=0; i < range.count(); ++i){
            range.append(tmp.at(i).trimmed());
            qDebug() << "Profiles::getVolumeLevel() result.at(" << i << "):" << range.at(i);
        }
        qDebug() << "Profiles::getVolumeLevel() result count:" << tmp.count();
    }
    else {
        qDebug() << "Profiles::getVolumeLevel() reply type:" << reply.type();
    }
    return range;
}

int Profiles::getVolumeLevel(QString profileName)
{
    int volume = 30;
    QDBusMessage reply = managerIf->call(PROFILED_GET_VALUE,
                                         QVariant(profileName),
                                         QVariant(KeyVolume));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::getVolumeLevel() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        volume = reply.arguments().at(0).toString().toInt();
        qDebug() << "Profiles::getVolumeLevel() result count:" << volume;
    }
    else {
        qDebug() << "Profiles::getVolumeLevel() reply type:" << reply.type();
    }
    return volume;
}

bool Profiles::isVibrationEnabled(QString profileName)
{
    bool vibra = true;
    QDBusMessage reply = managerIf->call(PROFILED_GET_VALUE,
                                         QVariant(profileName),
                                         QVariant(KeyVibra));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::isVibrationEnabled() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        vibra = reply.arguments().at(0).toString().toUInt();
        qDebug() << "Profiles::isVibrationEnabled() result count:" << vibra;
    }
    else {
        qDebug() << "Profiles::isVibrationEnabled() reply type:" << reply.type();
    }
    return vibra;
}

bool Profiles::setVolumeLevel(QString profileName, int level)
{
    bool success = false;
    QDBusMessage reply = managerIf->call(PROFILED_SET_VALUE,
                                         QVariant(profileName),
                                         QVariant(KeyVolume),
                                         QVariant(QString(level)));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::setVolumeLevel() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        success = reply.arguments().at(0).toBool();
        qDebug() << "Profiles::setVolumeLevel() result:" << success;
        emit profileChanged(ProfileData(profileName, level, isVibrationEnabled(profileName)));
    }
    else {
        qDebug() << "Profiles::setVolumeLevel() reply type:" << reply.type();
    }
    return success;
}

bool Profiles::setVibration(QString profileName, bool enabled)
{
    bool success = false;
    QString val;
    QDBusMessage reply = managerIf->call(PROFILED_SET_VALUE,
                                         QVariant(profileName),
                                         QVariant(KeyVibra),
                                         QVariant(val.setNum((unsigned int)enabled)));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::setVibration() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage && reply.arguments().count() > 0) {
        success = reply.arguments().at(0).toBool();
        qDebug() << "Profiles::setVibration() result:" << success;
        emit profileChanged(ProfileData(profileName, getVolumeLevel(profileName), enabled));
    }
    else {
        qDebug() << "Profiles::setVibration() reply type:" << reply.type();
    }
    return success;
}

ProfileData Profiles::getProfile(QString profileName)
{
    ProfileData data;
    QDBusMessage reply = managerIf->call(PROFILED_CHANGED, QVariant(profileName));

    if(reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Profiles::setVibration() error reply:" << reply.errorName();
    }
    else if(reply.type() == QDBusMessage::ReplyMessage) {
       data.profileName = profileName;
       for(int i=0; i < reply.arguments().count(); ++i){
           QStringList value(reply.arguments().at(i).toStringList());
           if(value.count() > 2){ // TODO: '> 1' is enough...
               qDebug() << "Profiles::getProfile() result.at(" << i << "):" << value.at(0) << "," << value.at(1) << "," << value.at(2);
               if(value.at(0) == KeyVibra){
                    data.vibrationEnabeld = (bool)(value.at(1).toUInt());
                }
                else if(value.at(0) == KeyVolume){
                    data.volumeLevel = value.at(1).toInt();
                }
            }
       }
       qDebug() << "Profiles::getVolumeLevel() result count:" << reply.arguments().count();
    }
    else {
        qDebug() << "Profiles::setVibration() reply type:" << reply.type();
    }
    return data;
}


