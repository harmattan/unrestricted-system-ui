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
#include "shutdownui.h"
#include "sysuid.h"

#include <MLabel>
#include <QTimer>
#include <QGraphicsLinearLayout>
#include <MSceneManager>
#include <MImageWidget>
#include <MApplicationWindow>
#include <MLocale>

#include <qmdisplaystate.h>

#define DEBUG
#define WARNING
#include "debug.h"

ShutdownUI::ShutdownUI () :
        m_text1 (0),
        m_text2 (0),
        m_logo (0)
{
    setPannable (false);
    setComponentsDisplayMode (MApplicationPage::AllComponents,
                              MApplicationPageModel::Hide);
}

ShutdownUI::~ShutdownUI ()
{

}

void
ShutdownUI::createContent ()
{
    MApplicationPage::createContent ();

    //% "Shutting down"
    m_text1 = new MLabel (qtTrId ("qtn_shut_down"));
    m_text1->setObjectName ("shutdownTextBig");
    m_text1->setAlignment (Qt::AlignCenter);

    //% "Good bye!"
    m_text2 = new MLabel (qtTrId ("qtn_shut_greeting"));
    m_text2->setObjectName ("shutdownTextSmaller");
    m_text2->setAlignment (Qt::AlignCenter);

    QGraphicsLinearLayout *layout
        = new QGraphicsLinearLayout (Qt::Vertical);

    layout->addStretch ();
    layout->addItem (m_text1);
    layout->addStretch ();
    layout->addItem (m_text2);
    layout->addStretch ();

    setLayout (layout);

    m_logo = new MImageWidget ("nokia_logo");
    m_logo->setGeometry (QRectF (0., 0., 864., 480.));
}

void
ShutdownUI::showWindow()
{
    SYS_DEBUG ("");

    MApplicationWindow &win = Sysuid::sysuid ()->applicationWindow ();

    if (win.isHidden ())
    {
        win.show ();
        win.showFullScreen ();
    }

    QTimer::singleShot (2000, this, SLOT (showLogo ()));
}

void
ShutdownUI::showLogo ()
{
    SYS_DEBUG ("");

    // FIXME: we need logo for both orientations...
    Sysuid::sysuid ()->applicationWindow ().setLandscapeOrientation ();
    Sysuid::sysuid ()->applicationWindow ().lockOrientation ();

    delete m_text1;
    delete m_text2;

    setCentralWidget (m_logo);

    QTimer::singleShot (2000, this, SLOT (turnOffScreen ()));
}

void
ShutdownUI::turnOffScreen ()
{
    SYS_DEBUG ("");
    bool success;

    Maemo::QmDisplayState  display;

    // Try to dim
    success = display.set (Maemo::QmDisplayState::Dimmed);

    if (! success)
    {
        SYS_WARNING ("Dimming the display failed!");
    }

    // Try to turn off
    success = display.set (Maemo::QmDisplayState::Off);

    if (! success)
    {
        SYS_WARNING ("Turning off the display failed!");
    }
}

