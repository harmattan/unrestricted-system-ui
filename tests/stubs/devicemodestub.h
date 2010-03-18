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
#ifndef DEVICEMODESTUB_H
#define DEVICEMODESTUB_H

#include <QObject>

// STUB CLASS

namespace Maemo
{

class QmDeviceMode : public QObject
{
    Q_OBJECT
public:

    /** Possible power save modes for the device. */
    enum PSMState {
        PSMError = -1,    /**< Power save mode could not be figured out. */
        PSMStateOff = 0,  /**< Power save mode is off */
        PSMStateOn        /**< Power save mode is on */
    };

    QmDeviceMode(QObject *parent = 0);
    virtual ~QmDeviceMode();

    /**
     * Get current power save mode.
     * @return Currenet power save mode.
     */
    PSMState getPSMState() const;

    /**
     * Set device power save mode.
     * @param state Power state mode to set.
     * @return True on success, false otherwise.
     */
    bool setPSMState(PSMState state);


signals:
    /** Sent when device power save mode has changed.
     * @param state Current power save mode.
     */
    void devicePSMStateChanged(Maemo::QmDeviceMode::PSMState state);

private:
    PSMState state;
};
}

#endif // DEVICEMODESTUB_H
