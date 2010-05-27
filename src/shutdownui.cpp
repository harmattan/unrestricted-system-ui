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
#include <MFeedback>
#include <MApplication>
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
        m_Realized (false),
        m_SceneWindow (0),
        m_timer (new QTimer),
        m_Label1 (0),
        m_Label2 (0),
        m_Image (0),
        m_feedback (0)
{
    /*
     * We have to pre-created/load the shutdown ui content because when it
     * should show, the in should show quickly
     */
    QTimer::singleShot (500, this, SLOT (realize()));

    connect (m_timer, SIGNAL (timeout ()),
             this, SLOT (showLogo ()));
}

ShutdownUI::~ShutdownUI ()
{
    delete m_timer;
    delete m_SceneWindow;
}

void
ShutdownUI::realize ()
{
    SYS_DEBUG ("");

    if (m_Realized)
        return;

    // FIXME: we need logo for both orientations...
    setLandscapeOrientation ();
    lockOrientation ();

    // Initilaize non-graphical feedback
    m_feedback = new MFeedback (this);
#ifdef FEEDBACK_SUPPLIED
    m_feedback->setName ("DF_POWER_OFF");
#else
    m_feedback->setName ("press");
#endif

    //% "Shutting down"
    m_Label1 = new MLabel (qtTrId ("qtn_shut_down"));
    m_Label1->setObjectName ("shutdownTextBig");
    m_Label1->setAlignment (Qt::AlignCenter);

    //% "Good bye!"
    m_Label2 = new MLabel (qtTrId ("qtn_shut_greeting"));
    m_Label2->setObjectName ("shutdownTextSmaller");
    m_Label2->setAlignment (Qt::AlignCenter);

    /*
     * A full screen logo that we show when the labels are already gone.
     */
    m_Image = new MImageWidget ("nokia_logo");
    m_Image->hide ();

    QGraphicsLinearLayout *layout
        = new QGraphicsLinearLayout (Qt::Vertical);

    layout->addStretch ();
    layout->addItem (m_Label1);
    layout->addStretch ();
    layout->addItem (m_Image);
    layout->addStretch ();
    layout->addItem (m_Label2);
    layout->addStretch ();

    /*
     * Creating a scene window and putting everything into it.
     */
    m_SceneWindow = new MSceneWindow;
    m_SceneWindow->setLayout (layout);
    m_SceneWindow->appear (this);
    
    m_Realized = true;
}

// Hack for RFS/CUD
#ifdef NOTDEFINED
static const char * const ids [] =
{
    //% "Restoring settings"
    QT_TRID_NOOP("qtn_rset_restore_down"),
    //% "Clearing device"
    QT_TRID_NOOP("qtn_rset_clear_down"),
    //% "Please wait!"
    QT_TRID_NOOP("qtn_rset_wait"),
    0,
};
#endif

/*!
 * \param text1 The text of the primary message line
 * \param text2 The text of the secondary message line
 * \param timeout After this timeout we will hide the texts and show the logo
 *
 * Shows a full screen window with two lines of text, then waits for the
 * specified amount of time and will hide the text lines and show an image.
 * After the image image shown for some time will dimm the screen.
 */
void
ShutdownUI::showWindow (
        const QString    &text1, 
        const QString    &text2, 
        int               timeout)
{
    SYS_DEBUG ("");
    
    /*
     * If the widgets are not created we create them now.
     */
    if (!m_Realized)
        realize ();
    
    /*
     * We set the labels to show the text strings that we got.
     */
    if (! (text1.isEmpty () && text2.isEmpty ())) {
        if (text1.startsWith ("qtn"))
            m_Label1->setText (qtTrId (text1.toLatin1 ().constData ()));
        else
            m_Label1->setText (text1);

        if (text2.startsWith ("qtn"))
            m_Label2->setText (qtTrId (text2.toLatin1 ().constData ()));
        else
            m_Label2->setText (text2);
    }


    // Stop the previous timer...
    m_timer->stop ();

    /*
     *
     */
    show ();


    // Turn on
    Maemo::QmDisplayState  display;
    display.set (Maemo::QmDisplayState::On);

    m_feedback->play ();

    // Set the interval and start the timer...
    m_timer->setInterval (timeout);
    m_timer->start ();
}

void
ShutdownUI::showLogo ()
{
    SYS_DEBUG ("");
    m_timer->stop ();

    /*
     * We hide the labels and show the image.
     */
    m_Label1->hide ();
    m_Label2->hide ();
    m_Image->show ();
    m_Image->setGeometry (QRectF (0., 0., 864., 480.));

    QTimer::singleShot (2000, this, SLOT (turnOffScreen ()));
}

void
ShutdownUI::turnOffScreen ()
{
    bool success = false;

    SYS_DEBUG ("");

    #ifndef __i386__
    Maemo::QmDisplayState  display;

    // Try to dim
    success = display.set (Maemo::QmDisplayState::Dimmed);
    if (! success) {
        SYS_WARNING ("Dimming the display failed!");
    }

    // Try to turn off
    success = display.set (Maemo::QmDisplayState::Off);
    if (! success) {
        SYS_WARNING ("Turning off the display failed!");
    }
    #endif
    
    if (!success) {
        SYS_DEBUG ("No way dimming the screen on scratchbox.");
        QPalette Palette (palette());

        Palette.setColor(QPalette::Background, QColor ("black"));
        setPalette(Palette);

        setBackgroundRole (QPalette::Background);
        m_Image->hide ();
    }
}

