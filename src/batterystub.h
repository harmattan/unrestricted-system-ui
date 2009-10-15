#ifndef BATTERYSTUB_H
#define BATTERYSTUB_H

#include <QObject>
#include <QList>

// STUB CLASS

namespace Maemo {

class QmBattery : public QObject
{
    Q_OBJECT
public:

    /** Battery charge level states */
    enum Level
    {
        LevelFull = 0,        
        Level4,
        Level3,
        Level2,
        LevelLow,
        LevelCritical
    };

    /** Battery charging states */
    enum State
    {
        StateCharging = 0,   /**< Charging */
        StateNotCharging,     /**< Not charging */
        StateChargingFailed
    };

    /** Charging events */
    enum ChargerEvent
    {
        ChargerConnected,    /**< Charger connected */
        ChargerDisconnected, /**< Charger disconnected */
        ChargerNotCharging   /**< Charger not charging */
    };

    QmBattery(QObject *parent = 0);
    virtual ~QmBattery();

    /*
    * Current battery state
    * @return  QmBattery::State whether charging or not charging
    */
    QmBattery::State getState();

    /*
    * Current battery charge level
    * @return  QmBattery::Level whether battery is low/full/critical
    */
    QmBattery::Level getLevel();

    /*
    * Remaining battery time (idle), seconds, -1=not known
    * @return battery time in seconds if its not charging
    *	      -1 , if its charging
    */
    int  remainingTime();

    /*
    * Remaining battery time (talk time), seconds, -1=not known
    *  This will get changed after lower layer supports remaining talktime api
    * @return battery time in seconds if its not charging
    *	      -1 , if its charging
    */
    int  remainingTalkTime();



signals:
    /**
    * Sent when battery charging state has changed.
    * @param state Current battery charging status.
    */
    void batteryStatusChanged(Maemo::QmBattery::State state);

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
    * @param bars_now current number of bars remaining
    * @param bars_max maximum number of bars
    */
    void batteryStateChanged(int bars_now, int bars_max);

private slots:
    void updateValues();

private: //methods
    void initValues();

private: //attributes
    QList<QmBattery::Level> levels;
    int levelIndex;
    int levelIndexIncrease;
    bool charging;

};
}

#endif // BATTERYSTUB_H
