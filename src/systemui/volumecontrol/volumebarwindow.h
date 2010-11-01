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
#ifndef VOLUMEBARWINDOW_H
#define VOLUMEBARWINDOW_H

#include <MWindow>

class MOverlay;
class VolumeBarLogic;
class VolumeBar;

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
#endif

#ifdef HAVE_QMSYSTEM
// For Hw-volume key handling
#include <qmkeys.h>
#include <qmlocks.h>
#endif

/*! The VolumeBarWindow allows to display a volume-bar based on current-volume
 *  on hardware-volume-key presses and lets user to modify the volume by touching
 *  the full-screen VolumeBar widget...
 */
class VolumeBarWindow : public MWindow
{
    Q_OBJECT

public:
    //! Constructs a new VolumeBarWindow instance
    VolumeBarWindow (QWidget *parent = 0);
    //! Destructs a VolumeBarWindow instance
    ~VolumeBarWindow ();

private slots:
    /*! Internal slot to handle the VolumeBar changes
     * \val the new volume level
     */
    void volumeBarChanged (int val);
#ifdef HAVE_QMSYSTEM
    /*! Internal slot to handle the hardware volume-key presses (see QmKeys API documentation)
     * \key the key type
     * \state the key new state
     */
    void hwKeyEvent (MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);
    /*! An internal slot to handle the screen and/or device lock changes
     * (volume-bar shouldn't appear on top of these) (see QmLocks API documentation)
     * \locks what the type of lock
     * \how the lock new state
     */
    void locksChanged (MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
#endif
    //! An internal slot to handle the case when we got the hardware volume keys resource
    void hwKeyResourceAcquired ();
    //! An internal slot to handle the case when we lost the hardware volume keys resource
    void hwKeyResourceLost ();

private:
    Q_DISABLE_COPY(VolumeBarWindow)

    //! The PulseAudio MainVolume1 wrapper class
    VolumeBarLogic  *logic;
    //! The VolumeBar widget
    VolumeBar       *volumeBar;
    //! An overlay which contains the VolumeBar widget
    MOverlay        *overlay;
#ifdef HAVE_QMSYSTEM
    //! an QmKeys instance to get signals about volume-key presses
    MeeGo::QmKeys   *hwkeys;
    //! a QmLocks instance to get signals about screen/device lock changes
    MeeGo::QmLocks  *locks;
#endif
    //! here we store the current lock-state
    bool             locked;

#ifdef HAVE_LIBRESOURCEQT
    //! A resource object for volume(zoom)-hardware keys
    ResourcePolicy::ResourceSet *hwkeyResource;
#endif

#ifdef UNIT_TEST
    friend class Ut_VolumeBarWindow;
#endif
};

#endif
