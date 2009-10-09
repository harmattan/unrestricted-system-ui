#ifndef PROFILEBUSINESSLOGIC_H
#define PROFILEBUSINESSLOGIC_H

#include <QObject>
#include <QStringList>

class ProfileBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ProfileBusinessLogic();
    virtual ~ProfileBusinessLogic();

public:
    void getCurrentProfile();
    void getVolumeLevels();
    void getVibrationValues();
    void setProfile(int value);
    void setVibration(int id, bool value);
    void setVolumeLevel(int id, int value);

signals:
    void currentProfile(int id);
    void volumeLevels(QStringList levels);
    void vibrationValue(int id, bool value);
    void volumeLevel(int id, int value);

private:

};

#endif // PROFILEBUSINESSLOGIC_H
