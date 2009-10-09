#include "profilebusinesslogicadaptor.h"
#include "profilebusinesslogic.h"

ProfileBusinessLogicAdaptor::ProfileBusinessLogicAdaptor(QObject *obj, ProfileBusinessLogic* logic) :
        QDBusAbstractAdaptor(obj),
        logic(logic)
{
    connect(logic, SIGNAL(currentProfile(int)), this, SIGNAL(currentProfile(int)));
    connect(logic, SIGNAL(volumeLevels(QStringList)), this, SIGNAL(volumeLevels(QStringList)));
    connect(logic, SIGNAL(vibrationValue(int, bool)), this, SIGNAL(vibrationValue(int, bool)));
    connect(logic, SIGNAL(volumeLevel(int, int)), this, SIGNAL(volumeLevel(int, int)));
}

ProfileBusinessLogicAdaptor::~ProfileBusinessLogicAdaptor()
{
}

void ProfileBusinessLogicAdaptor::getCurrentProfile()
{
    logic->getCurrentProfile();
}

void ProfileBusinessLogicAdaptor::getVolumeLevels()
{
    logic->getVolumeLevels();
}

void ProfileBusinessLogicAdaptor::getVibrationValues()
{
    logic->getVibrationValues();
}

void ProfileBusinessLogicAdaptor::setProfile(int value)
{
    logic->setProfile(value);
}

void ProfileBusinessLogicAdaptor::setVibration(int id, bool value)
{
    logic->setVibration(id, value);
}

void ProfileBusinessLogicAdaptor::setVolumeLevel(int id, int value)
{
    logic->setVolumeLevel(id, value);
}
