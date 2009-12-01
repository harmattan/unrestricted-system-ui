#ifndef PROFILEDATAINTERFACE_H
#define PROFILEDATAINTERFACE_H

#include <QObject>

class Profile;

class ProfileDataInterface : public QObject
{
    Q_OBJECT

public:
    ProfileDataInterface();
    virtual ~ProfileDataInterface();

    QStringList profileNames();
    QString currentProfileName();    
    bool setProfile(const QString &profileName);
    QString profileName2Text(const QString &profileName);

signals:
    void currentProfileNameChanged(QString);

private: //attributes
    Profile *profileAPI;    

};

#endif // PROFILEDATAINTERFACE_H
