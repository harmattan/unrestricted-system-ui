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
#ifndef UT_STATUSAREAVIEW_H
#define UT_STATUSAREAVIEW_H

#include <QObject>

class MApplication;
class NotifierNotificationSink;
class StatusAreaView;
class StatusArea;

class Ut_StatusAreaView : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSignalConnections();

private:
    MApplication *app;
    NotifierNotificationSink *notifierNotificationSink;
    StatusAreaView *m_subject;
    StatusArea *statusArea;
};

#endif
