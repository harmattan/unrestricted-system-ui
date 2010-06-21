/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#ifndef _UT_NGFADAPTER_
#define _UT_NGFADAPTER_

#include <QtTest/QtTest>
#include <QObject>

#include "ngfadapter.h"

class Ut_NGFAdapter : public QObject
{
    Q_OBJECT

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    //! Test the feedback playing and stopping
    void testPlayAndStop();

    //! Test that the adapter behaves correctly even if the client creation fails
    void testNgfClientCreationFails();

private:
    //! The test subject
    NGFAdapter* adapter;

};
#endif //_UT_NGFADAPTER_
