#ifndef DISPLAYDBUSINTERFACE_H
#define DISPLAYDBUSINTERFACE_H

#include <QObject>
#include <QStringList>

class QDBusInterface;

class DisplayDBusInterface : public QObject
{
    Q_OBJECT

public:
    DisplayDBusInterface();
    virtual ~DisplayDBusInterface();

    void brightnessValuesRequired();
    void brightnessValueRequired();
    void screenLightsValuesRequired();
    void screenLightsValueRequired();
    void screenLightsToggleValueRequired();
    void setBrightnessValue(const QString &value);
    void setScreenLightsValue(const QString &value);    
    void setScreenLightsToggleValue(bool value);

private slots:
    void valueSet();
    void DBusMessagingFailure();    

signals:
    void brightnessValuesReceived(QStringList);
    void brightnessValueReceived(QString);
    void screenLightsValuesReceived(QStringList);
    void screenLightsValueReceived(QString);
    void screenLightsToggleValueReceived(bool);

private:
    QDBusInterface *dbusIf;


};

#endif // DISPLAYDBUSINTERFACE_H
