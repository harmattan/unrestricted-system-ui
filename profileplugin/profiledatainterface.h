#ifndef PROFILEDATAINTERFACE_H
#define PROFILEDATAINTERFACE_H

#include <QObject>

class Profile;

class ProfileDataInterface : public QObject
{
public:
    ProfileDataInterface();
    virtual ~ProfileDataInterface();

    QStringList profileNames();
    QString currentProfileName();    
    bool setProfile(const QString &profileName);
    QString profileName2Text(const QString &profileName);

private: //attributes
    Profile *profileAPI;    

};

#endif // PROFILEDATAINTERFACE_H
