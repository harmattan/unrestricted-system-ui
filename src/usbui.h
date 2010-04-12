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
#include "usbbusinesslogic.h"

class MDialog;
class MNotification;

class UsbUi : public QObject
{
    Q_OBJECT

    public:
        UsbUi (QObject *parent = 0);
        ~UsbUi ();
        UsbBusinessLogic * getLogic ();

    private slots:
        void ShowDialog ();
        void UsbEvent (bool Connected);
        void OviSuiteSelected ();
        void MassStorageSelected ();

    private:
        UsbBusinessLogic    *m_logic;
        MNotification     *m_notification;
        MDialog           *m_dialog;

        void show_notification (int id);
};

#endif
