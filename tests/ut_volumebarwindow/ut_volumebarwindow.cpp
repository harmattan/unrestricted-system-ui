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
#include "ut_volumebarwindow.h"
#include <volumebarwindow.h>
#include <volumebarlogic.h>
#include <volumebar.h>
#include <MApplication>
#include <QGraphicsItem>
#include <MWindow>
#include <MOverlay>
#include <QDebug>

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
/*********************************************************************************
 * Stub for ResourcePolicy
 */

namespace ResourcePolicy
{
    ResourceSet::ResourceSet(const QString&, QObject*)
    {
    }

    bool ResourceSet::setAlwaysReply ()
    {
        return true;
    }


    ScaleButtonResource::ScaleButtonResource()
    {
    }

    void ResourceSet::addResourceObject (ResourcePolicy::Resource* rsc)
    {
        Q_UNUSED(rsc);
        return;
    }

    bool ResourceSet::acquire ()
    {
        return true;
    }

    void ResourceSet::deleteResource (ResourcePolicy::ResourceType)
    {
        return;
    }
}
#endif // HAVE_LIBRESOURCEQT


#ifdef HAVE_QMSYSTEM
// For Hw-volume key handling
#include <qmkeys.h>
#include <qmlocks.h>
/*********************************************************************************
 * Stub for Maemo::QmKeys && Maemo::QmLocks
 */
namespace Maemo
{
    QmKeys::QmKeys(QObject *parent)
    {
        Q_UNUSED(parent);
    }

    QmKeys::~QmKeys()
    {
    }

    QmLocks::State qmlocksStubRetval;

    QmLocks::QmLocks(QObject *parent)
    {
        Q_UNUSED(parent);

        qmlocksStubRetval = Maemo::QmLocks::Unlocked;
    }

    QmLocks::~QmLocks()
    {
    }

    QmLocks::State QmLocks::getState(QmLocks::Lock what) const
    {
        Q_UNUSED(what);
        return qmlocksStubRetval;
    }
}
#endif

/*********************************************************************************
 * Stub for VolumeBar
 */
VolumeBar::VolumeBar(QGraphicsItem *parent)
{
    Q_UNUSED(parent);
}

VolumeBar::~VolumeBar()
{

}

void VolumeBar::updateVolume (int val, int max)
{
    Q_UNUSED(val);
    Q_UNUSED(max);
}

void VolumeBar::updateContents ()
{

}

void VolumeBar::applyStyle ()
{

}

void VolumeBar::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
}

void VolumeBar::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
}

void VolumeBar::finishAnimations ()
{

}

/*********************************************************************************
 * Stub for VolumeBarLogic
 */
VolumeBarLogic::VolumeBarLogic ()
{
    currentvolume = 20;
}

VolumeBarLogic::~VolumeBarLogic ()
{
}

quint32 VolumeBarLogic::volume ()
{
        return currentvolume;
}

quint32 VolumeBarLogic::maxVolume ()
{
        return 100;
}

void VolumeBarLogic::ping ()
{
}

void VolumeBarLogic::setVolume (quint32 value)
{
    currentvolume = value;
}

void VolumeBarLogic::stepsUpdated (quint32 value, quint32 maxvalue)
{
    Q_UNUSED(value);
    Q_UNUSED(maxvalue);
}

void VolumeBarLogic::initValues ()
{
}

void VolumeBarLogic::addSignalMatch ()
{
}

void VolumeBarLogic::openConnection (bool init)
{
    Q_UNUSED(init);
}

/*********************************************************************************
 * stub for MWindow
 */
bool isWindowVisible = false;

void MWindow::setVisible (bool visible)
{
    isWindowVisible = visible;
}

/*********************************************************************************
 * The Ut_VolumeBarWindow implements the unit tests.
 */
void Ut_VolumeBarWindow::init ()
{
    volumeBarWindow = new VolumeBarWindow;
    QVERIFY(volumeBarWindow->logic != 0);
#ifdef HAVE_QMSYSTEM
    QVERIFY(volumeBarWindow->hwkeys != 0);
    QVERIFY(volumeBarWindow->locks != 0);
#endif
}

void Ut_VolumeBarWindow::cleanup ()
{
    delete volumeBarWindow;
}

static int argc = 1;
static char *argv[] = { (char *) "./ut_volumebarwindow", 0 };

void Ut_VolumeBarWindow::initTestCase ()
{
    app = new MApplication (argc, argv); 
}

void Ut_VolumeBarWindow::cleanupTestCase ()
{
    app->deleteLater ();
}

void Ut_VolumeBarWindow::testVolumeBarChanged ()
{
    volumeBarWindow->volumeBarChanged(32);
    QVERIFY(volumeBarWindow->logic->volume() == 32);
}

#ifdef HAVE_QMSYSTEM
void Ut_VolumeBarWindow::testHwKeyEvent()
{
    // when current_volume >= max_volume
    volumeBarWindow->volumeBarChanged(120);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 99u);

    // Turn up the volume
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 21u);

    // Turn down the volume
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeDown, Maemo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 19u);

    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyUp);
    QCOMPARE(volumeBarWindow->logic->volume(), 20u);

    // push some other hw key
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::Camera, Maemo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 20u);
}

void Ut_VolumeBarWindow::testLocking()
{
    // Device locked
    volumeBarWindow->locksChanged (Maemo::QmLocks::Device, Maemo::QmLocks::Locked);
    QCOMPARE (volumeBarWindow->locked, true);

    // Turn up the volume
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 21u);

    // window should be non-visible
    QCOMPARE (isWindowVisible, false);

    // Turn down the volume
    volumeBarWindow->volumeBarChanged(20);
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeDown, Maemo::QmKeys::KeyDown);
    QCOMPARE(volumeBarWindow->logic->volume(), 19u);

    // window should be non-visible
    QCOMPARE (isWindowVisible, false);

    // Lock also the touchscreen & keyboard
    volumeBarWindow->locksChanged (Maemo::QmLocks::TouchAndKeyboard, Maemo::QmLocks::Locked);
    QCOMPARE (volumeBarWindow->locked, true);

    // Unlock only the device
    /* TouchscreenAndKeyboard is still locked: */
    Maemo::qmlocksStubRetval = Maemo::QmLocks::Locked;
    volumeBarWindow->locksChanged (Maemo::QmLocks::Device, Maemo::QmLocks::Unlocked);
    QCOMPARE (volumeBarWindow->locked, true);

    // And now unlock the touch-screen also:
    /* Device is unlocked already:: */
    Maemo::qmlocksStubRetval = Maemo::QmLocks::Unlocked;
    volumeBarWindow->locksChanged (Maemo::QmLocks::TouchAndKeyboard, Maemo::QmLocks::Unlocked);
    QCOMPARE (volumeBarWindow->locked, false);

    /* Press a volume down key */
    volumeBarWindow->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    // window should be visible now
    QCOMPARE (isWindowVisible, true);
}
#endif

QTEST_APPLESS_MAIN(Ut_VolumeBarWindow)
