/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef DISPLAYDBUSINTERFACE_H
#define DISPLAYDBUSINTERFACE_H

#include <QObject>
#include <QStringList>
#include <QDBusError>

#if 0
/*
 * Removed the class because was not used at all. The file itself will be
 * removed soon if noone touches the file.
 */
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
    void DBusMessagingFailure (QDBusError error);

signals:
    void brightnessValuesReceived(int, QStringList);
    void screenLightsValuesReceived(int, QStringList);
    void blankInhibitValueReceived(bool);

private:
    QDBusInterface *dbusIf;

};
#endif
#endif // DISPLAYDBUSINTERFACE_H
