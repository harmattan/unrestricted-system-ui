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
#ifndef RADIOACCESS_H
#define RADIOACCESS_H

#include <QObject>

namespace Cellular
{

class RadioAccess : public QObject
{
    Q_OBJECT

public:
    RadioAccess(QObject *parent = 0);
    ~RadioAccess();

    enum Mode {
        UnknownMode = -1, //!< Radio access selection mode unknown
        DualMode, //!< Dual mode access selection in use
        GSMMode, //!< GSM only access selection mode in use
        UMTSMode //!< UMTS only access selection mode in use
    };

    enum Technology {
        UnknownTechnology = -1, //!< Current access technology is unknown
        GSM, //!< Using GSM
        UMTS //!< Using UMTS
    };

    enum State {
        Idle, //!< Radio access is idle
        AllocatedHSDPA, //!< High speed downlink allocated
        AllocatedHSUPA //!< High speed uplink allocated
    };

    Q_DECLARE_FLAGS(Flags, State);

    RadioAccess::Mode mode() const;
    RadioAccess::Technology technology() const;
    RadioAccess::Flags state() const;

public slots:
    //! Set radio access selection mode
    void setMode(RadioAccess::Mode mode);
    void setTechnology(RadioAccess::Technology technology);
    void setState(RadioAccess::State state);

signals:
    //! Radio access status changed
    void statusChanged(RadioAccess::Flags flags);
    //! Radio access selection mode changed
    void modeChanged(int mode);
    //! Radio access technology changed
    void technologyChanged(int technology);

private:
    RadioAccess::Mode currentMode;
    RadioAccess::Technology currentTechnology;
    RadioAccess::Flags flags;

};

}
#endif // RADIOACCESS_H
