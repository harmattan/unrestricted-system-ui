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
    void blankTimeoutValuesRequired();
    void blankTimeoutValueRequired();
    void dimTimeoutValuesRequired();
    void dimTimeoutValueRequired();
    void blankInhibitValueRequired();
    void setBrightnessValue(const QString &value);
    void setBlankTimeoutValue(const QString &value);
    void setDimTimeoutValue(const QString &value);
    void setBlankInhibitValue(bool value);

private slots:
    void valueSet();
    void DBusMessagingFailure();    

signals:
    void brightnessValuesReceived(QStringList);
    void brightnessValueReceived(QString);
    void blankTimeoutValuesReceived(QStringList);
    void blankTimeoutValueReceived(QString);
    void dimTimeoutValuesReceived(QStringList);
    void dimTimeoutValueReceived(QString);
    void blankInhibitValueReceived(bool);

private:
    QDBusInterface *dbusIf;

};

#endif // DISPLAYDBUSINTERFACE_H
