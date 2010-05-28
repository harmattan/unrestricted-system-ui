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
#ifndef USBUI_H
#define USBUI_H

#include <QObject>

#ifndef UNIT_TEST
#include <qmusbmode.h>
#include <qmlocks.h>
#else
#include "usbmode_stub.h"
#include "locksstub.h"
#endif

class MDialog;
class MNotification;
class QProcess;

class UsbUi : public QObject
{
    Q_OBJECT

    public:
        UsbUi (QObject *parent = 0);
        ~UsbUi ();

    private slots:
        void currentModeChanged (Maemo::QmUSBMode::Mode mode);
        void locksChanged (Maemo::QmLocks::Lock what, Maemo::QmLocks::State how);
        void OviSuiteSelected ();
        void MassStorageSelected ();
        void ShowDialog ();

        void initialize ();

    private:
        void ShowNotification (int id);

        Maemo::QmUSBMode    *m_logic;
        Maemo::QmLocks      *m_locks;
        MNotification       *m_notification;
        MDialog             *m_dialog;
        bool                 m_showdialog;
        QProcess            *m_process;

#ifdef UNIT_TEST
    friend class Ut_UsbUi;
#endif
};

#endif
