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
#include <QDebug>
#include <QTimer>
#include <QPointF>
#include <QGraphicsLinearLayout>
#include <MGConfItem>
#include <MWidget>
#include <MFeedback>
#include <MImageWidget>
#include <MViewCreator>
#include <QGraphicsSceneMouseEvent>
#include <MSceneManager>

#include "lockscreenviewwithpadlock.h"
#include "lockscreen.h"
#include "unlockwidgets.h"

// For WM_SET_NAME:
#include <X11/Xlib.h>
#include <X11/Xatom.h>

LockScreenViewWithPadlock::LockScreenViewWithPadlock(MSceneWindow* controller) :
    LockScreenView(controller),
    dragAndDropWindow (dynamic_cast<MWindow*>(controller->parent())),
    dragAndDropIcon (0),
    dragAndDropState (STATE_NONE),
    lockLiftArea ( new UnlockHeader),
    lockLandArea (new UnlockArea),
    drawPending(false),
    layoutPolicy(new QGraphicsLinearLayout (Qt::Vertical)),
    notificationArea(new UnlockNotifications),
    controller(controller)
{
    dragAndDropOverlay.setVisible (false);
    dragAndDropOverlay.setManagedManually (true);
    setObjectName ("LockScreenViewWithPadlock");

    layoutPolicy->setContentsMargins (0., 0., 0., 0.);
    layoutPolicy->setSpacing (0.);

    notificationArea->setVisible (false);

    connect (notificationArea, SIGNAL (needToShow (bool)),this, SLOT (showHideNotifications (bool)), Qt::DirectConnection);

    layoutPolicy->addItem (lockLiftArea);
    layoutPolicy->addItem (lockLandArea);

    // Set the main layout
    controller->setLayout(layoutPolicy);
    connect (this, SIGNAL (unlocked ()), controller, SLOT (sliderUnlocked ()), Qt::DirectConnection);
    connect(controller, SIGNAL(dateTimeChanged()), this, SLOT(updateDataTime()));
}

LockScreenViewWithPadlock::~LockScreenViewWithPadlock ()
{
}

void LockScreenViewWithPadlock::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    bool startDragAndDrop = false;

    if (lockLiftArea == 0)
        return;

    if ((lockLiftArea->pos ().y () + lockLiftArea->preferredHeight ()) > event->pos ().y ())
    {
        if (lockLiftArea->layoutDirection () == Qt::RightToLeft)
        {
            // Icon position in RTL layout direction
            // TODO: What is this 160 ?
            startDragAndDrop = event->pos ().x () < 160;
        }
        else
        {
            // Icon position in LTR layout direction
            // TODO: What is this 160 ?
            startDragAndDrop = event->pos ().x () > lockLiftArea->preferredWidth () - 160;
        }
    }

    // We should go to STATE_MOVING state if user tappend on
    // the top-right corner of the window...
    if (startDragAndDrop)
    {
        dragAndDropState = STATE_MOVING;

        // Move the icon to the start position:
        QSizeF size = dragAndDropOverlay.preferredSize ();

        dragAndDropPosition.setX (event->pos ().rx () - (size.width () / 1.4));
        dragAndDropPosition.setY (event->pos ().ry () - (size.height () / 1.4));

        redraw ();

        static_cast<UnlockHeader*>(lockLiftArea)->setActive (false);
        static_cast<UnlockArea*> (lockLandArea)->setEnabled (true);

        /*
         * Playing the appropriate feedback.
         */
        MFeedback feedback ("start-dragndrop");
        feedback.play ();
    }
    else
        dragAndDropState = STATE_NONE;
}

void LockScreenViewWithPadlock::redraw ()
{
    if (drawPending == true)
        return;

    drawPending = true;
    QTimer::singleShot (50, this, SLOT (redrawIdle ()));
}

void LockScreenViewWithPadlock::redrawIdle ()
{
    drawPending = false;

    dragAndDropOverlay.setPos (dragAndDropPosition);
    updateDragAndDropIcon();
}

void LockScreenViewWithPadlock::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    if (lockLandArea == 0)
        return;

    if (dragAndDropState == STATE_NONE)
        return;

    // First of all, icon should follow the mouse positions:
    QSizeF size = dragAndDropOverlay.preferredSize ();
    QPointF pos (event->pos ());

    pos.rx () -= (size.width  () / 1.4);
    pos.ry () -= (size.height () / 1.4);
    dragAndDropOverlay.setPos (pos);

    // And then check which icon we need to show:
    int newState = STATE_MOVING;

    // Check whether the DnD icon is inside the lock-land area...
    if (event->pos ().y () > lockLandArea->pos ().y ()) {
        newState = STATE_MOVING_ACTIVE;
    }

    // To avoid unnecessary screen updates...
    if (newState != dragAndDropState)
    {
        MFeedback feedback;
        dragAndDropState = newState;
        switch (newState)
        {
            case STATE_MOVING_ACTIVE:
                /*
                 * Entered the active area.
                 */
                feedback.setName ("enter-dragndrop-dropzone");
                feedback.play ();
                static_cast<UnlockArea*>(lockLandArea)->setActive (true);
                break;
            case STATE_MOVING:
            default:
                /*
                 * Exited the active area.
                 */
                feedback.setName ("exit-dragndrop-dropzone");
                feedback.play ();
                static_cast<UnlockArea*>(lockLandArea)->setActive (false);
                break;
        }
        // And update the DnD icon based on the current state :
        updateDragAndDropIcon();
    }
}

void LockScreenViewWithPadlock::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);
    bool unlock = false;
    MFeedback feedback;

    if (dragAndDropState == STATE_NONE) {
        return;
    }

    if (dragAndDropState  == STATE_MOVING_ACTIVE) {
        unlock = true;
    }

    /*
     * Playing the appropriate feedback.
     */
    if (unlock) {
        feedback.setName ("release-inside-dragndrop-dropzone");
    } else {
        feedback.setName ("release-outside-dragndrop-dropzone");
    }
    feedback.play ();

    // Reset the state to defaults...
    resetState ();

    if (unlock == true) {
        qDebug() << "emitting unlocked";
        emit unlocked ();
    }
}

void LockScreenViewWithPadlock::resetState ()
{
    // Restore the default state ...
    if (lockLiftArea != 0)
        static_cast<UnlockHeader*>(lockLiftArea)->setActive (true);
    if (lockLandArea != 0)
        static_cast<UnlockArea*> (lockLandArea)->setEnabled (false);

    dragAndDropState = STATE_NONE;
    updateDragAndDropIcon();
}

void LockScreenViewWithPadlock::updateDragAndDropIcon()
{
    bool enable = (dragAndDropState != STATE_NONE);
    // Icon not yet created... make it:
    if (dragAndDropIcon == 0)
    {
        dragAndDropIcon = new MImageWidget;

        dragAndDropIcon->setImage ("icon-m-common-locked", QSize (64, 64));
        dragAndDropIcon->setObjectName ("LockScreenDnDIcon");
        dragAndDropIcon->setZoomFactor (1.0);

        dragAndDropOverlay.setWidget (dragAndDropIcon);
    }

    // Update the theming...
    if ((dragAndDropState == STATE_MOVING_ACTIVE) &&
        (dragAndDropIcon->objectName () != "LockScreenDnDIconActive"))
    {
        dragAndDropIcon->setObjectName ("LockScreenDnDIconActive");
        dragAndDropIcon->update ();
    }
    else if (dragAndDropIcon->objectName () != "LockScreenDnDIcon")
    {
        dragAndDropIcon->setObjectName ("LockScreenDnDIcon");
        dragAndDropIcon->update ();
    }

    dragAndDropOverlay.setVisible (enable);
    if (enable)
    {
        controller->sceneManager()->appearSceneWindow(&dragAndDropOverlay);
    } else {
        dragAndDropOverlay.disappear ();
    }
}

void LockScreenViewWithPadlock::showHideNotifications (bool show)
{
    if (notificationArea == 0)
        return;

    // Hide the whole missed events notification area when
    // there is no any missed events...
    if (notificationArea->isVisible () && (show == false))
    {
        notificationArea->setVisible (false);
        layoutPolicy->removeItem (notificationArea);
    }

    // Add notification area to policy when previously was
    // hidden, but there are some missed events...
    if ((notificationArea->isVisible () == false) && (show == true))
    {
        notificationArea->setVisible (true);
        layoutPolicy->insertItem (0, notificationArea);
    }
}

void LockScreenViewWithPadlock::updateDataTime()
{
    static_cast<UnlockHeader *> (lockLiftArea)->updateDateTime ();
}

M_REGISTER_VIEW_NEW(LockScreenViewWithPadlock, LockScreen)
