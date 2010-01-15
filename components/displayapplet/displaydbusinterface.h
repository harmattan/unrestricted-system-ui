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
    void screenLightsValuesRequired();
    void blankInhibitValueRequired();

public slots:
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

#endif // DISPLAYDBUSINTERFACE_H
