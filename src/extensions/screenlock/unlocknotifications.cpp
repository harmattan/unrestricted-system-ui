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
#include "unlocknotifications.h"
#include "unlockmissedevents.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <QPixmap>
#include <MImageWidget>
#include <MSceneManager>
#include <MStylableWidget>
#include <MOrientationChangeEvent>

#define ICON_SIZE 32

UnlockNotifications::UnlockNotifications () :
    m_vbox (0)
{
    m_icon_ids[UnlockMissedEvents::NotifyEmail] =
        QString ("icon-m-content-email");
    m_icon_ids[UnlockMissedEvents::NotifySms] =
        QString ("icon-m-content-sms");
    m_icon_ids[UnlockMissedEvents::NotifyMessage] =
        QString ("icon-m-content-chat");
    m_icon_ids[UnlockMissedEvents::NotifyCall] =
        QString ("icon-m-content-call");
    m_icon_ids[UnlockMissedEvents::NotifyOther] =
        QString ("icon-m-content-event");

 /*
  * Create the "other events area"
  */
    m_otherevents_area = new MStylableWidget;
    m_otherevents_area->setObjectName ("LockOtherEventsArea");
    m_otherevents_area->setContentsMargins (0., 0., 0., 0.);

    m_icon_layout = new QGraphicsLinearLayout (Qt::Horizontal);
    m_icon_layout->setContentsMargins (0., 0., 0., 0.);

    m_otherevents_area->setLayout (m_icon_layout);

 /*
  * Create the "most recent notification" layout
  */
    m_mostrecent_layout = new QGraphicsLinearLayout (Qt::Horizontal);
    /* Everything came from CSS, no need for default margins */
    m_mostrecent_layout->setContentsMargins (0., 0., 0., 0.);

    m_last_icon = new MImageWidget;
    m_last_icon->setZoomFactor (1.0);
    m_last_icon->setObjectName ("LockMostRecentIcon");

    m_last_subject = new MLabel;
    m_last_subject->setObjectName ("LockMostRecentLabel");
    m_last_subject->setTextElide (true);
    m_last_subject->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Maximum);

    /* We have to add the last icon / subject to the other events area at first,
     * because the scene-window orientation is landscape by default... */
    m_icon_layout->addItem (m_last_icon);
    m_icon_layout->addItem (m_last_subject),

 /*
  * Create the "most recent area"
  */
    m_mostrecent_area = new MStylableWidget;
    m_mostrecent_area->setObjectName ("LockMostRecentArea");
    m_mostrecent_area->setContentsMargins (0., 0., 0., 0.);

    m_mostrecent_area->setLayout (m_mostrecent_layout);
    m_mostrecent_area->setVisible (false);

 /*
  * Create the main vertical box
  */
    m_vbox = new QGraphicsLinearLayout (Qt::Vertical);
    m_vbox->setContentsMargins (0., 0., 0., 0.);
    m_vbox->setSpacing (0.);

 /*
  * And add the "persistent item" the other events area to it
  */
    m_vbox->addItem (m_otherevents_area);

    setLayout (m_vbox);

    connect (&UnlockMissedEvents::getInstance (), SIGNAL (updated ()),
             this, SLOT (updateContents ()));
}

UnlockNotifications::~UnlockNotifications ()
{
    m_labels.clear ();
    m_icon_ids.clear ();
}

QSizeF
UnlockNotifications::sizeHint (Qt::SizeHint which,
                               const QSizeF& constraint) const
{
    if (m_vbox == 0)
        return QSizeF (0., 0.);
    /*
     * forward sizeHint queries to main vbox
     */
    return m_vbox->sizeHint (which, constraint);
}

void
UnlockNotifications::orientationChangeEvent (
        MOrientationChangeEvent *event)
{
    orientationChanged (event->orientation ());
}

void
UnlockNotifications::orientationChanged (
        M::Orientation orientation)
{
    if (orientation == M::Landscape)
    {
        /*
         * Remove & hide the most recent area from top
         */
        m_vbox->removeItem (m_mostrecent_area);
        m_mostrecent_area->setVisible (false);

        /*
         * Add most recent event widgets to other events layout
         */
        m_mostrecent_layout->removeItem (m_last_subject);
        m_icon_layout->insertItem (0, m_last_subject);
        m_mostrecent_layout->removeItem (m_last_icon);
        m_icon_layout->insertItem (0, m_last_icon);
        /*
         * Re-size the other events area a bit bigger...
         */
        m_otherevents_area->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Maximum);
    }
    else
    {
        /*
         * Remove the most recent event widgets from other events layout
         */
        m_icon_layout->removeItem (m_last_icon);
        m_icon_layout->removeItem (m_last_subject);
        m_mostrecent_layout->insertItem (0, m_last_subject);
        m_mostrecent_layout->insertItem (0, m_last_icon);

        /*
         * Add & show the most recent area on top
         */
        m_vbox->insertItem (0, m_mostrecent_area);
        m_mostrecent_area->setVisible (true);

        /*
         * Re-size the other events area to a bit smaller
         */
        m_otherevents_area->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Minimum);
    }

    /*
     * to re-calculate the sizes...
     */
    m_vbox->invalidate ();
}

void
UnlockNotifications::updateContents ()
{
    UnlockMissedEvents::Types mostRecent =
        UnlockMissedEvents::getInstance ().getLastType ();

    if (mostRecent == UnlockMissedEvents::NotifyLast)
    {
        emit needToShow (false);

        /*
         * Clear out all the missed events...
         */
        foreach (MLabel *label, m_labels)
            delete label;
        m_labels.clear ();

        foreach (MImageWidget *icon, m_icons)
            delete icon;
        m_icons.clear ();

        m_last_subject->setText ("");
        m_last_icon->setImage ("");
    }
    else
    {
        emit needToShow (true);
        /*
         * It seems that we don't get the signal, forcing it manually to detect
         * the orientation.
         */
        if (sceneManager())
            orientationChanged (sceneManager()->orientation());

        /*
         * Check the actually shown most-recent notification
         * type... (yeah it is hacky a bit...)
         */
        int actualType = m_icon_ids.key (m_last_icon->image ());
        int eventCount =
            UnlockMissedEvents::getInstance ().getCount (mostRecent);

        /*
         * Sms and call must be the highest priority....
         * [so it should shown as most recent even there
         *  are newer other events...]
         */
        if (((actualType != UnlockMissedEvents::NotifySms) &&
             (actualType != UnlockMissedEvents::NotifyCall)) ||
            (mostRecent == UnlockMissedEvents::NotifySms) ||
            (mostRecent == UnlockMissedEvents::NotifyCall))
        {
            m_last_icon->setImage (m_icon_ids[mostRecent],
                                   QSize (ICON_SIZE, ICON_SIZE));

            QString mostRecentText =
                UnlockMissedEvents::getInstance ().getLastSubject (mostRecent);

            if (eventCount > 1)
            {
                switch (mostRecent)
                {
                    case UnlockMissedEvents::NotifyCall:
                        mostRecentText =
                            //% "%1 missed calls"
                            qtTrId ("qtn_scrl_missed_call").arg (eventCount);
                        break;
                    case UnlockMissedEvents::NotifySms:
                        mostRecentText =
                            //% "%1 text messages"
                            qtTrId ("qtn_scrl_sms").arg (eventCount);
                        break;
                    case UnlockMissedEvents::NotifyEmail:
                        mostRecentText =
                            //% "%1 emails"
                            qtTrId ("qtn_scrl_email").arg (eventCount);
                        break;
                    case UnlockMissedEvents::NotifyMessage:
                        mostRecentText =
                            //% "%1 chats"
                            qtTrId ("qtn_scrl_chat").arg (eventCount);
                        break;
                    default:
                        break;
                }
            }

            m_last_subject->setText (mostRecentText);
        }

        if (m_labels.value (mostRecent, 0) != 0)
        {
            /*
             * Icon+Label exists remove it from the layout
             */
            m_icon_layout->removeItem (m_labels.value (mostRecent));
            m_icon_layout->removeItem (m_icons.value (mostRecent));
        }
        else
        {
            /*
             * Create the label & icon
             */
            m_labels[mostRecent] = new MLabel;
            m_labels[mostRecent]->setObjectName ("LockNotifierLabel");
            m_icons[mostRecent] = new MImageWidget;
            m_icons[mostRecent]->setImage (m_icon_ids[mostRecent],
                                           QSize (ICON_SIZE, ICON_SIZE));
            m_icons[mostRecent]->setZoomFactor (1.0);
            m_icons[mostRecent]->setObjectName ("LockNotifierIcon");
        }

        m_labels[mostRecent]->setText (QString ("%L1").arg (eventCount));

        /*
         * Most recent area only visible when orientation is portrait
         */
        int newIndex = (m_mostrecent_area->isVisible () == false) ? 2 : 0;
        /* Somehow isVisible sometimes lying at first call :-S */
        if (m_icon_layout->count () == 0)
            newIndex = 0;

        /*
         * Put the new icons to the proper place...
         */
        m_icon_layout->insertItem (newIndex, m_labels[mostRecent]);
        m_icon_layout->setAlignment (m_labels[mostRecent], Qt::AlignLeft);
        m_icon_layout->insertItem (newIndex, m_icons[mostRecent]);
        m_icon_layout->setAlignment (m_icons[mostRecent], Qt::AlignLeft);
    }
}

