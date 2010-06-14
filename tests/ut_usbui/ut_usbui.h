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
#ifndef _UT_USBUI_H
#define _UT_USBUI_H

#include <QtTest/QtTest>
#include <QObject>

#include <usbui.h>

class Ut_UsbUi : public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase ();
        void cleanupTestCase ();

#ifdef HAVE_QMSYSTEM
        void show_hide_dialog ();
        void usbnotifications ();
        void testdialogbuttoncallbacks ();
#endif

    private:
        UsbUi   *m_subject;
};

#endif
