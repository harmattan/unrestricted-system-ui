/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
**
****************************************************************************/
#ifndef UT_LOCKSCREENVIEW_H
#define UT_LOCKSCREENVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MSceneWindow>
#include "lockscreenview.h"

class MApplication;

class Ut_LockScreenView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

private:
    LockScreenView* lockScreenView;
    MApplication* app;
    MSceneWindow controller;
};

#endif
