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
#include "../../src/volumecontrol/volumecontrolui.h"
#include <volumebarlogic.h>
#include <volumeoverlay.h>
#include <QGraphicsItem>
#include <MWindow>
#include <MOverlay>
#include <QDebug>

#ifdef HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
#endif

#ifdef HAVE_QMSYSTEM
// For Hw-volume key handling
#include <qmkeys.h>
#endif

/*********************************************************************************
 * Stub for ResourcePolicy
 */

namespace ResourcePolicy
{
    ResourceSet::ResourceSet(const QString&, QObject*)
    {
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


/*********************************************************************************
 * Stub for Maemo::QmKeys
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
}

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
VolumeOverlay::orientationChanged (M::Orientation orientation)
{
    Q_UNUSED(orientation);
}

void
VolumeOverlay::hideMe ()
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
    QVERIFY(m_Api->m_logic != NULL);
    QVERIFY(m_Api->m_hwkeys != NULL);
}

void
Ut_VolumeControlUI::testOverlayChanged()
{
    m_Api->overlayChanged(32);
    QVERIFY(m_Api->m_logic->getVolume() == 32);
}

void
Ut_VolumeControlUI::testHwKeyEvent()
{
    // when current_volume >= max_volume
    m_Api->overlayChanged(120);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QVERIFY(m_Api->m_logic->getVolume() == 99);

    // Turn up the volume
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyDown);
    QVERIFY(m_Api->m_logic->getVolume() == 21);

    // Turn down the volume
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeDown, Maemo::QmKeys::KeyDown);
    QVERIFY(m_Api->m_logic->getVolume() == 19);

    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::VolumeUp, Maemo::QmKeys::KeyUp);
    QVERIFY(m_Api->m_logic->getVolume() == 20);

    // push some other hw key
    m_Api->overlayChanged(20);
    m_Api->hwKeyEvent(Maemo::QmKeys::Camera, Maemo::QmKeys::KeyDown);
    QVERIFY(m_Api->m_logic->getVolume() == 20);
}

void
Ut_VolumeControlUI::cleanupTestCase ()
{
    delete m_Api;
}

QTEST_MAIN(Ut_VolumeControlUI)
