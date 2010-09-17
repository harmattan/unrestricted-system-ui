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
#include "ut_volumecontrolui.h"
#include "volumecontrolui.h"
#include <volumebarlogic.h>
#include <volumeoverlay.h>
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
    ResourceSet::ResourceSet(const QString& str, QObject* o, bool a, bool b)
    {
        Q_UNUSED(str);
        Q_UNUSED(o);
        Q_UNUSED(a);
        Q_UNUSED(b);
    }

    bool
    ResourceSet::setAlwaysReply ()
    {
        return true;
    }


    ScaleButtonResource::ScaleButtonResource()
    {
    }

    void
    ResourceSet::addResourceObject (ResourcePolicy::Resource* rsc)
    {
        Q_UNUSED(rsc);
        return;
    }

    bool
    ResourceSet::acquire ()
    {
        return true;
    }

    void
    ResourceSet::deleteResource (ResourcePolicy::ResourceType)
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
        QTest::qWarn("Maemo::QmKeys");
    }

    QmKeys::~QmKeys()
    {
        QTest::qWarn("Maemo::~QmKeys");
    }

    QmLocks::State qmlocksStubRetval;

    QmLocks::QmLocks(QObject *parent)
    {
        Q_UNUSED(parent);
        QTest::qWarn("Maemo::QmLocks");

        qmlocksStubRetval = Maemo::QmLocks::Unlocked;
    }

    QmLocks::~QmLocks()
    {
        QTest::qWarn("Maemo::~QmLocks");
    }

    QmLocks::State
    QmLocks::getState(QmLocks::Lock what) const
    {
        Q_UNUSED(what);
        return qmlocksStubRetval;
    }
}
#endif

/*********************************************************************************
 * Stub for VolumeOverlay
 */
VolumeOverlay::VolumeOverlay(QGraphicsItem *parent)
{
    Q_UNUSED(parent);
}

VolumeOverlay::~VolumeOverlay()
{

}

void
VolumeOverlay::UpdateVolume (int val, int max)
{
    Q_UNUSED(val);
    Q_UNUSED(max);
}

void
VolumeOverlay::hideMe ()
{

}

void
VolumeOverlay::updateMask ()
{

}

void
VolumeOverlay::removeMask ()
{

}

/*********************************************************************************
 * Stub for VolumeBarLogic
 */
VolumeBarLogic::VolumeBarLogic ()
{
    m_currentvolume = 20;
}

VolumeBarLogic::~VolumeBarLogic ()
{
}

quint32
VolumeBarLogic::getVolume ()
{
        return m_currentvolume;
}

quint32
VolumeBarLogic::getMaxVolume ()
{
        return 100;
}

void
VolumeBarLogic::ping ()
{
}

void
VolumeBarLogic::setVolume (quint32 value)
{
    m_currentvolume = value;
}

void
VolumeBarLogic::stepsUpdated (quint32 value, quint32 maxvalue)
{
    Q_UNUSED(value);
    Q_UNUSED(maxvalue);
}

void
VolumeBarLogic::initValues ()
{
}

void
VolumeBarLogic::addSignalMatch ()
{
}

void
VolumeBarLogic::openConnection (bool init)
{
    Q_UNUSED(init);
}


/*********************************************************************************
 * The Ut_VolumeControlUI implements the unit tests.
 */
void
Ut_VolumeControlUI::initTestCase ()
{
    m_Api = new VolumeControlUI;
    QVERIFY(m_Api->m_logic != 0);
#ifdef HAVE_QMSYSTEM
    QVERIFY(m_Api->m_hwkeys != 0);
    QVERIFY(m_Api->m_locks != 0);
#endif
}

void
Ut_VolumeControlUI::testOverlayChanged()
{
    m_Api->overlayChanged(32);
    QVERIFY(m_Api->m_logic->getVolume() == 32);
}

#ifdef HAVE_QMSYSTEM
void
Ut_VolumeControlUI::testHwKeyEvent()
{
    // when current_volume >= max_volume
    m_Api->overlayChanged(120);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QCOMPARE(m_Api->m_logic->getVolume(), 99u);

    // Turn up the volume
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QCOMPARE(m_Api->m_logic->getVolume(), 21u);

    // Turn down the volume
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeDown, Maemo::QmKeys::KeyDown);
    QCOMPARE(m_Api->m_logic->getVolume(), 19u);

    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyUp);
    QCOMPARE(m_Api->m_logic->getVolume(), 20u);

    // push some other hw key
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::Camera, Maemo::QmKeys::KeyDown);
    QCOMPARE(m_Api->m_logic->getVolume(), 20u);
}

void
Ut_VolumeControlUI::testLocking()
{
    // Device locked
    m_Api->locksChanged (Maemo::QmLocks::Device, Maemo::QmLocks::Locked);
    QCOMPARE (m_Api->m_locked, true);

    /*
     * Volume controlling should work,
     * but overlay should not be shown:
     */
    if (m_Api->m_overlay != 0)
    {
        delete m_Api->m_overlay;
        m_Api->m_overlay = 0;
    }

    // Turn up the volume
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QCOMPARE(m_Api->m_logic->getVolume(), 21u);

    // Check m_overlay, should not be instantiated
    QVERIFY(m_Api->m_overlay == 0);

    // Turn down the volume
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeDown, Maemo::QmKeys::KeyDown);
    QCOMPARE(m_Api->m_logic->getVolume(), 19u);

    // Check m_overlay, should not be instantiated
    QVERIFY(m_Api->m_overlay == 0);

    // Lock also the touchscreen & keyboard
    m_Api->locksChanged (Maemo::QmLocks::TouchAndKeyboard, Maemo::QmLocks::Locked);
    QCOMPARE (m_Api->m_locked, true);

    // Unlock only the device
    /* TouchscreenAndKeyboard is still locked: */
    Maemo::qmlocksStubRetval = Maemo::QmLocks::Locked;
    m_Api->locksChanged (Maemo::QmLocks::Device, Maemo::QmLocks::Unlocked);
    QCOMPARE (m_Api->m_locked, true);

    // And now unlock the touch-screen also:
    /* Device is unlocked already:: */
    Maemo::qmlocksStubRetval = Maemo::QmLocks::Unlocked;
    m_Api->locksChanged (Maemo::QmLocks::TouchAndKeyboard, Maemo::QmLocks::Unlocked);
    QCOMPARE (m_Api->m_locked, false);

    /* Press a volume down key */
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    /* Overlay should be instantiated: */
    QVERIFY (m_Api->m_overlay != 0);
}
#endif

void
Ut_VolumeControlUI::cleanupTestCase ()
{
    delete m_Api;
}

QTEST_MAIN(Ut_VolumeControlUI)
