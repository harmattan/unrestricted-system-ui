/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYBUSINESSLOGICADAPTOR_H
#define BATTERYBUSINESSLOGICADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

class BatteryBusinessLogic;

/*!
 * Implements the DBus API interface for the battery charging status, the power
 * save mode system and the remaining use time system so that the applications
 * can get the information and change the settings through the DBus system.
 */
class BatteryBusinessLogicAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.battery")

public:
    BatteryBusinessLogicAdaptor (
            QObject              *obj, 
            BatteryBusinessLogic *batteryLogic);

signals:
    void batteryCharging (int);
    void batteryNotCharging ();
    void batteryBarValueChanged (int);
    /*!
     * Power save mode turned on/off.
     */
    void PSMValueChanged (bool);
    void PSMAutoDisabled ();
    void remainingTimeValuesChanged (QStringList);

public slots:
    Q_NOREPLY void setPSMValue (bool toggle);
    Q_NOREPLY void setPSMAutoValue (bool disabled);
    Q_NOREPLY void setPSMThresholdValue (const QString &value);
    bool PSMValue();
    bool PSMAutoValue();
    void batteryChargingState();
    int batteryBarValue();
    QString PSMThresholdValue();
    QStringList PSMThresholdValues();
    QStringList remainingTimeValues();

private:
    BatteryBusinessLogic *m_BatteryLogic;
};

#endif // BATTERYBUSINESSLOGICADAPTOR_H
