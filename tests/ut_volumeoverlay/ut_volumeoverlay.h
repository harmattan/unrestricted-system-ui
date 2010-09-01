/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
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
#ifndef UT_VOLUMEOVERLAY_H
#define UT_VOLUMEOVERLAY_H

#include <QtTest/QtTest>
#include <QObject>
class VolumeOverlay;
class MApplication;

class Ut_VolumeOverlay : public QObject
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void testUpdateVolume ();
    void testHideMe ();
    void cleanupTestCase ();
    
private:
    VolumeOverlay  *m_Api;
    MApplication   *m_App;

};

#endif
