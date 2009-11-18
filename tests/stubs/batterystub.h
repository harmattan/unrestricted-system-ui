#ifndef BATTERYSTUB_H
#define BATTERYSTUB_H

#include <QObject>
#include <QList>

// STUB CLASS

namespace Maemo {

#ifdef UTILISE_BATTERY_USER
class QmBatteryUser : public QObject
{
    Q_OBJECT
public:
    QmBatteryUser() { ; }
    virtual ~QmBatteryUser() { ; }

public slots:
    void start();

signals:
    void changeLevel();
};
#endif

class QmBattery : public QObject
{
    Q_OBJECT
public:

    /** Battery charge level states */
    enum Level
    {
        LevelFull = 0,       /**< Battery full */
        LevelLow,            /**< Battery low */
        LevelCritical        /**< Battery level critically low */
    };

    /** The mode in which the remaining time is to be calculated */
    enum RemainingTimeMode
    {
        NormalMode,       /**< In normal mode */
        PowersaveMode     /**< In powersave mode */
    };

    /** The type of charger connected */
    enum ChargerType
    {        
        Wall,                /**< Wall charger  */
        USB_500mA,           /**< USB with 500mA output */
        USB_100mA            /**< USB with 100mA output */
    };

    /** Battery charging states */
    enum State
    {
        StateCharging = 0,    /**< Charging */
        StateNotCharging,     /**< Not charging */
        StateChargingFailed
    };

    /** Charging events */
    enum ChargerEvent
    {
        ChargerConnected,    /**< Charger connected */
        ChargerDisconnected, /**< Charger disconnected */        
    };

    QmBattery(QObject *parent = 0);
    virtual ~QmBattery();

    /*
    * Current battery charge level
    * @return  QmBattery::Level whether battery is low/full/critical
    */
    QmBattery::Level getLevel();    

    /*
    * Current battery state
    * @return  QmBattery::State whether charging or not charging
    */
    QmBattery::State getState();


    /*
     * Get current charger type
     * @return  QmBattery::ChargerType charger type
     */
    QmBattery::ChargerType getChargerType();

    /*
     * Get current battery energy level.
     * @return  Battery level in percentages
     */
    int getBatteryEnergyLevel();       

    /* Remaining battery time (idle), seconds
     * @param mode The mode
     * @return battery time in seconds, -1 if not available
     */
    int remainingTime(QmBattery::RemainingTimeMode mode);

    /*
     * Remaining battery time (talk time), seconds
     * @param mode The mode
     * @return battery time in seconds, -1 if not available
     */
    int remainingTalkTime(QmBattery::RemainingTimeMode mode);

signals:

    /**
    * Sent when battery charge level has changed. See enumeration of
    * #Maemo::QmBattery::Level
    * @param level Current battery charge level.
    */
    void batteryLevelChanged(Maemo::QmBattery::Level level);

    /**
    * Sent when a charger event has occurred.
    * @param event Charger event.
    */
    void chargerEvent(Maemo::QmBattery::ChargerEvent event);

    /**
     * Sent when the number of bars indicating battery charge changes
     * @param percentages the battery energy level in percentages
     * @param bars_max maximum number of bars
     */
    void batteryEnergyLevelChanged(int percentages);

    /**
    * Sent when battery charging state has changed.
    * @param state Current battery charging status.
    */
    void batteryStatusChanged(Maemo::QmBattery::State state);

private slots:
    void changeLevel(); //own addition

private: //methods
    void initValues();

private: //attributes
    QList<QmBattery::Level> levels;
    int levelIndex;
    int energyLevel;    
    QmBattery::State state;
    QmBattery::ChargerType type;
#ifdef UTILISE_BATTERY_USER
    QmBatteryUser *batteryUser;
#endif

};
}

#endif // BATTERYSTUB_H
