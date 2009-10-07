#ifndef PROFILEDBUSINTERFACE_H
#define PROFILEDBUSINTERFACE_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

class profileDBusInterface : public QObject
{
    Q_OBJECT

public:
    profileDBusInterface();
    virtual ~profileDBusInterface();

    void brightnessValuesRequired();    
    void screenLightsValuesRequired();    
    void blankInhibitValueRequired();
    void setBrightnessValue(const QString &value);
    void setScreenLightsValue(const QString &value);
    void setBlankInhibitValue(bool value);

private slots:
    void valueSet();
    void DBusMessagingFailure();    

signals:
    void brightnessValuesReceived(int, QStringList);
    void screenLightsValuesReceived(int, QStringList);
    void blankInhibitValueReceived(bool);

private:
    QDBusInterface *dbusIf;

};

#endif // PROFILEDBUSINTERFACE_H
