#ifndef PROFILEBUSINESSLOGICADAPTOR_H
#define PROFILEBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

class ProfileBusinessLogic;

class ProfileBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.profile")

public:
    ProfileBusinessLogicAdaptor();

public:
    ProfileBusinessLogicAdaptor(QObject *obj, ProfileBusinessLogic* logic);
    virtual ~ProfileBusinessLogicAdaptor();

public slots:
    Q_NOREPLY void getCurrentProfile();
    Q_NOREPLY void getVolumeLevels();
    Q_NOREPLY void getVibrationValues();
    Q_NOREPLY void setProfile(int value);
    Q_NOREPLY void setVibration(int id, bool value);
    Q_NOREPLY void setVolumeLevel(int id, int value);

signals:
    void currentProfile(int id);
    void volumeLevels(QStringList levels);
    void vibrationValue(int id, bool value);
    void volumeLevel(int id, int value);

private: //attributes
    ProfileBusinessLogic* logic;
};

#endif // PROFILEBUSINESSLOGICADAPTOR_H
