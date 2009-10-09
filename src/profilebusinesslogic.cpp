#include "profilebusinesslogic.h"

#include <QDebug>
#include <QStringList>
#include <QList>

ProfileBusinessLogic::ProfileBusinessLogic()
{
}

ProfileBusinessLogic::~ProfileBusinessLogic()
{
}

void ProfileBusinessLogic::getCurrentProfile()
{
    qDebug() << "ProfileBusinessLogic::getCurrentProfile()";
    int id = 0;
    // TODO: get correct functionality
    emit currentProfile(id);
}

void ProfileBusinessLogic::getVolumeLevels()
{
    qDebug() << "ProfileBusinessLogic::getVolumeLevels()";

    // TODO: get correct functionality
    QStringList levels;
    levels << QString("1") << QString("5") << QString("10");

    emit volumeLevels(levels);

    emit volumeLevel(0, 1);
    emit volumeLevel(1, -1);
    emit volumeLevel(2, 0);
    emit volumeLevel(3, 2);
}

void ProfileBusinessLogic::getVibrationValues()
{
    qDebug() << "ProfileBusinessLogic::getVibrationValues()";

    // TODO: get correct functionality
    emit vibrationValue(0, true);
    emit vibrationValue(1, false);
    emit vibrationValue(2, true);
    emit vibrationValue(3, true);
}

void ProfileBusinessLogic::setProfile(int value)
{
    qDebug() << "ProfileBusinessLogic::setProfile(" << value << ")";
}

void ProfileBusinessLogic::setVibration(int id, bool value)
{
    qDebug() << "ProfileBusinessLogic::setVibration(" << id << "," << value <<  ")";
}

void ProfileBusinessLogic::setVolumeLevel(int id, int value)
{
    qDebug() << "ProfileBusinessLogic::setVolumeLevel(" << id << "," << value <<  ")";
}


