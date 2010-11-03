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
#include <MStylableWidget>
#include <MLocale>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(ShutdownUI)

// For WM_SET_NAME:
#include <X11/Xlib.h>
#include <X11/Xatom.h>

ShutdownUI::ShutdownUI () :
        m_Realized (false),
        m_SceneWindow (0),
        m_Timer (new QTimer),
        m_Label1 (0),
        m_Label2 (0),
        m_logo (0),
        m_layout (0),
        m_Feedback (0)
{
    setObjectName ("ShutdownUIWindow");
    /*
     * We have to pre-created/load the shutdown ui content because when it
     * should show, the in should show quickly
     */
    QTimer::singleShot (500, this, SLOT (realize()));

    connect (m_Timer, SIGNAL (timeout ()),
             this, SLOT (showLogo ()));
}

ShutdownUI::~ShutdownUI ()
{
    delete m_Timer;

    if (m_SceneWindow) {
        delete m_SceneWindow;
    }
    // FIXME: What about m_Feedback?
}

/*
 * Here we create the widgets that we use, and we put them into the layout that
 * we use. The logo will not be shown yet.
 */
void
ShutdownUI::realize ()
{
    if (m_Realized)
        return;

    // Initilaize non-graphical feedback
    m_Feedback = new MFeedback (this);
    m_Feedback->setName ("power-off");

    //% "Shutting down"
    m_Label1 = new MLabel (qtTrId ("qtn_shut_down"));
    m_Label1->setAlignment (Qt::AlignCenter);
    m_Label1->setObjectName ("shutdownTextFirst");

    //% "Good bye!"
    m_Label2 = new MLabel (qtTrId ("qtn_shut_greeting"));
    m_Label2->setAlignment (Qt::AlignCenter);
    m_Label2->setObjectName ("shutdownTextSecond");

    /*
     * A full screen logo that we show when the labels are already gone.
     */
    m_logo = new MStylableWidget;
    m_logo->setObjectName ("shutdownLogo");

    m_layout = new QGraphicsLinearLayout (Qt::Vertical);
    m_layout->setContentsMargins (0., 0., 0., 0.);
    m_layout->setSpacing (0.);

    m_layout->addItem (m_Label1);
    m_layout->addItem (m_Label2);

    /*
     * Creating a scene window and putting everything into it.
     */
    m_SceneWindow = new MSceneWindow;
    m_SceneWindow->setObjectName ("shutdownWindow");
    m_SceneWindow->setContentsMargins (0., 0., 0., 0.);
    m_SceneWindow->setLayout (m_layout);
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
    m_Timer->stop ();

    /*
     * It is as simple as this when we use MWindow.
     */
    show ();

    m_Feedback->play ();

    // Set the interval and start the timer to the next phase: hiding the labels
    // and showing the logo.
    m_Timer->setInterval (timeout);
    m_Timer->start ();
}

/*
 * Hides the labels, shows the logo image and starts up a timer to turn off the
 * screen.
 */
void
ShutdownUI::showLogo ()
{
    m_Timer->stop ();

    /*
     * We hide the labels and show the image.
     */
    for (int i = m_layout->count (); i > 0; i--)
        m_layout->removeAt (0);

#if 0
    /* Logo available only for portait ATM */
    sceneManager ()->setOrientationAngle (M::Angle0, MSceneManager::ImmediateTransition);
    lockOrientation ();
#endif

    m_layout->addItem (m_logo);

    delete m_Label1;
    delete m_Label2;

    QTimer::singleShot (2000, this, SLOT (turnOffScreen ()));
}

/*!
 * We need to turn off the screen so that the user will not see the actual
 * shutdown on the GUI. We could show a black screen but we might be prematurely
 * killed, so this window will be removed early. Turning off the screen is an
 * excellent way to solve this issue... except that we might be killed even
 * sooner.
 */
void
ShutdownUI::turnOffScreen ()
{
    bool success = false;

    /*
     * No way dimming or turning off the screen inside scratchbox.
     */
    #if defined(HAVE_QMSYSTEM)
    MeeGo::QmDisplayState  display;

    // Try to dim
    success = display.set (MeeGo::QmDisplayState::Dimmed);

    // Try to turn off
    success = display.set (MeeGo::QmDisplayState::Off);
    #endif

    if (!success) {
        QPalette Palette (palette());

        Palette.setColor(QPalette::Background, QColor ("black"));
        setPalette(Palette);

        setBackgroundRole (QPalette::Background);
        m_logo->hide ();
    }
}

/*
 * Set the highest stacking layer here 
 * and the window-name for debugging purposes
 */
void 
ShutdownUI::showEvent (
        QShowEvent *event)
{
    Q_UNUSED (event);

    Window      windowID;
    Display    *display;
    Atom        nameAtom;
    Atom        utf8StringAtom;
    Atom        stackingLayerAtom;
    const char *windowName = "ShutdownUI";
    long        layer = 6;

    display = QX11Info::display ();
    if (!display) {
        return;
    }
    
    stackingLayerAtom = XInternAtom (display, "_MEEGO_STACKING_LAYER", False);
    nameAtom = XInternAtom (display, "_NET_WM_NAME", False);
    utf8StringAtom = XInternAtom (display, "UTF8_STRING", False);

    windowID = internalWinId();
    if (windowID == None) {
        return;
    }

    /*
     * Setting the stacking layer.
     */
    if (stackingLayerAtom != None)
        XChangeProperty (display, windowID, stackingLayerAtom, XA_CARDINAL, 
                32, PropModeReplace, (unsigned char*)&layer, 1);

    /*
     * Setting the name.
     */
    if (nameAtom != None && utf8StringAtom != None)
        XChangeProperty (display, windowID, nameAtom, utf8StringAtom, 
                8, PropModeReplace, 
                (unsigned char *) windowName, strlen(windowName));
}

