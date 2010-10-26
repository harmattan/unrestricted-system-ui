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
#ifndef UT_VOLUMEBAR_H
#define UT_VOLUMEBAR_H

#include <QtTest/QtTest>
#include <QObject>
class VolumeBar;
class MApplication;
class MSceneManager;

class Ut_VolumeBar : public QObject
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void testUpdateVolume ();
    void testAnimations ();
    void testChangeVolume ();
    void cleanupTestCase ();
    
private:
    VolumeBar      *volumeBar;
    MApplication   *app;
};

#endif
