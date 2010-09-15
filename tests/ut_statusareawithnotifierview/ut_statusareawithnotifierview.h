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

#include <QtTest/QtTest>
#include <QObject>

class StatusAreaWithNotifierView;
class StatusArea;
class MButton;
class MApplication;

class Ut_StatusAreaWithNotifierView : public QObject
{
    Q_OBJECT

public:
    static int windowExecutionCount;
    static int windowRejectionCount;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testNotifierAddedToLayout();
    void testStatusIndicatorMenuVisibilityChangedSignal();

signals:
    void statusIndicatorMenuVisibilityChanged(bool);

private:
    MApplication *app;
    StatusAreaWithNotifierView *m_subject;
    StatusArea *statusArea;
};

#endif
