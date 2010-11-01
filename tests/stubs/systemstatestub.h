/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef SYSTEMSTATESTUB_H
#define SYSTEMSTATESTUB_H

#include <QObject>

// STUB CLASS

namespace MeeGo {

/**
 * @brief Provides information and actions on device state.
 */
class QmSystemState : public QObject
{
    Q_OBJECT
public:
    /** States the user can set */
    enum NextState
    {
        Powerup = 0,        /**< Powerup from acting dead */
        Reboot,             /**< Reboot */
        ShuttingDown        /**< Shutdown */
    };

    /** State indication */
    enum StateIndication
    {
        Shutdown = 0, 		/**< Normal shutdown */
        ThermalStateFatal,	/**< Shutdown due to thermal state */
        BatteryStateEmpty,	/**< Shutdown due battery empty within few seconds */
        SaveData,           /**< Save data */
        RebootDeniedUSB,    /**< Reboot denied because USB is connected in mass storage mode */
        ShutdownDeniedUSB   /**< Shutdown denied because USB is connected in mass storage mode */
    };

public:
    QmSystemState(QObject *parent = 0);
    ~QmSystemState();

    /**
     * Request to system to move into state @c nextState.
     * <p><b>Credentials</b>:<br>
     *   <em>access-power-mgmt</em> Resource token required to set the device state.
     * @param New state.
     * @return True on success, false otherwise.
     */
    bool set(NextState nextState);

private slots:
    void emitShutdown();
    void emitThermalShutdown(QString);
    void emitBatteryShutdown();
    void emitSaveData();
    void emitShutdownDenied(QString, QString);

signals:
    /**
     * Sent when device state indication has been received.
     * @param what Received state indication type.
     */
    void systemStateChanged(MeeGo::QmSystemState::StateIndication what);
};
}

#endif // SYSTEMSSTATESTUB_H
